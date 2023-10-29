#ifndef STORAGE_CONTENT_LIBRARY_HPP_
#define STORAGE_CONTENT_LIBRARY_HPP_

#include <dnd_config.hpp>

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

#include <core/content_library.hpp>
#include <core/models/content_piece.hpp>
#include <core/searching/fuzzy_search/trie.hpp>
#include <core/utils/string_manipulation.hpp>

namespace dnd {

/**
 * @brief A content library that stores the content pieces directly.
 * @tparam T the type of the content pieces being stored
 */
template <typename T>
requires isContentPieceType<T>
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
     * @return map of all pieces of content
     */
    const std::unordered_map<std::string, T>& get_all() const;
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
    void save_in_fuzzy(const T& content_piece);

    std::unordered_map<std::string, T> data;
    Trie<T> trie;
};

template <typename T>
requires isContentPieceType<T>
void StorageContentLibrary<T>::save_in_fuzzy(const T& content_piece) {
    std::string lower_name = dnd::string_lowercase_copy(content_piece.get_name());

    trie.insert(lower_name, &content_piece);
    for (size_t i = 0; i < lower_name.size(); ++i) {
        if (lower_name[i] == ' ' || lower_name[i] == '_' || lower_name[i] == '-') {
            std::string_view after_sep(lower_name.c_str() + i + 1, lower_name.size() - i - 1);
            trie.insert(after_sep, &content_piece);
        }
        if (lower_name[i] == '(') { // do not include parentheses in fuzzy
            break;
        }
    }
}

template <typename T>
requires isContentPieceType<T>
bool StorageContentLibrary<T>::contains(const std::string& name) const {
    return data.contains(name);
}

template <typename T>
requires isContentPieceType<T>
bool StorageContentLibrary<T>::empty() const {
    return data.empty();
}

template <typename T>
requires isContentPieceType<T>
size_t StorageContentLibrary<T>::size() const {
    return data.size();
}

template <typename T>
requires isContentPieceType<T>
const T& StorageContentLibrary<T>::get(size_t index) const {
    return std::next(data.begin(), static_cast<long>(index))->second;
}

template <typename T>
requires isContentPieceType<T>
const T& StorageContentLibrary<T>::get(const std::string& name) const {
    return data.at(name);
}

template <typename T>
requires isContentPieceType<T>
const std::unordered_map<std::string, T>& StorageContentLibrary<T>::get_all() const {
    return data;
}

template <typename T>
requires isContentPieceType<T>
bool StorageContentLibrary<T>::add(T&& content_piece) {
    const std::string name = content_piece.get_name();
    if (contains(name)) {
        return false;
    }
    data.emplace(name, std::move(content_piece));
    save_in_fuzzy(data.at(name));
    return true;
}

template <typename T>
requires isContentPieceType<T>
const TrieNode<T>* StorageContentLibrary<T>::get_trie_root() const {
    return trie.get_root();
}

} // namespace dnd

#endif // STORAGE_CONTENT_LIBRARY_HPP_
