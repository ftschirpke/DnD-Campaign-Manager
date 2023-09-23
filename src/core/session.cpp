#include <dnd_config.hpp>

#include "session.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <future>
#include <string>

#include <nlohmann/json.hpp>

#include <core/content.hpp>
#include <core/content_visitors/list_visitor.hpp>
#include <core/content_visitors/session_visitor.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/content_piece.hpp>
#include <core/parsing/content_parser.hpp>
#include <core/searching/content_search.hpp>
#include <core/utils/char_manipulation.hpp>

dnd::Session::Session(const char* last_session_filename)
    : last_session_filename(last_session_filename), status(SessionStatus::CONTENT_DIR_SELECTION), content_directory(),
      campaign_name(), last_session_open_tabs(), search(), search_results(), search_result_count(0),
      open_content_pieces(), search_result_strings(), error_messages(), parsing_results(), parser(), errors(),
      content() {}

dnd::Session::~Session() { save_session_values(); }

dnd::SessionStatus dnd::Session::get_status() const noexcept { return status; }

dnd::Content& dnd::Session::get_content() noexcept { return content; }

const dnd::Errors& dnd::Session::get_errors() const noexcept { return errors; }

const std::vector<std::string>& dnd::Session::get_error_messages() const { return error_messages; }

const std::string& dnd::Session::get_campaign_name() const noexcept { return campaign_name; }

const std::filesystem::path& dnd::Session::get_content_directory() const noexcept { return content_directory; }

std::deque<const dnd::ContentPiece*>& dnd::Session::get_open_content_pieces() noexcept { return open_content_pieces; }

size_t dnd::Session::get_search_result_count() const noexcept { return search_result_count; }

std::vector<std::string> dnd::Session::get_possible_campaign_names() const {
    if (content_directory.empty()) {
        return {};
    }
    std::vector<std::string> campaign_names;
    for (const auto& entry : std::filesystem::directory_iterator(content_directory)) {
        if (!entry.is_directory() || entry.path().filename() == "general") {
            continue;
        }
        campaign_names.emplace_back(entry.path().filename().string());
    }
    return campaign_names;
}

std::vector<std::string> dnd::Session::get_search_result_strings() const {
    ListVisitor list_visitor;
    if (search_result_count > max_search_results) {
        return {};
    }
    for (size_t i = 0; i < search_result_count; ++i) {
        search_results[i]->accept(&list_visitor);
    }
    return list_visitor.get_list();
}

void dnd::Session::retrieve_last_session_values() {
    if (!std::filesystem::exists(last_session_filename)) {
        return;
    }
    std::ifstream last_session_file(last_session_filename);
    if (!last_session_file.is_open()) {
        return;
    }
    nlohmann::json last_session;
    last_session_file >> last_session;
    last_session_file.close();

    if (!last_session.contains("content_directory")) {
        return;
    }
    set_content_directory(last_session["content_directory"].get<std::filesystem::path>());
    if (last_session.contains("campaign_name")) {
        set_campaign_name(last_session["campaign_name"].get<std::string>());
    }

    if (!last_session.contains("open_tabs")) {
        return;
    }
    if (!last_session["open_tabs"].is_object()) {
        return;
    }
    last_session_open_tabs = last_session["open_tabs"].get<std::unordered_map<std::string, std::vector<std::string>>>();
}

void dnd::Session::save_session_values() {
    nlohmann::json last_session;
    if (!content_directory.empty()) {
        last_session["content_directory"] = content_directory.string();
        if (!campaign_name.empty()) {
            last_session["campaign_name"] = campaign_name;
        }
    }

    SessionVisitor session_visitor;
    for (const auto open_content_piece : open_content_pieces) {
        open_content_piece->accept(&session_visitor);
    }
    last_session["open_tabs"] = session_visitor.get_open_tabs();

    std::ofstream last_session_file(last_session_filename);
    last_session_file << std::setw(4) << last_session;
    last_session_file.close();
}

void dnd::Session::clear_error_messages() { error_messages.clear(); }

bool dnd::Session::set_campaign_name(const std::string& new_campaign_name) {
    if (campaign_name == new_campaign_name) {
        return !campaign_name.empty();
    }
    std::vector<std::string> possible_names = get_possible_campaign_names();
    if (std::find(possible_names.begin(), possible_names.end(), new_campaign_name) == possible_names.end()) {
        return false;
    }
    campaign_name = new_campaign_name;
    status = SessionStatus::PARSING;
    start_parsing();
    return true;
}

static dnd::Errors validate_content_directory(const std::filesystem::path& content_directory) {
    dnd::Errors errors;
    if (!std::filesystem::exists(content_directory)) {
        errors.add_validation_error(
            dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, nullptr, "The content directory does not exist."
        );
    } else if (!std::filesystem::is_directory(content_directory)) {
        errors.add_validation_error(
            dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, nullptr, "The content directory is not a directory."
        );
    } else if (!std::filesystem::exists(content_directory / "general")) {
        errors.add_validation_error(
            dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, nullptr,
            "The content directory does not contain a \"general\" directory."
        );
    } else if (!std::filesystem::is_directory(content_directory / "general")) {
        errors.add_validation_error(
            dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, nullptr,
            "The content directory does not contain a \"general\" directory, but a file with the same name."
        );
    }
    return errors;
}

