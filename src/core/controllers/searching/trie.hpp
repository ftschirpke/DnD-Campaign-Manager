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
    /**
     * @brief Get the children of this node
     * @return the map of children
     */
    const std::map<char, std::unique_ptr<TrieNode<T>>>& get_children() const;
    /**
     * @brief Get the child node for the given character if it exists
     * @param c the character to get the child node for
     * @return pointer to child node if it exists, else nullptr
     */
    TrieNode<T>* get_child(char c) const;
    /**
     * @brief Get the data associated with the end of a word
     * @return the data
     */
    const std::vector<T*>& get_end_words() const;
    /**
     * @brief Create an empty child node for the given character
     * @param c the given character
     * @return a pointer to the created node
     */
    TrieNode<T>* create_child(char c);
    /**
     * @brief Add a piece of data associated with the end of a word
     * @param end_word the data
     */
    void add_end_word(T* end_word);
    /**
     * @brief Return a set of all successors of the node (including the node itself)
     * @return a set of pointers to all the successor nodes
     */
    std::unordered_set<T*> successors() const;
private:
    // the children of this node in the trie
    std::map<char, std::unique_ptr<TrieNode<T>>> children;
    // a pointer to the data associated with the end of a word
    std::vector<T*> end_words;
};

template <typename T>
const std::map<char, std::unique_ptr<TrieNode<T>>>& TrieNode<T>::get_children() const {
    return children;
}

template <typename T>
TrieNode<T>* TrieNode<T>::get_child(char c) const {
    if (children.contains(c)) {
        return children.at(c).get();
    }
    return nullptr;
}

template <typename T>
const std::vector<T*>& TrieNode<T>::get_end_words() const {
    return end_words;
}

template <typename T>
TrieNode<T>* TrieNode<T>::create_child(char c) {
    children[c] = std::make_unique<TrieNode<T>>();
    return children.at(c).get();
}

template <typename T>
void TrieNode<T>::add_end_word(T* end_word) {
    end_words.push_back(end_word);
}


template <typename T>
std::unordered_set<T*> TrieNode<T>::successors() const {
    std::unordered_set<T*> successors;
    std::stack<const TrieNode<T>*> node_stack;
    node_stack.push(this);

    while (!node_stack.empty()) {
        const TrieNode<T>* current_node = node_stack.top();
        node_stack.pop();

        const std::vector<T*>& end_words = current_node->get_end_words();
        if (!end_words.empty()) {
            successors.insert(current_node->end_words.begin(), current_node->end_words.end());
        }
        for (auto it = current_node->get_children().crbegin(); it != current_node->get_children().crend(); ++it) {
            node_stack.push(it->second.get());
        }
    }
    return successors;
}

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
    return successors(prefix_node);
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
