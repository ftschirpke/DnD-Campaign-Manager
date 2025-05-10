#include <dnd_config.hpp>

#include "session.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <future>
#include <set>
#include <string>
#include <utility>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/source_info.hpp>
#include <core/parsing/content_parsing.hpp>
#include <core/searching/advanced_search/advanced_content_search.hpp>
#include <core/searching/fuzzy_search/fuzzy_content_search.hpp>
#include <core/searching/search_result.hpp>
#include <core/types.hpp>
#include <core/utils/string_manipulation.hpp>
#include <core/visitors/content/collect_open_tabs_visitor.hpp>
#include <core/visitors/content/list_content_visitor.hpp>

static const char* OPEN_TABS = "open_tabs";
static const char* CONTENT_DIRECTORY = "content_directory";

namespace dnd {

Session::Session(const char* last_session_filename)
    : last_session_filename(last_session_filename), status(SessionStatus::CONTENT_DIR_SELECTION), content_directories(),
      parsing_future(), errors(), content(), last_session_open_tabs(), open_content_pieces(), selected_content_piece(),
      fuzzy_search_results(max_search_results), advanced_search(content), unknown_error_messages() {}

Session::~Session() { save_session_values(); }

SessionStatus Session::get_status() const { return status; }

Content& Session::get_content() { return content; }

const Errors& Session::get_errors() const { return errors; }

const std::vector<std::string>& Session::get_unknown_error_messages() const { return unknown_error_messages; }

const std::vector<std::string>& Session::get_parsing_error_messages() const { return parsing_error_messages; }

const std::vector<std::string>& Session::get_validation_error_messages() const { return validation_error_messages; }

const std::set<std::filesystem::path>& Session::get_content_directories() const { return content_directories; }

std::deque<const ContentPiece*>& Session::get_open_content_pieces() { return open_content_pieces; }

const ContentPiece* Session::get_selected_content_piece() {
    const ContentPiece* rv = selected_content_piece;
    selected_content_piece = nullptr;
    return rv;
}

size_t Session::get_fuzzy_search_result_count() const { return fuzzy_search_results.size(); }

bool Session::too_many_fuzzy_search_results() const { return fuzzy_search_results.size() > max_search_results; }

std::vector<std::string> Session::get_fuzzy_search_result_strings() const {
    DND_MEASURE_FUNCTION();
    ListContentVisitor list_content_visitor;
    list_content_visitor.reserve(fuzzy_search_results.size());
    if (fuzzy_search_results.size() > max_search_results) {
        return {};
    }
    for (size_t i = 0; i < fuzzy_search_results.size(); ++i) {
        fuzzy_search_results[i].content_piece_ptr->accept_visitor(list_content_visitor);
    }
    return list_content_visitor.get_list();
}

std::vector<std::string> Session::get_advanced_search_result_strings() const {
    DND_MEASURE_FUNCTION();
    ListContentVisitor list_content_visitor;
    const std::vector<const ContentPiece*>& advanced_search_results = advanced_search.get_search_results();
    list_content_visitor.reserve(advanced_search_results.size());
    for (const ContentPiece* content_piece : advanced_search_results) {
        content_piece->accept_visitor(list_content_visitor);
    }
    return list_content_visitor.get_list();
}

void Session::retrieve_last_session_values() {
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

    if (!last_session.contains(CONTENT_DIRECTORY)) {
        return;
    }
    if (last_session[CONTENT_DIRECTORY].is_string()) {
        add_content_directory(last_session[CONTENT_DIRECTORY].get<std::filesystem::path>());
    } else if (last_session[CONTENT_DIRECTORY].is_array()) {
        for (auto& entry : last_session[CONTENT_DIRECTORY]) {
            add_content_directory(entry.get<std::filesystem::path>());
        }
    }

    if (!last_session.contains(OPEN_TABS)) {
        return;
    }
    if (!last_session[OPEN_TABS].is_object()) {
        return;
    }
    last_session_open_tabs = last_session[OPEN_TABS].get<std::unordered_map<std::string, std::vector<std::string>>>();
}

void Session::save_session_values() {
    nlohmann::json last_session;
    if (!content_directories.empty()) {
        last_session[CONTENT_DIRECTORY] = content_directories;
    }

    CollectOpenTabsVisitor collect_open_tabs_visitor;
    for (const ContentPiece* open_content_piece : open_content_pieces) {
        open_content_piece->accept_visitor(collect_open_tabs_visitor);
    }
    last_session[OPEN_TABS] = collect_open_tabs_visitor.get_open_tabs();

    std::ofstream last_session_file(last_session_filename);
    last_session_file << std::setw(4) << last_session;
    last_session_file.close();
}

void Session::clear_unknown_error_messages() { unknown_error_messages.clear(); }

static Errors validate_content_directory(const std::filesystem::path& content_directory) {
    Errors errors;
    if (!std::filesystem::exists(content_directory)) {
        errors.add_runtime_error(RuntimeError::Code::INVALID_ARGUMENT, "The content directory does not exist.");
    } else if (!std::filesystem::is_directory(content_directory)) {
        errors.add_runtime_error(RuntimeError::Code::INVALID_ARGUMENT, "The content directory is not a directory.");
    }
    return errors;
}

Errors Session::add_content_directory(const std::filesystem::path& new_content_directory) {
    Errors content_dir_errors;
    if (content_directories.contains(new_content_directory)) {
        if (new_content_directory.empty()) {
            errors.add_runtime_error(RuntimeError::Code::INVALID_ARGUMENT, "The content directory is empty.");
        }
        return content_dir_errors;
    }
    content_dir_errors = validate_content_directory(new_content_directory);
    if (content_dir_errors.ok()) {
        content_directories.insert(new_content_directory);
    } else {
        status = SessionStatus::CONTENT_DIR_SELECTION;
    }
    return content_dir_errors;
}

void Session::set_fuzzy_search(const std::string& search_query, const FuzzySearchOptions& search_options) {
    DND_MEASURE_FUNCTION();
    if (search_query.size() < FUZZY_SEARCH_MINIMUM_QUERY_LENGTH) {
        fuzzy_search_results.clear();
        return;
    }
    fuzzy_search_results = fuzzy_search_content(content, search_query, search_options);
    std::sort(
        fuzzy_search_results.begin(), fuzzy_search_results.end(),
        [](const SearchResult& a, const SearchResult& b) {
            if (a.significance == b.significance) {
                return a.content_piece_ptr->get_name() < b.content_piece_ptr->get_name();
            }
            return a.significance > b.significance;
        }
    );
}

void Session::open_fuzzy_search_result(size_t index) {
    if (index >= fuzzy_search_results.size()) {
        return;
    }
    open_content_piece(fuzzy_search_results[index].content_piece_ptr);
}

void Session::open_advanced_search_result(size_t index) {
    const std::vector<const ContentPiece*>& advanced_search_results = advanced_search.get_search_results();
    if (index >= advanced_search_results.size()) {
        return;
    }
    open_content_piece(advanced_search_results[index]);
}

void Session::start_advanced_search() { advanced_search.start_searching(); }

bool Session::advanced_search_results_available() {
    try {
        return advanced_search.search_results_available();
    } catch (const std::exception& e) {
        unknown_error_messages.push_back(e.what());
        status = SessionStatus::UNKNOWN_ERROR;
        return false;
    }
}

void Session::set_advanced_search_filter(ContentFilterVariant&& filter) {
    advanced_search.set_filter(std::move(filter));
}

ContentFilterVariant& Session::get_advanced_search_filter() { return advanced_search.get_filter(); }

bool Session::parsing_result_available() {
    DND_MEASURE_FUNCTION();
    if (status == SessionStatus::PARSING
        && parsing_future.wait_for(std::chrono::nanoseconds(1)) == std::future_status::ready) {
        try {
            parsing_future.get();
            status = SessionStatus::READY;
            open_last_session();
        } catch (const std::exception& e) {
            unknown_error_messages.push_back(e.what());
            status = SessionStatus::UNKNOWN_ERROR;
        }
    }
    return status == SessionStatus::READY;
}

void Session::start_parsing() {
    if (status != SessionStatus::PARSING) {
        parsing_future = std::async(std::launch::async, &Session::parse_content_and_initialize, this);
        status = SessionStatus::PARSING;
    }
}

bool Session::directories_differ() const { return content_directories != parsed_content_directories; }

void Session::parse_content_and_initialize() {
    ParsingResult parsing_result = parse_content(content_directories);
    content = std::move(parsing_result.content);
    errors = std::move(parsing_result.errors);
    parsed_content_directories = std::move(parsing_result.content_paths);

    parsing_error_messages.clear();
    validation_error_messages.clear();

    for (const Error& error : errors.get_errors()) {
        switch (error.index()) {
            case 0: {
                const ParsingError& parsing_error = std::get<ParsingError>(error);
                parsing_error_messages.push_back(
                    fmt::format("{} ({})", parsing_error.get_error_message(), parsing_error.get_filepath().string())
                );
                break;
            }
            case 1: {
                const ValidationError& validation_error = std::get<ValidationError>(error);
                validation_error_messages.push_back(validation_error.get_error_message());
                break;
            }
            case 2: {
                const RuntimeError& runtime_error = std::get<RuntimeError>(error);
                unknown_error_messages.push_back(runtime_error.get_error_message());
                break;
            }
        }
    }
}

void Session::open_last_session() {
    for (const std::string& character_to_open : last_session_open_tabs["character"]) {
        OptCRef<Character> character = content.get_characters().get(character_to_open);
        if (character.has_value()) {
            open_content_pieces.push_back(&character.value().get());
        }
    }
    for (const std::string& class_to_open : last_session_open_tabs["class"]) {
        OptCRef<Class> cls = content.get_classes().get(class_to_open);
        if (cls.has_value()) {
            open_content_pieces.push_back(&cls.value().get());
        }
    }
    for (const std::string& subclass_to_open : last_session_open_tabs["subclass"]) {
        OptCRef<Subclass> subclass = content.get_subclasses().get(subclass_to_open);
        if (subclass.has_value()) {
            open_content_pieces.push_back(&subclass.value().get());
        }
    }
    for (const std::string& species_to_open : last_session_open_tabs["species"]) {
        OptCRef<Species> species = content.get_species().get(species_to_open);
        if (species.has_value()) {
            open_content_pieces.push_back(&species.value().get());
        }
    }
    for (const std::string& subspecies_to_open : last_session_open_tabs["subspecies"]) {
        OptCRef<Subspecies> subspecies = content.get_subspecies().get(subspecies_to_open);
        if (subspecies.has_value()) {
            open_content_pieces.push_back(&subspecies.value().get());
        }
    }
    for (const std::string& item_to_open : last_session_open_tabs["item"]) {
        OptCRef<Item> item = content.get_items().get(item_to_open);
        if (item.has_value()) {
            open_content_pieces.push_back(&item.value().get());
        }
    }
    for (const std::string& spell_to_open : last_session_open_tabs["spell"]) {
        OptCRef<Spell> spell = content.get_spells().get(spell_to_open);
        if (spell.has_value()) {
            open_content_pieces.push_back(&spell.value().get());
        }
    }
    for (const std::string& feature_to_open : last_session_open_tabs["feature"]) {
        OptCRef<Feature> feature = content.get_features().get(feature_to_open);
        if (feature.has_value()) {
            open_content_pieces.push_back(&feature.value().get());
        }
    }
    for (const std::string& class_feature_to_open : last_session_open_tabs["class_feature"]) {
        OptCRef<ClassFeature> class_feature = content.get_class_features().get(class_feature_to_open);
        if (class_feature.has_value()) {
            open_content_pieces.push_back(&class_feature.value().get());
        }
    }
    for (const std::string& choosable_to_open : last_session_open_tabs["choosable"]) {
        OptCRef<Choosable> choosable = content.get_choosables().get(choosable_to_open);
        if (choosable.has_value()) {
            open_content_pieces.push_back(&choosable.value().get());
        }
    }
}

void Session::open_content_piece(const ContentPiece* content_piece) {
    if (std::find(open_content_pieces.begin(), open_content_pieces.end(), content_piece) == open_content_pieces.end()) {
        open_content_pieces.push_back(content_piece);
    }
    selected_content_piece = content_piece;
}

} // namespace dnd
