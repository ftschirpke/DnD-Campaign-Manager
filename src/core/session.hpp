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
#include <core/parsing/content_parser.hpp>
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

    /**
     * @brief Gets the current session status.
     * @return the current session status
     */
    SessionStatus get_status() const noexcept;
    /**
     * @brief Gets the current content.
     * @return the current content
     */
    Content& get_content() noexcept;
    /**
     * @brief Gets the current errors.
     * @return the current errors
     */
    const Errors& get_errors() const noexcept;
    /**
     * @brief Gets the error messages for the unknown errors.
     * @return a vector of error message strings
     */
    const std::vector<std::string>& get_unknown_error_messages() const noexcept;
    /**
     * @brief Gets the error messages for the parsing errors.
     * @return a vector of error message strings
     */
    const std::vector<std::string>& get_parsing_error_messages() const noexcept;
    /**
     * @brief Gets the error messages for the validation errors.
     * @return a vector of error message strings
     */
    const std::vector<std::string>& get_validation_error_messages() const noexcept;
    /**
     * @brief Gets the current campaign name.
     * @return the current campaign name
     */
    const std::string& get_campaign_name() const noexcept;
    /**
     * @brief Gets the current content directory.
     * @return the current content directory
     */
    const std::filesystem::path& get_content_directory() const noexcept;
    /**
     * @brief Gets the list of open content pieces.
     * @return a deque containing the open content pieces
     */
    std::deque<const ContentPiece*>& get_open_content_pieces() noexcept;
    /**
     * @brief Gets the currently selected content piece and sets it to "nullptr".
     * @return the currently selected content piece
     */
    const ContentPiece* get_selected_content_piece() noexcept;
    /**
     * @brief Gets the number of search results for the fuzzy search.
     * @return the number of search results
     */
    size_t get_fuzzy_search_result_count() const noexcept;
    /**
     * @brief Returns whether there are too many search results for the fuzzy search.
     * @return "true" if there are too many search results, "false" otherwise
     */
    bool too_many_fuzzy_search_results() const noexcept;
    /**
     * @brief Gets the list of possible campaign names.
     * @return a vector containing the possible campaign names for the current content directory
     */
    std::vector<std::string> get_possible_campaign_names() const;
    /**
     * @brief Returns a list of strings representing the search results.
     * @return the list of strings representing the search results
     */
    std::vector<std::string> get_fuzzy_search_result_strings() const;

    /**
     * @brief Refuzzyves the last session values from a file, if it exists.
     */
    void retrieve_last_session_values();
    /**
     * @brief Saves the current session values to a file.
     */
    void save_session_values();
    /**
     * @brief Clears the unknown error messages.
     */
    void clear_unknown_error_messages();
    /**
     * @brief If the provided campaign name is valid, sets the campaign name and starts parsing.
     * @param campaign_name the campaign name to set
     * @return "true" if the campaign name is valid, "false" otherwise
     */
    Errors set_campaign_name(const std::string& campaign_name);
    /**
     * @brief If the provided content directory is valid, sets the content directory and starts campaign selection.
     * @param content_directory the content directory to set
     * @return "true" if the content directory is valid, "false" otherwise
     */
    Errors set_content_directory(const std::filesystem::path& content_directory);
    /**
     * @brief Sets the search query for the fuzzy search to the provided string.
     * @param new_query the new search query
     */
    void set_fuzzy_search(const std::string& search_query, const std::array<bool, 9>& search_options);
    /**
     * @brief Fuzzys to open a content piece in the fuzzy search result by index.
     * @param index the index of the content piece to open
     */
    void open_fuzzy_search_result(size_t index);

    void update();
private:
    void start_parsing();
    void parse_content();
    void finish_parsing();
    void open_last_session();
    void open_content_piece(const ContentPiece* content_piece);

    static constexpr int max_search_results = 500;

    const char* const last_session_filename;

    SessionStatus status;

    std::filesystem::path content_directory;
    std::string campaign_name;

    std::unordered_map<std::string, std::vector<std::string>> last_session_open_tabs;
    std::deque<const ContentPiece*> open_content_pieces;
    const ContentPiece* selected_content_piece;

    std::unique_ptr<FuzzyContentSearch> fuzzy_search;
    std::array<const ContentPiece*, 500> fuzzy_search_results;
    size_t fuzzy_search_result_count;
    std::array<std::string, 500> fuzzy_search_result_strings;

    std::vector<std::string> unknown_error_messages;
    std::vector<std::string> parsing_error_messages;
    std::vector<std::string> validation_error_messages;

    std::future<void> parsing_future;
    ContentParser parser;
    Errors errors;
    // the object holding all the DnD content relevant for the selected campaign
    Content content;
};

} // namespace dnd

#endif // SESSION_HPP_
