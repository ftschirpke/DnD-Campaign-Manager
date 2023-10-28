#ifndef ADVANCED_SEARCH_WINDOW_HPP_
#define ADVANCED_SEARCH_WINDOW_HPP_

#include <dnd_config.hpp>

#include <core/session.hpp>

namespace dnd {

/**
 * @brief A class representing a window for advanced searching.
 */
class AdvancedSearchWindow {
public:
    AdvancedSearchWindow(Session& session);
    /**
     * @brief Renders the window.
     */
    void render();
private:
    Session& session;
};

} // namespace dnd

#endif // ADVANCED_SEARCH_WINDOW_HPP_
