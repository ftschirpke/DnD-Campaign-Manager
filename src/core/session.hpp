#ifndef SESSION_HPP_
#define SESSION_HPP_

#include <dnd_config.hpp>

#include <deque>
#include <filesystem>
#include <future>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/models/content_piece.hpp>
#include <core/parsing/content_parsing.hpp>
#include <core/searching/advanced_search/advanced_content_search.hpp>
#include <core/searching/fuzzy_search/fuzzy_content_search.hpp>

namespace dnd {

enum class SessionStatus {
    CONTENT_DIR_SELECTION,
    PARSING,
    READY,
    UNKNOWN_ERROR,
};

class Session {
public:
    Session(const char* last_session_filename = "last_session.ini");
    ~Session();
    SessionStatus get_status() const;
    Content& get_content();

    const Errors& get_errors() const;
    const std::vector<std::string>& get_unknown_error_messages() const;
    const std::vector<std::string>& get_parsing_error_messages() const;
    const std::vector<std::string>& get_validation_error_messages() const;
    void clear_unknown_error_messages();

    const std::set<std::filesystem::path>& get_content_directories() const;

    std::deque<Id>& get_open_content_pieces();
    Opt<Id> get_selected_content_piece();

    void retrieve_last_session_values();
    void save_session_values();

    Errors add_content_directory(const std::filesystem::path& content_directory);

    size_t get_fuzzy_search_result_count() const;
    bool too_many_fuzzy_search_results() const;
    std::vector<std::string> get_fuzzy_search_result_strings() const;
    void set_fuzzy_search(const std::string& search_query, const FuzzySearchOptions& search_options);
    void open_fuzzy_search_result(size_t index);

    std::vector<std::string> get_advanced_search_result_strings() const;
    void open_advanced_search_result(size_t index);
    void start_advanced_search();
    bool advanced_search_results_available();
    void set_advanced_search_filter(ContentFilterVariant&& filter);
    ContentFilterVariant& get_advanced_search_filter();

    bool parsing_result_available();

    void start_parsing();
    bool directories_differ() const;
private:
    void parse_content_and_initialize();
    void open_last_session();
    void open_content_piece(Id content_piece);

    static constexpr int max_search_results = 1000;

    const char* const last_session_filename;

    SessionStatus status;

    std::set<std::filesystem::path> content_directories;

    std::future<void> parsing_future;
    Errors errors;
    // the object holding all selected DnD content
    Content content;
    std::set<std::filesystem::path> parsed_content_directories;

    std::unordered_map<std::string, std::vector<std::string>> last_session_open_tabs;
    std::deque<Id> open_content_pieces;
    Opt<Id> selected_content_piece;

    std::vector<SearchResult> fuzzy_search_results;

    AdvancedContentSearch advanced_search;

    std::vector<std::string> unknown_error_messages;
    std::vector<std::string> parsing_error_messages;
    std::vector<std::string> validation_error_messages;
};

} // namespace dnd

#endif // SESSION_HPP_
