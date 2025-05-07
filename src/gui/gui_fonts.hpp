#ifndef GUI_FONTS_HPP_
#define GUI_FONTS_HPP_

#include <dnd_config.hpp>

#include <imgui.h>

namespace dnd {

struct GuiFonts {
    ImFont* regular;
    ImFont* bold;
    ImFont* italic;
    ImFont* bold_italic;
};

} // namespace dnd

#endif // GUI_FONTS_HPP_
