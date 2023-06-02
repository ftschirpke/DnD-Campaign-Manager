#ifndef GUI_APP_HPP_
#define GUI_APP_HPP_

#include "dnd_config.hpp"

#include <array>
#include <deque>
#include <filesystem>
#include <future>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <imgui/imfilebrowser.h>
#include <imgui/imgui.h>

#include "core/controllers/content_holder.hpp"
#include "core/controllers/searching/content_search.hpp"
#include "core/parsing/controllers/content_parser.hpp"
#include "gui_app/content_visitors/display_visitor.hpp"

namespace dnd {

/**
 * @brief Class for the main GUI application allowing content management and rendering.
 */
class GUIApp {
public:
    GUIApp();
    ~GUIApp();
    /**
     * @brief Renders one or multiple DearImGui windows displaying the content of the application.
     */
    void render();
    /**
     * @brief Initializes the GUI elements of the application.
     */
    void initialize();
    /**
     * @brief Cleans up the GUI elements of the application.
     */
    void clean_up();
private:
    void start_parsing();
    void finish_parsing();

    void save_session_values();
    void get_last_session_values();
    void open_last_session_tabs();

    void render_content_dir_selection();
    void render_campaign_selection();
    void render_overview_window();
    void render_search_window();
    void render_status_window();
    void render_parsing_error_popup();
    void render_content_window();

    static constexpr int max_search_results = 100;

    bool show_demo_window;
    bool select_campaign;
    bool is_parsing;

    // the file dialog for selecting the content directory
    ImGui::FileBrowser content_dir_dialog;

    std::filesystem::path content_directory;
    std::string campaign_name;

    std::unordered_map<std::string, std::vector<std::string>> last_session_open_tabs;

    std::string search_query;

    std::unique_ptr<ContentSearch> search;
    std::array<const ContentPiece*, 100> search_results;
    size_t search_result_count;
    std::deque<const ContentPiece*> open_content_pieces;
    std::array<std::string, 100> search_result_strings;

    std::vector<std::string> error_messages;

    std::future<ContentHolder> parsed_content;
    ContentParser parser;
    // the object holding all the DnD content relevant for the selected campaign
    ContentHolder content;

    DisplayVisitor display_visitor;
};

} // namespace dnd

#endif // GUI_APP_HPP_
