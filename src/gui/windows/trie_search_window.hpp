#ifndef TRIE_SEARCH_WINDOW_HPP_
#define TRIE_SEARCH_WINDOW_HPP_

#include <dnd_config.hpp>

#include <array>
#include <string>

#include <core/session.hpp>

namespace dnd {

/**
 * @brief A window of the GUI displaying the trie search (or simple search).
 */
class TrieSearchWindow {
public:
    TrieSearchWindow(Session& session);
    /**
     * @brief Renders the window.
     */
    void render();
private:
    Session& session;
    std::string search_query;
    std::array<bool, 9> search_options;
};

} // namespace dnd

#endif // TRIE_SEARCH_WINDOW_HPP_
