#ifndef CONTENT_SELECTION_HPP_
#define CONTENT_SELECTION_HPP_

#include <dnd_config.hpp>

#include <imgui/imgui.h>
#include <imgui_filebrowser/imfilebrowser.h>

#include <core/session.hpp>

namespace dnd {

/**
 * @brief A window of the GUI asking the user to select a content directory and campaign.
 */
class ContentSelection {
public:
    ContentSelection(Session& session);
    /**
     * @brief Initializes values of the window.
     */
    void initialize();
    /**
     * @brief Opens the content directory selection.
     */
    void select_content_directory();
    /**
     * @brief Opens the campaign selection.
     */
    void select_campaign();
    /**
     * @brief Renders the content directory selection window.
     */
    void render();
private:
    void open_content_dir_dialog();
    void render_content_dir_selection();
    void render_campaign_selection();

    Session& session;
    ImGui::FileBrowser content_dir_dialog;
    bool is_selecting_campaign;
};

} // namespace dnd

#endif // CONTENT_SELECTION_HPP_
