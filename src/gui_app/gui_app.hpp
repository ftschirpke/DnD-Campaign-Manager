#ifndef GUI_APP_HPP_
#define GUI_APP_HPP_

#include <dnd_config.hpp>

#include <array>
#include <deque>
#include <filesystem>
#include <future>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <imgui/imgui.h>
#include <imgui_filebrowser/imfilebrowser.h>

#include <core/content.hpp>
#include <core/models/content_piece.hpp>
#include <core/searching/content_search.hpp>
#include <core/session.hpp>
#include <gui_app/content_visitors/display_visitor.hpp>

namespace dnd {

/**
 * @brief Class for the main GUI application allowing content management and rendering.
 */
class GUIApp {
public:
    GUIApp();
    /**
     * @brief Initializes the GUI elements of the application.
     */
    void initialize();
    /**
     * @brief Renders one or multiple DearImGui windows displaying the content of the application.
     */
    void render();
private:
    void open_content_dir_dialog();
    void render_content_dir_selection();
    void render_campaign_selection();
    void render_overview_window();
    void render_search_window();
    void render_parsing_error_popup();
    void render_content_window();

    static constexpr int max_search_results = 100;

    bool show_demo_window;
    bool select_campaign;

    Session session;

    // the file dialog for selecting the content directory
    ImGui::FileBrowser content_dir_dialog;

    std::unordered_map<std::string, std::vector<std::string>> last_session_open_tabs;
    std::string search_query;

    const ContentPiece* forced_next_selection;

    DisplayVisitor display_visitor;
};

} // namespace dnd

#endif // GUI_APP_HPP_
