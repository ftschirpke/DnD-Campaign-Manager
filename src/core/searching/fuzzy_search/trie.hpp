#ifndef TRIE_HPP_
#define TRIE_HPP_

#include <dnd_config.hpp>

#include <cassert>
#include <string_view>

#include <core/searching/fuzzy_search/trie_node.hpp>

namespace dnd {

/**
 * @brief A trie data structure that stores strings and associated data
 * @tparam T the type of data associated with the end of a word
 */
template <typename T>
class Trie {
public:
    /**
     * @brief Constructor for the trie
     */
    Trie();
    /**
     * @brief Inserts a word into the trie with the given data (as a pointer).
     * @param word the string to insert (as string_view)
     * @param data the data to associate with the end of the word
     */
    void insert(std::string_view word, const T* data);
    /**
     * @brief Get the root node of the trie
     * @return the root node
     */
    const TrieNode<T>* get_root() const;
private:
    // the root node of the trie
    TrieNode<T> root;
};


// === IMPLEMENTATION ===

template <typename T>
Trie<T>::Trie() : root() {}

template <typename T>
void Trie<T>::insert(std::string_view word, const T* data) {
    TrieNode<T>* current_node = &root;
    for (char c : word) {
        TrieNode<T>* child = current_node->get_child(c);
        if (child == nullptr) {
            child = current_node->create_child(c);
        }
        assert(child != nullptr);
        assert(current_node->get_child(c) == child);
        current_node = child;
    }
    current_node->add_end_word(data);
}

template <typename T>
inline const TrieNode<T>* Trie<T>::get_root() const {
    return &root;
}

} // namespace dnd

#endif // TRIE_HPP_
