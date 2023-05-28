#ifndef GUI_APP_HPP_
#define GUI_APP_HPP_

#include "dnd_config.hpp"

#include <filesystem>
#include <future>
#include <string>
#include <vector>

#include <imgui/imfilebrowser.h>
#include <imgui/imgui.h>

#include "core/controllers/content_holder.hpp"
#include "core/parsing/controllers/content_parser.hpp"

namespace dnd {

/**
 * @brief Class for the main GUI application allowing content management and rendering.
 */
class GUIApp {
public:
    GUIApp();
    /**
     * @brief Renders one or multiple DearImGui windows displaying the content of the application.
     */
    void render();
    /**
     * @brief Initializes the GUI elements of the application.
     */
    void initialize_gui_elements();
private:
    void start_parsing();

    void render_content_dir_selection();
    void render_campaign_selection();
    void render_main_window();
    void render_content_window();
    void render_parsing_error_popup();

    bool show_demo_window;

    bool select_campaign;

    std::filesystem::path content_directory;
    std::string campaign_name;

    bool is_parsing;
    std::future<ContentHolder> parsed_content;
    std::vector<std::string> error_messages;

    bool show_error_popup;

    ContentParser parser;
    // the object holding all the DnD content relevant for the selected campaign
    ContentHolder content;

    // the file dialog for selecting the content directory
    ImGui::FileBrowser content_dir_dialog;
};

} // namespace dnd

#endif // GUI_APP_HPP_
