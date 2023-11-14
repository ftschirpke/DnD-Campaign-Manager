#ifndef CONTENT_SELECTION_WINDOW_HPP_
#define CONTENT_SELECTION_WINDOW_HPP_

#include <dnd_config.hpp>

#include <imgui/imgui.h>
#include <imgui_filebrowser/imfilebrowser.h>

#include <core/session.hpp>

namespace dnd {

class ContentConfigurationWindow {
public:
    ContentConfigurationWindow(Session& session);
    void initialize();
    void open_content_directory_selection();
    void open_campaign_selection();
    void render();
private:
    void render_content_dir_selection();
    void render_campaign_selection();

    Session& session;
    ImGui::FileBrowser content_dir_dialog;
    bool is_selecting_campaign;
};

} // namespace dnd

#endif // CONTENT_SELECTION_WINDOW_HPP_
