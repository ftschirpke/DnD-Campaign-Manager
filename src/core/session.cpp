#include <dnd_config.hpp>

#include "core/searching/advanced_search/advanced_content_search.hpp"
#include "core/utils/types.hpp"
#include "session.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <future>
#include <string>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/source_info.hpp>
#include <core/parsing/content_parsing.hpp>
#include <core/searching/fuzzy_search/fuzzy_content_search.hpp>
#include <core/utils/string_manipulation.hpp>
#include <core/visitors/content/collect_open_tabs_visitor.hpp>
#include <core/visitors/content/list_content_visitor.hpp>

dnd::Session::Session(const char* last_session_filename)
    : last_session_filename(last_session_filename), status(SessionStatus::CONTENT_DIR_SELECTION), content_directory(),
      campaign_name(), parsing_future(), errors(), content(), last_session_open_tabs(), open_content_pieces(),
      selected_content_piece(), fuzzy_search(), fuzzy_search_results(), fuzzy_search_result_count(0),
      fuzzy_search_result_strings(), advanced_search(content), unknown_error_messages() {}

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

const dnd::ContentPiece* dnd::Session::get_selected_content_piece() noexcept {
    const ContentPiece* rv = selected_content_piece;
    selected_content_piece = nullptr;
    return rv;
}

size_t dnd::Session::get_fuzzy_search_result_count() const noexcept { return fuzzy_search_result_count; }

bool dnd::Session::too_many_fuzzy_search_results() const noexcept {
    return fuzzy_search_result_count > max_search_results;
}

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

std::vector<std::string> dnd::Session::get_fuzzy_search_result_strings() const {
    DND_MEASURE_FUNCTION();
    ListContentVisitor list_content_visitor;
    list_content_visitor.reserve(fuzzy_search_result_count);
    if (fuzzy_search_result_count > max_search_results) {
        return {};
    }
    for (size_t i = 0; i < fuzzy_search_result_count; ++i) {
        fuzzy_search_results[i]->accept_visitor(list_content_visitor);
    }
    return list_content_visitor.get_list();
}

std::vector<std::string> dnd::Session::get_advanced_search_result_strings() const {
    DND_MEASURE_FUNCTION();
    ListContentVisitor list_content_visitor;
    const std::vector<const ContentPiece*>& advanced_search_results = advanced_search.get_search_results();
    list_content_visitor.reserve(advanced_search_results.size());
    for (const ContentPiece* content_piece : advanced_search_results) {
        content_piece->accept_visitor(list_content_visitor);
    }
    return list_content_visitor.get_list();
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

    CollectOpenTabsVisitor collect_open_tabs_visitor;
    for (const ContentPiece* open_content_piece : open_content_pieces) {
        open_content_piece->accept_visitor(collect_open_tabs_visitor);
    }
    last_session["open_tabs"] = collect_open_tabs_visitor.get_open_tabs();

    std::ofstream last_session_file(last_session_filename);
    last_session_file << std::setw(4) << last_session;
    last_session_file.close();
}

void dnd::Session::clear_unknown_error_messages() { unknown_error_messages.clear(); }

