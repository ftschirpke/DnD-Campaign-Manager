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
#include <core/searching/content_search.hpp>

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

    SessionStatus get_status() const noexcept;
    Content& get_content() noexcept;
    const Errors& get_errors() const noexcept;
    const std::vector<std::string>& get_error_messages() const;
    const std::string& get_campaign_name() const noexcept;
    const std::filesystem::path& get_content_directory() const noexcept;
    std::deque<const ContentPiece*>& get_open_content_pieces() noexcept;
    size_t get_search_result_count() const noexcept;

    /**
     * @brief Gets the list of possible campaign names.
     * @return a vector containing the possible campaign names for the current content directory
     */
    std::vector<std::string> get_possible_campaign_names() const;
    /**
     * @brief Returns a list of strings representing the search results.
     * @return the list of strings representing the search results
     */
    std::vector<std::string> get_search_result_strings() const;

    /**
     * @brief Retrieves the last session values from a file, if it exists.
     */
    void retrieve_last_session_values();
    /**
     * @brief Saves the current session values to a file.
     */
    void save_session_values();
    /**
     * @brief Clears the error messages.
     */
    void clear_error_messages();
    /**
     * @brief If the provided campaign name is valid, sets the campaign name and starts parsing.
     * @param campaign_name the campaign name to set
     * @return "true" if the campaign name is valid, "false" otherwise
     */
    bool set_campaign_name(const std::string& campaign_name);
    /**
     * @brief If the provided content directory is valid, sets the content directory and starts campaign selection.
     * @param content_directory the content directory to set
     * @return "true" if the content directory is valid, "false" otherwise
     */
    bool set_content_directory(const std::filesystem::path& content_directory);
    /**
     * @brief Sets the search query to the provided string.
     * @param new_query the new search query
     */
    void set_search_query(const std::string& new_query);
    /**
     * @brief Tries to open a content piece in the search result by index.
     * @param index the index of the content piece to open
     */
    void open_search_result(size_t index);

    void update();
private:
    void start_parsing();
    void finish_parsing();
    void open_last_session();

    static constexpr int max_search_results = 100;

    const char* const last_session_filename;

    SessionStatus status;

    std::filesystem::path content_directory;
    std::string campaign_name;

    std::unordered_map<std::string, std::vector<std::string>> last_session_open_tabs;

    std::unique_ptr<ContentSearch> search;
    std::array<const ContentPiece*, 100> search_results;
    size_t search_result_count;
    std::deque<const ContentPiece*> open_content_pieces;
    std::array<std::string, 100> search_result_strings;

    std::vector<std::string> error_messages;

    std::future<ParsingResult> parsing_results;
    ContentParser parser;
    Errors errors;
    // the object holding all the DnD content relevant for the selected campaign
    Content content;
};

} // namespace dnd

#endif // SESSION_HPP_