bool dnd::Session::set_content_directory(const std::filesystem::path& new_content_directory) {
    if (content_directory == new_content_directory) {
        return !content_directory.empty();
    }
    bool valid = validate_content_directory(new_content_directory).ok();
    if (valid) {
        content_directory = new_content_directory;
        campaign_name.clear();
        status = SessionStatus::CAMPAIGN_SELECTION;
    } else {
        status = SessionStatus::CONTENT_DIR_SELECTION;
    }
    return valid;
}

// A comparator for content pieces that sorts them by name and prioritizes those whose name starts with a givencharacter
class ContentPieceComparator {
public:
    explicit ContentPieceComparator(char leading_character)
        : leading_character(dnd::char_to_lowercase(leading_character)) {}
    bool starts_with_leading_character(const std::string& name) const {
        return name[0] == leading_character || name[0] == dnd::char_to_uppercase(leading_character);
    }
    bool operator()(const dnd::ContentPiece* a, const dnd::ContentPiece* b) const {
        const std::string& a_name = a->get_name();
        const std::string& b_name = b->get_name();
        bool prioritize_a = starts_with_leading_character(a_name);
        bool prioritize_b = starts_with_leading_character(b_name);
        if (prioritize_a && !prioritize_b) {
            return true;
        }
        if (!prioritize_a && prioritize_b) {
            return false;
        }
        return a->get_name() < b->get_name();
    }
private:
    char leading_character;
};

void dnd::Session::set_search_query(const std::string& new_search_query) {
    search->set_search_query(new_search_query);
    std::vector<const ContentPiece*> vec_search_results = search->get_results();
    search_result_count = vec_search_results.size();
    if (search_result_count > max_search_results) {
        return;
    }
    ContentPieceComparator comparator(new_search_query[0]);
    std::sort(vec_search_results.begin(), vec_search_results.end(), comparator);
    for (size_t i = 0; i < search_result_count; ++i) {
        search_results[i] = vec_search_results[i];
    }
}

void dnd::Session::open_search_result(size_t index) {
    if (index >= search_result_count) {
        return;
    }
    const ContentPiece* content_piece = search_results[index];
    open_content_pieces.push_back(content_piece);
}

void dnd::Session::update() {
    if (status == SessionStatus::PARSING) {
        if (parsing_results.wait_for(std::chrono::microseconds(100)) == std::future_status::ready) {
            finish_parsing();
        }
    }
}

void dnd::Session::start_parsing() {
    parsing_results = std::async(std::launch::async, &ContentParser::parse, &parser, content_directory, campaign_name);
    status = SessionStatus::PARSING;
}

void dnd::Session::finish_parsing() {
    try {
        ParsingResult parsing_result = parsing_results.get();
        content = std::move(parsing_result.content);
        errors = std::move(parsing_result.errors);
        search = std::make_unique<ContentSearch>(content);
        status = SessionStatus::READY;
    } catch (const std::exception& e) {
        error_messages.push_back(e.what());
        status = SessionStatus::UNKNOWN_ERROR;
    }
}

void dnd::Session::open_last_session() {
    for (const std::string& character_to_open : last_session_open_tabs["characters"]) {
        if (content.get_characters().contains(character_to_open)) {
            open_content_pieces.push_back(&content.get_characters().get(character_to_open));
        }
    }
    for (const std::string& character_class_to_open : last_session_open_tabs["character_classes"]) {
        if (content.get_character_classes().contains(character_class_to_open)) {
            open_content_pieces.push_back(&content.get_character_classes().get(character_class_to_open));
        }
    }
    for (const std::string& character_subclass_to_open : last_session_open_tabs["character_subclasses"]) {
        if (content.get_character_subclasses().contains(character_subclass_to_open)) {
            open_content_pieces.push_back(&content.get_character_subclasses().get(character_subclass_to_open));
        }
    }
    for (const std::string& character_race_to_open : last_session_open_tabs["character_races"]) {
        if (content.get_character_races().contains(character_race_to_open)) {
            open_content_pieces.push_back(&content.get_character_races().get(character_race_to_open));
        }
    }
    for (const std::string& character_subrace_to_open : last_session_open_tabs["character_subraces"]) {
        if (content.get_character_subraces().contains(character_subrace_to_open)) {
            open_content_pieces.push_back(&content.get_character_subraces().get(character_subrace_to_open));
        }
    }
    for (const std::string& item_to_open : last_session_open_tabs["items"]) {
        if (content.get_items().contains(item_to_open)) {
            open_content_pieces.push_back(&content.get_items().get(item_to_open));
        }
    }
    for (const std::string& spell_to_open : last_session_open_tabs["spells"]) {
        if (content.get_spells().contains(spell_to_open)) {
            open_content_pieces.push_back(&content.get_spells().get(spell_to_open));
        }
    }
    for (const std::string& feature_to_open : last_session_open_tabs["features"]) {
        if (content.get_features().contains(feature_to_open)) {
            open_content_pieces.push_back(content.get_features().get(feature_to_open));
        }
    }
    for (const std::string& choosable_feature_to_open : last_session_open_tabs["choosable_features"]) {
        if (content.get_choosable_features().contains(choosable_feature_to_open)) {
            open_content_pieces.push_back(&content.get_choosable_features().get(choosable_feature_to_open));
        }
    }
}
