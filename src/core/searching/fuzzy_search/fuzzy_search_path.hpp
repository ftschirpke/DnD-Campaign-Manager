#ifndef FUZZY_SEARCH_PATH_HPP_
#define FUZZY_SEARCH_PATH_HPP_

#include <dnd_config.hpp>

#include <stack>
#include <unordered_set>
#include <vector>

#include <core/searching/fuzzy_search/trie_node.hpp>

namespace dnd {

template <typename T>
class FuzzySearchPath : public std::stack<const TrieNode<T>*> {
public:
    /**
     * @brief Push the child of the top node onto the stack if it exists (otherwise push nullptr)
     * @param c the character to get the child for
     */
    void push_top_child(char c);
    /**
     * @brief Insert the successors of the top node into a vector if such successors exist
     * @tparam S
     * @param vector the vector to insert the successors into
     * @return "true" if successors were inserted, "false" otherwise (nothing inserted)
     */
    template <typename S>
    requires std::derived_from<T, S>
    bool insert_top_successors_into(std::vector<S*>& vec) const;
};

template <typename T>
void FuzzySearchPath<T>::push_top_child(char c) {
    if (this->top() == nullptr) {
        this->push(nullptr);
    } else {
        this->push(this->top()->get_child(c));
    }
}

template <typename T>
template <typename S>
requires std::derived_from<T, S>
bool FuzzySearchPath<T>::insert_top_successors_into(std::vector<S*>& vec) const {
    if (this->top() == nullptr) {
        return false;
    }
    std::vector<const T*> successors = this->top()->successors();
    if (successors.empty()) {
        return false;
    }
    vec.insert(vec.end(), successors.begin(), successors.end());
    return true;
}

} // namespace dnd

#endif // FUZZY_SEARCH_PATH_HPP_
