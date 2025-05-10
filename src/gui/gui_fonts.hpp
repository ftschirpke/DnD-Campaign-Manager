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

    ImFont* get(bool want_bold, bool want_italic) const;
};

inline ImFont* GuiFonts::get(bool want_bold, bool want_italic) const {
    if (want_bold && want_italic) {
        return bold_italic;
    } else if (want_bold) {
        return bold;
    } else if (want_italic) {
        return italic;
    } else {
        return regular;
    }
}

} // namespace dnd

#endif // GUI_FONTS_HPP_
