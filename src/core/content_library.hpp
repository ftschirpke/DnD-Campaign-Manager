#ifndef CONTENT_LIBRARY_HPP_
#define CONTENT_LIBRARY_HPP_

#include <dnd_config.hpp>

#include <string>
#include <vector>

#include <core/models/content_piece.hpp>
#include <core/searching/fuzzy_search/trie_node.hpp>
#include <core/utils/types.hpp>

namespace dnd {

/**
 * @brief An interface for a content library manages content pieces
 * @tparam T the type of the content pieces
 */
template <typename T>
requires isContentPieceType<T>
class ContentLibrary {
public:
    virtual bool contains(const std::string& name) const = 0;
    virtual bool empty() const = 0;
    virtual size_t size() const = 0;
    /**
     * @brief Get content piece by its index
     * @param index the index of the piece of content
     * @return constant reference to the piece of content if it exists, std::nullopt otherwise
     */
    virtual OptCRef<T> get(size_t index) const = 0;
    /**
     * @brief Get content piece by its name
     * @param name the name of the piece of content
     * @return constant reference to the piece of content if it exists, std::nullopt otherwise
     */
    virtual OptCRef<T> get(const std::string& name) const = 0;
    /**
     * @brief Get the root of the fuzzy search trie
     * @return a pointer to the root of the fuzzy search trie
     */
    virtual const TrieNode<T>* get_trie_root() const = 0;
};

} // namespace dnd

#endif // CONTENT_LIBRARY_HPP_
