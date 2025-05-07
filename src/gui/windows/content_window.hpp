#ifndef CONTENT_WINDOW_HPP_
#define CONTENT_WINDOW_HPP_

#include <dnd_config.hpp>

#include <core/session.hpp>
#include <gui/gui_fonts.hpp>
#include <gui/visitors/content/display_visitor.hpp>

namespace dnd {

/**
 * @brief A window of the GUI displaying the opened content pieces.
 */
class ContentWindow {
public:
    ContentWindow(Session& session, const GuiFonts& fonts);
    void render();
private:
    Session& session;
    DisplayVisitor display_visitor;
};

} // namespace dnd

#endif // CONTENT_WINDOW_HPP_
