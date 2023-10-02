#ifndef CLI_APP_HPP_
#define CLI_APP_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>

#include <cli/output/command_line_output.hpp>
#include <cli/visitors/content/display_visitor.hpp>
#include <core/session.hpp>

namespace dnd {

class CliApp {
public:
    CliApp();
    /**
     * @brief Initializes the application.
     */
    void initialize(const std::filesystem::path& content_directory, const std::string& campaign_name, bool testrun);
    /**
     * @brief Starts the application.
     */
    void start();
private:
    void search_content_by_name();
    void list_all_content_of_a_type();
    void view_open_content_pieces();
    void display_content_piece(const ContentPiece* content_piece);

    Session session;
    CommandLineOutput output;
    DisplayVisitor display_visitor;
};

} // namespace dnd

#endif // CLI_APP_HPP_
