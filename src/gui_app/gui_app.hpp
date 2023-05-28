#ifndef GUI_APP_HPP_
#define GUI_APP_HPP_

#include "dnd_config.hpp"

#include <imgui/imgui.h>

#include "core/controllers/content_holder.hpp"

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

    const ImVec4& get_clear_color() const { return clear_color; }
private:
    bool show_demo_window;
    // the object holding all the DnD content relevant for the selected campaign
    ContentHolder content;
    // the color used to clear the background of the main window
    ImVec4 clear_color;
};

} // namespace dnd

#endif // GUI_APP_HPP_
