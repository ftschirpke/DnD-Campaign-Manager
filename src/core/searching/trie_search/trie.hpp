#ifndef TRIE_HPP_
#define TRIE_HPP_

#include <dnd_config.hpp>

#include <cassert>
#include <memory>
#include <string_view>
#include <unordered_set>
#include <vector>

#include <core/searching/trie_search/trie_node.hpp>

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
     * @brief Inserts a word into the trie with the given data (by reference).
     * @param word the string to insert (as string_view)
     * @param data the data to associate with the end of the word
     */
    void insert(std::string_view word, T& data);
    /**
     * @brief Inserts a word into the trie with the given data (as a pointer).
     * @param word the string to insert (as string_view)
     * @param data the data to associate with the end of the word
     */
    void insert(std::string_view word, T* data);
    /**
     * @brief Searches the trie for a word and returns the associated data if it exists.
     * @param word the string to search for (as string_view)
     * @return a pointer to the associated data if the word exists in the trie, else nullptr
     */
    std::vector<T*> search(std::string_view word) const;
    /**
     * @brief Searches the trie for all words with the given prefix and returns the associated data in order
     * @param prefix the prefix string to search for (as string_view)
     * @return a set of pointers to the associated data of all words with the given prefix
     */
    std::unordered_set<T*> search_prefix(std::string_view prefix) const;
    /**
     * @brief Get the root node of the trie
     * @return the root node
     */
    const TrieNode<T>* get_root() const;
private:
    /**
     * @brief Finds the trie node representing the given word
     * @param word the word to find the trie node for
     * @return the trie node representing the given word if it exists, else nullptr
     */
    TrieNode<T>* find_node(std::string_view word) const;

    // the root node of the trie
    std::unique_ptr<TrieNode<T>> root;
};

template <typename T>
Trie<T>::Trie() : root(std::make_unique<TrieNode<T>>(TrieNode<T>())) {}

template <typename T>
inline void Trie<T>::insert(std::string_view word, T& data) {
    insert(word, &data);
}

template <typename T>
void Trie<T>::insert(std::string_view word, T* data) {
    TrieNode<T>* current_node = root.get();
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
inline std::vector<T*> Trie<T>::search(std::string_view word) const {
    TrieNode<T>* word_node = find_node(word);
    if (word_node == nullptr) {
        return {};
    }
    return word_node->get_end_words();
}

template <typename T>
std::unordered_set<T*> Trie<T>::search_prefix(std::string_view prefix) const {
    TrieNode<T>* prefix_node = find_node(prefix);
    if (prefix_node == nullptr) {
        return {};
    }
    return prefix_node->successors();
}

template <typename T>
TrieNode<T>* Trie<T>::find_node(std::string_view word) const {
    TrieNode<T>* current_node = root.get();
    for (char c : word) {
        current_node = current_node->get_child(c);
        if (current_node == nullptr) {
            return nullptr;
        }
    }
    return current_node;
}

template <typename T>
inline const TrieNode<T>* Trie<T>::get_root() const {
    return root.get();
}

} // namespace dnd

#endif // TRIE_HPP_
