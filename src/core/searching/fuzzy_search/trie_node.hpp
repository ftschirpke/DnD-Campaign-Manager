#ifndef TRIE_NODE_HPP_
#define TRIE_NODE_HPP_

#include <cassert>
#include <map>
#include <stack>
#include <vector>

namespace dnd {

/**
 * @brief A node of a trie data structure that stores a character and a map to its children
 * @tparam T the type of data associated with the end of a word
 */
template <typename T>
class TrieNode {
public:
    const std::map<char, TrieNode<T>>& get_children() const;
    TrieNode<T>* get_child(char c);
    const TrieNode<T>* get_child(char c) const;
    /**
     * @brief Get the data associated with the end of a word
     * @return the data
     */
    const std::vector<const T*>& get_end_words() const;
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
    void add_end_word(const T* end_word);
    /**
     * @brief Return a set of all successors of the node (including the node itself)
     * @return a set of pointers to all the successor nodes
     */
    std::vector<const T*> successors() const;
private:
    // the children of this node in the trie
    std::map<char, TrieNode<T>> children;
    // a pointer to the data associated with the end of a word
    std::vector<const T*> end_words;
};


// === IMPLEMENTATION ===

template <typename T>
const std::map<char, TrieNode<T>>& TrieNode<T>::get_children() const {
    return children;
}

template <typename T>
TrieNode<T>* TrieNode<T>::get_child(char c) {
    if (children.contains(c)) {
        return &children.at(c);
    }
    return nullptr;
}

template <typename T>
const TrieNode<T>* TrieNode<T>::get_child(char c) const {
    if (children.contains(c)) {
        return &children.at(c);
    }
    return nullptr;
}

template <typename T>
const std::vector<const T*>& TrieNode<T>::get_end_words() const {
    return end_words;
}

template <typename T>
TrieNode<T>* TrieNode<T>::create_child(char c) {
    children[c] = TrieNode<T>();
    return &children.at(c);
}

template <typename T>
void TrieNode<T>::add_end_word(const T* end_word) {
    end_words.push_back(end_word);
}

template <typename T>
std::vector<const T*> TrieNode<T>::successors() const {
    std::vector<const T*> successors;
    std::stack<const TrieNode<T>*> node_stack;
    node_stack.push(this);

    while (!node_stack.empty()) {
        const TrieNode<T>* current_node = node_stack.top();
        assert(current_node != nullptr);
        node_stack.pop();

        const std::vector<const T*>& current_end_words = current_node->get_end_words();
        if (!current_end_words.empty()) {
            successors.insert(successors.end(), current_end_words.cbegin(), current_end_words.cend());
        }
        for (auto it = current_node->get_children().crbegin(); it != current_node->get_children().crend(); ++it) {
            node_stack.push(&it->second);
        }
    }
    return successors;
}

} // namespace dnd

#endif // TRIE_NODE_HPP_
