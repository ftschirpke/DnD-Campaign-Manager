#ifndef REFERENCING_CONTENT_LIBRARY_HPP_
#define REFERENCING_CONTENT_LIBRARY_HPP_

#include <dnd_config.hpp>

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>

#include <core/content_library.hpp>
#include <core/searching/trie_search/trie.hpp>

namespace dnd {

/**
 * @brief A content library that stores pointers to the content pieces, which are stored elsewhere.
 * @tparam T the type of the content pieces being referenced
 */
template <typename T>
class ReferencingContentLibrary : public ContentLibrary<T> {
public:
    /**
     * @brief Check whether piece of content with given name exists
     * @param name the name
     * @return "true" if such a piece of content exists, "false" otherwise
     */
    bool contains(const std::string& name) const override;
    /**
     * @brief Returns whether the content library is empty
     * @return "true" if the content library is empty, "false" otherwise
     */
    bool empty() const override;
    /**
     * @brief Get the amount of content pieces in the library
     * @return the amount
     */
    size_t size() const override;
    /**
     * @brief Get content piece by its index
     * @param index the index of the piece of content
     * @return constant reference to the piece of content if it exists
     * @throws std::out_of_range if the piece of content does not exist
     */
    const T& get(size_t index) const override;
    /**
     * @brief Get content piece by its name
     * @param name the name of the piece of content
     * @return constant reference to the piece of content if it exists
     * @throws std::out_of_range if the piece of content does not exist
     */
    const T& get(const std::string& name) const override;
    /**
     * @brief Get all pointers
     * @return vector of all pointers
     */
    const std::vector<const T*>& get_all() const;
    /**
     * @brief Add a pointer to a content piece to the library
     * @param content_piece the content piece to add
     * @return "true" if the pointer was added, "false" if the content piece was already referenced
     */
    bool add(const T& content_piece);
    /**
     * @brief Get the root of the trie
     * @return a pointer to the root of the trie
     */
    const TrieNode<T>* get_trie_root() const override;
private:
    struct BinarySearch {
        std::string last_query = "";
        bool last_result_found = false;
        size_t last_result_index = 0;
        void search(const std::vector<const T*>& sorted_data, const std::string& name_query);
    };

    std::vector<const T*> sorted_data;
    Trie<T> trie;
    mutable BinarySearch binary_search;
};

template <typename T>
void ReferencingContentLibrary<T>::BinarySearch::search(
    const std::vector<const T*>& sorted_data, const std::string& name_query
) {
    if (name_query == last_query) {
        return;
    }
    last_query = name_query;
    if (sorted_data.empty()) {
        last_result_found = false;
        last_result_index = 0;
        return;
    }
    size_t lower = 0;
    size_t upper = sorted_data.size() - 1;
    while (lower <= upper) {
        size_t middle = (lower + upper) / 2;

        if (sorted_data[middle]->get_name() < name_query) {
            lower = middle + 1;
        } else if (name_query < sorted_data[middle]->get_name()) {
            if (middle == 0) {
                break;
            }
            upper = middle - 1;
        } else {
            last_result_found = true;
            last_result_index = middle;
            return;
        }
    }
    last_result_found = false;
    last_result_index = lower;
}

template <typename T>
bool ReferencingContentLibrary<T>::contains(const std::string& name) const {
    binary_search.search(sorted_data, name);
    return binary_search.last_result_found;
}

template <typename T>
bool ReferencingContentLibrary<T>::empty() const {
    return sorted_data.empty();
}

template <typename T>
size_t ReferencingContentLibrary<T>::size() const {
    return sorted_data.size();
}

template <typename T>
const T& ReferencingContentLibrary<T>::get(size_t index) const {
    return *sorted_data.at(index);
}

template <typename T>
const T& ReferencingContentLibrary<T>::get(const std::string& name) const {
    binary_search.search(sorted_data, name);
    if (!binary_search.last_result_found) {
        throw std::out_of_range("No content piece with name \"" + name + "\" exists.");
    }
    return *sorted_data[binary_search.last_result_index];
}

template <typename T>
const std::vector<const T*>& ReferencingContentLibrary<T>::get_all() const {
    return sorted_data;
}

template <typename T>
bool ReferencingContentLibrary<T>::add(const T& content_piece) {
    binary_search.search(sorted_data, content_piece.get_name());
    if (binary_search.last_result_found) {
        return false;
    }
    if (binary_search.last_result_index == sorted_data.size()) {
        sorted_data.emplace_back(&content_piece);
    } else {
        auto insert_it = sorted_data.cbegin() + static_cast<long>(binary_search.last_result_index);
        sorted_data.insert(insert_it, &content_piece);
    }
    binary_search.last_result_found = true; // allows for immediate access
    return true;
}

template <typename T>
const TrieNode<T>* ReferencingContentLibrary<T>::get_trie_root() const {
    return trie.get_root();
}

} // namespace dnd

#endif // REFERENCING_CONTENT_LIBRARY_HPP_
