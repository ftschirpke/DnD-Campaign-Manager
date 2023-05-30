#ifndef TRIE_HPP_
#define TRIE_HPP_

#include "dnd_config.hpp"

#include <map>
#include <memory>
#include <ranges>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

namespace dnd {

/**
 * @brief A node of a trie data structure that stores a character and a map to its children
 * @tparam T the type of data associated with the end of a word
 */
template <typename T>
class TrieNode {
public:
    // the children of this node in the trie
    std::map<char, std::unique_ptr<TrieNode<T>>> children;
    // a pointer to the data associated with the end of a word
    std::vector<T*> end_words;
};

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
     * @param word the string to insert
     * @param data the data to associate with the end of the word
     */
    void insert(std::string_view word, T& data);
    /**
     * @brief Inserts a word into the trie with the given data (as a pointer).
     * @param word the string to insert
     * @param data the data to associate with the end of the word
     */
    void insert(std::string_view word, T* data);
    /**
     * @brief Searches the trie for a word and returns the associated data if it exists.
     * @param word the string to search for
     * @return a pointer to the associated data if the word exists in the trie, else nullptr
     */
    std::vector<T*> search(std::string_view word) const;
    /**
     * @brief Searches the trie for all words with the given prefix and returns the associated data in order
     * @param prefix the prefix to search for
     * @return a set of pointers to the associated data of all words with the given prefix
     */
    std::unordered_set<T*> search_prefix(std::string_view prefix) const;
private:
    /**
     * @brief Finds the trie node representing the given word
     * @param word the word to find the trie node for
     * @return the trie node representing the given word if it exists, else nullptr
     */
    TrieNode<T>* find_node(std::string_view word) const;
    /**
     * @brief Return a list of all successors of a given node (including the node itself)
     * @param parent the parent or root node
     * @return a set of pointers to all the successor nodes
     */
    std::unordered_set<T*> successors(TrieNode<T>* parent) const;

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
        if (!current_node->children.contains(c)) {
            current_node->children[c] = std::make_unique<TrieNode<T>>(TrieNode<T>());
        }
        current_node = current_node->children[c].get();
    }
    current_node->end_words.push_back(data);
}

template <typename T>
inline std::vector<T*> Trie<T>::search(std::string_view word) const {
    TrieNode<T>* word_node = find_node(word);
    if (word_node == nullptr) {
        return {};
    }
    return word_node->end_words;
}

template <typename T>
std::unordered_set<T*> Trie<T>::search_prefix(std::string_view prefix) const {
    TrieNode<T>* prefix_node = find_node(prefix);
    if (prefix_node == nullptr) {
        return {};
    }
    return successors(prefix_node);
}

template <typename T>
TrieNode<T>* Trie<T>::find_node(std::string_view word) const {
    TrieNode<T>* current_node = root.get();
    for (char c : word) {
        if (!current_node->children.contains(c)) {
            return nullptr;
        }
        current_node = current_node->children[c].get();
    }
    return current_node;
}

template <typename T>
std::unordered_set<T*> Trie<T>::successors(TrieNode<T>* parent) const {
    std::unordered_set<T*> successors;
    std::stack<TrieNode<T>*> node_stack;
    node_stack.push(parent);

    while (!node_stack.empty()) {
        TrieNode<T>* current_node = node_stack.top();
        node_stack.pop();

        if (!current_node->end_words.empty()) {
            successors.insert(current_node->end_words.begin(), current_node->end_words.end());
        }
        for (auto it = current_node->children.rbegin(); it != current_node->children.rend(); ++it) {
            node_stack.push(it->second.get());
        }
    }
    return successors;
}

} // namespace dnd

#endif // TRIE_HPP_
