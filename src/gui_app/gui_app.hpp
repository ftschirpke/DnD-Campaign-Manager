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
 * @brief Struct for storing the result of a search query. (transitional solution - TODO)
 */
struct SearchResult {
    std::vector<const dnd::Spell*> spells;
    std::vector<const dnd::Item*> items;
    std::vector<const dnd::Feature*> features;
};

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
    void initialize();
    /**
     * @brief Cleans up the GUI elements of the application.
     */
    void clean_up();
private:
    void start_parsing();

    void save_session_values();
    void get_last_session_values();

    void render_content_dir_selection();
    void render_campaign_selection();
    void render_overview_window();
    void render_search_window();
    void render_status_window();
    void render_parsing_error_popup();
    void render_content_window();

    void render_spell_tab(const Spell* spell_ptr, size_t index);
    void render_item_tab(const Item* item_ptr, size_t index);
    void render_feature_tab(const Feature* feature_ptr, size_t index);

    ImGuiIO& io;

    bool show_demo_window;
    bool select_campaign;
    bool is_parsing;
    bool show_error_popup;

    std::filesystem::path content_directory;
    std::string campaign_name;

    std::string search_query;
    SearchResult search_result;
    std::array<bool, 100> selected_search_results;
    static const int max_search_results = 100;

    std::vector<std::string> error_messages;

    std::future<ContentHolder> parsed_content;
    ContentParser parser;
    // the object holding all the DnD content relevant for the selected campaign
    ContentHolder content;

    // the file dialog for selecting the content directory
    ImGui::FileBrowser content_dir_dialog;
};

} // namespace dnd

#endif // GUI_APP_HPP_
