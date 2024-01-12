#ifndef SESSION_HPP_
#define SESSION_HPP_

#include <dnd_config.hpp>

#include <array>
#include <deque>
#include <filesystem>
#include <future>
#include <memory>
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
    CAMPAIGN_SELECTION,
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

    const std::string& get_campaign_name() const;
    const std::filesystem::path& get_content_directory() const;

    std::deque<const ContentPiece*>& get_open_content_pieces();
    const ContentPiece* get_selected_content_piece();

    std::vector<std::string> get_possible_campaign_names() const;
    void retrieve_last_session_values();
    void save_session_values();

    Errors set_campaign_name(const std::string& campaign_name);
    Errors set_content_directory(const std::filesystem::path& content_directory);

    size_t get_fuzzy_search_result_count() const;
    bool too_many_fuzzy_search_results() const;
    std::vector<std::string> get_fuzzy_search_result_strings() const;
    void set_fuzzy_search(const std::string& search_query, const std::array<bool, 9>& search_options);
    void open_fuzzy_search_result(size_t index);

    std::vector<std::string> get_advanced_search_result_strings() const;
    void open_advanced_search_result(size_t index);
    void start_advanced_search();
    bool advanced_search_results_available();
    void set_advanced_search_filter(ContentFilterVariant&& filter);
    ContentFilterVariant& get_advanced_search_filter();

    bool parsing_result_available();
private:
    void start_parsing();
    void parse_content_and_initialize();
    void open_last_session();
    void open_content_piece(const ContentPiece* content_piece);

    static constexpr int max_search_results = 500;

    const char* const last_session_filename;

    SessionStatus status;

    std::filesystem::path content_directory;
    std::string campaign_name;

    std::future<void> parsing_future;
    Errors errors;
    // the object holding all the DnD content relevant for the selected campaign
    Content content;

    std::unordered_map<std::string, std::vector<std::string>> last_session_open_tabs;
    std::deque<const ContentPiece*> open_content_pieces;
    const ContentPiece* selected_content_piece;

    std::unique_ptr<FuzzyContentSearch> fuzzy_search;
    std::array<const ContentPiece*, 500> fuzzy_search_results;
    size_t fuzzy_search_result_count;
    std::array<std::string, 500> fuzzy_search_result_strings;

    AdvancedContentSearch advanced_search;

    std::vector<std::string> unknown_error_messages;
    std::vector<std::string> parsing_error_messages;
    std::vector<std::string> validation_error_messages;
};

} // namespace dnd

#endif // SESSION_HPP_
