#include <dnd_config.hpp>

#include "session.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <future>
#include <string>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include <core/content.hpp>
#include <core/content_visitors/list_visitor.hpp>
#include <core/content_visitors/session_visitor.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/source_info.hpp>
#include <core/parsing/content_parser.hpp>
#include <core/searching/trie_search/trie_content_search.hpp>
#include <core/utils/char_manipulation.hpp>

dnd::Session::Session(const char* last_session_filename)
    : last_session_filename(last_session_filename), status(SessionStatus::CONTENT_DIR_SELECTION), content_directory(),
      campaign_name(), last_session_open_tabs(), trie_search(), trie_search_results(), trie_search_result_count(0),
      open_content_pieces(), trie_search_result_strings(), unknown_error_messages(), parsing_future(), parser(),
      errors(), content() {}

dnd::Session::~Session() { save_session_values(); }

dnd::SessionStatus dnd::Session::get_status() const noexcept { return status; }

dnd::Content& dnd::Session::get_content() noexcept { return content; }

const dnd::Errors& dnd::Session::get_errors() const noexcept { return errors; }

const std::vector<std::string>& dnd::Session::get_unknown_error_messages() const noexcept {
    return unknown_error_messages;
}

const std::vector<std::string>& dnd::Session::get_parsing_error_messages() const noexcept {
    return parsing_error_messages;
}

const std::vector<std::string>& dnd::Session::get_validation_error_messages() const noexcept {
    return validation_error_messages;
}

const std::string& dnd::Session::get_campaign_name() const noexcept { return campaign_name; }

const std::filesystem::path& dnd::Session::get_content_directory() const noexcept { return content_directory; }

std::deque<const dnd::ContentPiece*>& dnd::Session::get_open_content_pieces() noexcept { return open_content_pieces; }

size_t dnd::Session::get_search_result_count() const noexcept { return trie_search_result_count; }

bool dnd::Session::too_many_search_results() const noexcept { return trie_search_result_count > max_search_results; }

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

std::vector<std::string> dnd::Session::get_trie_search_result_strings() const {
    DND_MEASURE_FUNCTION();
    ListVisitor list_visitor;
    list_visitor.reserve(trie_search_result_count);
    if (trie_search_result_count > max_search_results) {
        return {};
    }
    for (size_t i = 0; i < trie_search_result_count; ++i) {
        trie_search_results[i]->accept(&list_visitor);
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

void dnd::Session::clear_unknown_error_messages() { unknown_error_messages.clear(); }

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
    explicit ContentPieceComparator(const std::string& search_query)
        : query_length(search_query.size()), lower_query(dnd::string_lowercase_copy(search_query)),
          upper_query(dnd::string_uppercase_copy(search_query)) {}

    bool starts_with_search_query(const std::string& name) const {
        size_t i = 0;
        while (i < query_length) {
            if (name[i] != lower_query[i] && name[i] != upper_query[i]) {
                return false;
            }
            ++i;
        }
        return true;
    }

    bool operator()(const dnd::ContentPiece* a, const dnd::ContentPiece* b) const {
        const std::string& a_name = a->get_name();
        const std::string& b_name = b->get_name();
        bool prioritize_a = starts_with_search_query(a_name);
        bool prioritize_b = starts_with_search_query(b_name);
        if (prioritize_a && !prioritize_b) {
            return true;
        }
        if (!prioritize_a && prioritize_b) {
            return false;
        }
        return a->get_name() < b->get_name();
    }
private:
    size_t query_length;
    std::string lower_query;
    std::string upper_query;
};

void dnd::Session::set_trie_search(const std::string& search_query, const std::array<bool, 9>& search_options) {
    DND_MEASURE_FUNCTION();
    trie_search->set_search_query(search_query);
    std::vector<const ContentPiece*> vec_search_results = trie_search->get_results(search_options);
    trie_search_result_count = vec_search_results.size();
    if (trie_search_result_count > max_search_results) {
        return;
    }
    ContentPieceComparator comparator(search_query);
    std::sort(vec_search_results.begin(), vec_search_results.end(), comparator);
    for (size_t i = 0; i < trie_search_result_count; ++i) {
        trie_search_results[i] = vec_search_results[i];
    }
}

void dnd::Session::open_trie_search_result(size_t index) {
    if (index >= trie_search_result_count) {
        return;
    }
    const ContentPiece* content_piece = trie_search_results[index];
    open_content_pieces.push_back(content_piece);
}

void dnd::Session::update() {
    DND_MEASURE_FUNCTION();
    if (status == SessionStatus::PARSING) {
        if (parsing_future.wait_for(std::chrono::microseconds(1)) == std::future_status::ready) {
            finish_parsing();
        }
    }
}

void dnd::Session::start_parsing() {
    parsing_future = std::async(std::launch::async, &Session::parse_content, this);
    status = SessionStatus::PARSING;
}

void dnd::Session::parse_content() {
    ParsingResult parsing_result = parser.parse(content_directory, campaign_name);
    content = std::move(parsing_result.content);
    errors = std::move(parsing_result.errors);
    trie_search = std::make_unique<TrieContentSearch>(content);
    for (const ParsingError& error : errors.get_parsing_errors()) {
        SourceInfo source_info(error.get_filepath());
        parsing_error_messages.emplace_back(fmt::format(
            "{} ({} - {} - {})", error.get_error_message(), source_info.get_source_group_name(),
            source_info.get_source_type_name(), source_info.get_source_name()
        ));
    }
    for (const ValidationError& error : errors.get_validation_errors()) {
        if (error.get_validation_data() == nullptr) {
            validation_error_messages.emplace_back(error.get_error_message());
        } else {
            SourceInfo source_info(error.get_validation_data()->source_path);
            validation_error_messages.emplace_back(fmt::format(
                "{} ({} - {} - {})", error.get_error_message(), source_info.get_source_group_name(),
                source_info.get_source_type_name(), source_info.get_source_name()
            ));
        }
    }
}

void dnd::Session::finish_parsing() {
    try {
        parsing_future.get();
        status = SessionStatus::READY;
    } catch (const std::exception& e) {
        unknown_error_messages.push_back(e.what());
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