dnd::Errors dnd::Session::set_campaign_name(const std::string& new_campaign_name) {
    Errors campaign_errors;
    if (campaign_name == new_campaign_name) {
        if (campaign_name.empty()) {
            campaign_errors.add_validation_error(
                dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, nullptr, "The campaign name is empty."
            );
        }
        return campaign_errors;
    }
    std::vector<std::string> possible_names = get_possible_campaign_names();
    if (std::find(possible_names.begin(), possible_names.end(), new_campaign_name) == possible_names.end()) {
        campaign_errors.add_validation_error(
            dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, nullptr,
            fmt::format("The campaign name \"{}\" is not valid.", new_campaign_name)
        );
        return campaign_errors;
    }
    campaign_name = new_campaign_name;
    status = SessionStatus::PARSING;
    start_parsing();
    return campaign_errors;
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

dnd::Errors dnd::Session::set_content_directory(const std::filesystem::path& new_content_directory) {
    Errors content_dir_errors;
    if (content_directory == new_content_directory) {
        if (content_directory.empty()) {
            content_dir_errors.add_validation_error(
                dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, nullptr, "The content directory is empty."
            );
        }
        return content_dir_errors;
    }
    content_dir_errors = validate_content_directory(new_content_directory);
    if (content_dir_errors.ok()) {
        content_directory = new_content_directory;
        campaign_name.clear();
        status = SessionStatus::CAMPAIGN_SELECTION;
    } else {
        status = SessionStatus::CONTENT_DIR_SELECTION;
    }
    return content_dir_errors;
}

// A comparator for content pieces that sorts them by name and prioritizes those whose name starts with a
// givencharacter
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
            if (++i >= name.size()) {
                return false;
            }
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

void dnd::Session::set_fuzzy_search(const std::string& search_query, const std::array<bool, 9>& search_options) {
    DND_MEASURE_FUNCTION();
    fuzzy_search->set_search_query(search_query);
    std::vector<const ContentPiece*> vec_search_results = fuzzy_search->get_results(search_options);
    fuzzy_search_result_count = vec_search_results.size();
    if (fuzzy_search_result_count > max_search_results) {
        return;
    }
    ContentPieceComparator comparator(search_query);
    std::sort(vec_search_results.begin(), vec_search_results.end(), comparator);
    for (size_t i = 0; i < fuzzy_search_result_count; ++i) {
        fuzzy_search_results[i] = vec_search_results[i];
    }
}

void dnd::Session::open_fuzzy_search_result(size_t index) {
    if (index >= fuzzy_search_result_count) {
        return;
    }
    open_content_piece(fuzzy_search_results[index]);
}

void dnd::Session::open_advanced_search_result(size_t index) {
    const std::vector<const ContentPiece*>& advanced_search_results = advanced_search.get_search_results();
    if (index >= advanced_search_results.size()) {
        return;
    }
    open_content_piece(advanced_search_results[index]);
}

void dnd::Session::start_advanced_search() { advanced_search.start_searching(); }

bool dnd::Session::advanced_search_results_available() {
    try {
        return advanced_search.search_results_available();
    } catch (const std::exception& e) {
        unknown_error_messages.push_back(e.what());
        status = SessionStatus::UNKNOWN_ERROR;
        return false;
    }
}

void dnd::Session::set_advanced_search_filter(ContentFilterVariant&& filter) {
    advanced_search.set_filter(std::move(filter));
}

dnd::ContentFilterVariant& dnd::Session::get_advanced_search_filter() { return advanced_search.get_filter(); }

bool dnd::Session::parsing_result_available() {
    DND_MEASURE_FUNCTION();
    if (status == SessionStatus::PARSING
        && parsing_future.wait_for(std::chrono::nanoseconds(1)) == std::future_status::ready) {
        try {
            parsing_future.get();
            status = SessionStatus::READY;
        } catch (const std::exception& e) {
            unknown_error_messages.push_back(e.what());
            status = SessionStatus::UNKNOWN_ERROR;
        }
    }
    return status == SessionStatus::READY;
}

void dnd::Session::start_parsing() {
    parsing_future = std::async(std::launch::async, &Session::parse_content_and_initialize, this);
    status = SessionStatus::PARSING;
}

void dnd::Session::parse_content_and_initialize() {
    ParsingResult parsing_result = parse_content(content_directory, campaign_name);
    content = std::move(parsing_result.content);
    errors = std::move(parsing_result.errors);
    fuzzy_search = std::make_unique<FuzzyContentSearch>(content);
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

void dnd::Session::open_last_session() {
    for (const std::string& character_to_open : last_session_open_tabs["characters"]) {
        OptCRef<Character> character = content.get_characters().get(character_to_open);
        if (character.has_value()) {
            open_content_pieces.push_back(&character.value().get());
        }
    }
    for (const std::string& character_class_to_open : last_session_open_tabs["character_classes"]) {
        OptCRef<CharacterClass> character_class = content.get_character_classes().get(character_class_to_open);
        if (character_class.has_value()) {
            open_content_pieces.push_back(&character_class.value().get());
        }
    }
    for (const std::string& character_subclass_to_open : last_session_open_tabs["character_subclasses"]) {
        OptCRef<CharacterSubclass> character_subclass = content.get_character_subclasses().get(
            character_subclass_to_open
        );
        if (character_subclass.has_value()) {
            open_content_pieces.push_back(&character_subclass.value().get());
        }
    }
    for (const std::string& character_race_to_open : last_session_open_tabs["character_races"]) {
        OptCRef<CharacterRace> character_race = content.get_character_races().get(character_race_to_open);
        if (character_race.has_value()) {
            open_content_pieces.push_back(&character_race.value().get());
        }
    }
    for (const std::string& character_subrace_to_open : last_session_open_tabs["character_subraces"]) {
        OptCRef<CharacterSubrace> character_subrace = content.get_character_subraces().get(character_subrace_to_open);
        if (character_subrace.has_value()) {
            open_content_pieces.push_back(&character_subrace.value().get());
        }
    }
    for (const std::string& item_to_open : last_session_open_tabs["items"]) {
        OptCRef<Item> item = content.get_items().get(item_to_open);
        if (item.has_value()) {
            open_content_pieces.push_back(&item.value().get());
        }
    }
    for (const std::string& spell_to_open : last_session_open_tabs["spells"]) {
        OptCRef<Spell> spell = content.get_spells().get(spell_to_open);
        if (spell.has_value()) {
            open_content_pieces.push_back(&spell.value().get());
        }
    }
    for (const std::string& feature_to_open : last_session_open_tabs["features"]) {
        OptCRef<Feature> feature = content.get_features().get(feature_to_open);
        if (feature.has_value()) {
            open_content_pieces.push_back(&feature.value().get());
        }
    }
    for (const std::string& class_feature_to_open : last_session_open_tabs["class_features"]) {
        OptCRef<ClassFeature> class_feature = content.get_class_features().get(class_feature_to_open);
        if (class_feature.has_value()) {
            open_content_pieces.push_back(&class_feature.value().get());
        }
    }
    for (const std::string& choosable_to_open : last_session_open_tabs["choosables"]) {
        OptCRef<Choosable> choosable = content.get_choosables().get(choosable_to_open);
        if (choosable.has_value()) {
            open_content_pieces.push_back(&choosable.value().get());
        }
    }
}

void dnd::Session::open_content_piece(const ContentPiece* content_piece) {
    if (std::find(open_content_pieces.begin(), open_content_pieces.end(), content_piece) == open_content_pieces.end()) {
        open_content_pieces.push_back(content_piece);
    }
    selected_content_piece = content_piece;
}
