#ifndef TRIE_HPP_
#define TRIE_HPP_

#include "dnd_config.hpp"

#include <map>
#include <memory>
#include <ranges>
#include <stack>
#include <string>

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
    T* end_word;
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
     * @brief Inserts a word into the trie with the given data.
     * @param word the string to insert
     * @param data the data to associate with the end of the word
     */
    void insert(std::string_view word, T* data);
    /**
     * @brief Searches the trie for a word and returns the associated data if it exists.
     * @param word the string to search for
     * @return a pointer to the associated data if the word exists in the trie, else nullptr
     */
    T* search(std::string_view word);
    /**
     * @brief Searches the trie for all words with the given prefix and returns the associated data in order
     * @param prefix the prefix to search for
     * @return a vector of pointers to the associated data of all words with the given prefix
     */
    std::vector<T*> search_prefix(std::string_view prefix);
private:
    /**
     * @brief Finds the trie node representing the given word
     * @param word the word to find the trie node for
     * @return the trie node representing the given word if it exists, else nullptr
     */
    TrieNode<T>* find_node(std::string_view word);
    /**
     * @brief Return a list of all successors of a given node (including the node itself)
     * @param root the root node
     * @return a vector of pointers to all the successor nodes
     */
    std::vector<T*> successors(TrieNode<T>* root);

    // the root node of the trie
    std::unique_ptr<TrieNode<T>> root;
};

template <typename T>
Trie<T>::Trie() : root(std::make_unique<TrieNode<T>>(TrieNode<T>())) {}

template <typename T>
void Trie<T>::insert(std::string_view word, T* data) {
    TrieNode<T>* current_node = root.get();
    for (char c : word) {
        if (!current_node->children.contains(c)) {
            current_node->children[c] = std::make_unique<TrieNode<T>>(TrieNode<T>());
        }
        current_node = current_node->children[c].get();
    }
    current_node->end_word = data;
}

template <typename T>
inline T* Trie<T>::search(std::string_view word) {
    TrieNode<T>* word_node = find_node(word);
    if (word_node == nullptr) {
        return nullptr;
    }
    return word_node->end_word;
}

template <typename T>
std::vector<T*> Trie<T>::search_prefix(std::string_view prefix) {
    TrieNode<T>* prefix_node = find_node(prefix);
    if (prefix_node == nullptr) {
        return {};
    }
    return successors(prefix_node);
}

template <typename T>
TrieNode<T>* Trie<T>::find_node(std::string_view word) {
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
std::vector<T*> Trie<T>::successors(TrieNode<T>* root) {
    std::vector<T*> successors;
    std::stack<TrieNode<T>*> node_stack;
    node_stack.push(root);

    while (!node_stack.empty()) {
        TrieNode<T>* current_node = node_stack.top();
        node_stack.pop();

        if (current_node->end_word != nullptr) {
            successors.push_back(current_node->end_word);
        }
        for (const auto& [_, child] : current_node->children | std::views::reverse) {
            node_stack.push(child.get());
        }
    }
    return successors;
}

} // namespace dnd

#endif // TRIE_HPP_
