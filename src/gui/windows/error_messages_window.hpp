#ifndef ERROR_MESSAGES_WINDOW_HPP_
#define ERROR_MESSAGES_WINDOW_HPP_

#include <dnd_config.hpp>

#include <core/session.hpp>

namespace dnd {

class ErrorMessagesWindow {
public:
    ErrorMessagesWindow(Session& session);
    void render();
private:
    Session& session;
};

} // namespace dnd

#endif // ERROR_MESSAGES_WINDOW_HPP_
