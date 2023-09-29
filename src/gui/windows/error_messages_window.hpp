#ifndef ERROR_MESSAGES_WINDOW_HPP_
#define ERROR_MESSAGES_WINDOW_HPP_

#include <dnd_config.hpp>

#include <imgui/imgui.h>

#include <core/session.hpp>

namespace dnd {

/**
 * @brief A window of the GUI showing error messages.
 */
class ErrorMessagesWindow {
public:
    ErrorMessagesWindow(Session& session);
    /**
     * @brief Renders the error messages window.
     */
    void render();
private:
    Session& session;
};

} // namespace dnd

#endif // ERROR_MESSAGES_WINDOW_HPP_
