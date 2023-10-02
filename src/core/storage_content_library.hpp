#ifndef STORAGE_CONTENT_LIBRARY_HPP_
#define STORAGE_CONTENT_LIBRARY_HPP_

#include <dnd_config.hpp>

#include <iostream>

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <core/content_library.hpp>
#include <core/searching/trie_search/trie.hpp>
#include <core/utils/string_manipulation.hpp>

namespace dnd {

/**
 * @brief A content library that stores the content pieces directly.
 * @tparam T the type of the content pieces being stored
 */
template <typename T>
class StorageContentLibrary : public ContentLibrary<T> {
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
     * @brief Get all pieces of content
     * @return vector of all pieces of content
     */
    const std::vector<T>& get_all() const;
    /**
     * @brief Add a piece of content to the library
     * @param content_piece the piece of content to add
     * @return "true" if the piece of content was added, "false" if a piece of content with the same name already exists
     */
    bool add(T&& content_piece);
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
        void search(const std::vector<T>& sorted_data, const std::string& name_query);
    };

    void save_in_trie(const T& content_piece);

    std::vector<T> sorted_data;
    Trie<T> trie;
    mutable BinarySearch binary_search;
};

template <typename T>
void StorageContentLibrary<T>::BinarySearch::search(const std::vector<T>& sorted_data, const std::string& name_query) {
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

        if (sorted_data[middle].get_name() < name_query) {
            lower = middle + 1;
        } else if (name_query < sorted_data[middle].get_name()) {
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
void StorageContentLibrary<T>::save_in_trie(const T& content_piece) {
    std::string lower_name = dnd::string_lowercase_copy(content_piece.get_name());

    trie.insert(lower_name, &content_piece);
    for (size_t i = 0; i < lower_name.size(); ++i) {
        if (lower_name[i] == ' ' || lower_name[i] == '_' || lower_name[i] == '-') {
            std::string_view after_sep(lower_name.c_str() + i + 1, lower_name.size() - i - 1);
            trie.insert(after_sep, &content_piece);
        }
        if (lower_name[i] == '(') { // do not include parentheses in trie
            break;
        }
    }
}

template <typename T>
bool StorageContentLibrary<T>::contains(const std::string& name) const {
    binary_search.search(sorted_data, name);
    return binary_search.last_result_found;
}

template <typename T>
bool StorageContentLibrary<T>::empty() const {
    return sorted_data.empty();
}

template <typename T>
size_t StorageContentLibrary<T>::size() const {
    return sorted_data.size();
}

template <typename T>
const T& StorageContentLibrary<T>::get(size_t index) const {
    return sorted_data.at(index);
}

template <typename T>
const T& StorageContentLibrary<T>::get(const std::string& name) const {
    binary_search.search(sorted_data, name);
    if (!binary_search.last_result_found) {
        throw std::out_of_range("No content piece with name \"" + name + "\" exists.");
    }
    return sorted_data[binary_search.last_result_index];
}

template <typename T>
const std::vector<T>& StorageContentLibrary<T>::get_all() const {
    return sorted_data;
}

template <typename T>
bool StorageContentLibrary<T>::add(T&& content_piece) {
    binary_search.search(sorted_data, content_piece.get_name());
    if (binary_search.last_result_found) {
        return false;
    }
    if (binary_search.last_result_index == sorted_data.size()) {
        sorted_data.emplace_back(std::move(content_piece));
    } else {
        auto insert_it = sorted_data.cbegin() + static_cast<long>(binary_search.last_result_index);
        sorted_data.insert(insert_it, std::move(content_piece));
    }
    save_in_trie(sorted_data[binary_search.last_result_index]);
    binary_search.last_result_found = true; // allows for immediate access
    return true;
}

template <typename T>
const TrieNode<T>* StorageContentLibrary<T>::get_trie_root() const {
    return trie.get_root();
}

} // namespace dnd

#endif // STORAGE_CONTENT_LIBRARY_HPP_
