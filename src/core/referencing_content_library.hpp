#ifndef REFERENCING_CONTENT_LIBRARY_HPP_
#define REFERENCING_CONTENT_LIBRARY_HPP_

#include <dnd_config.hpp>

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include <core/content_library.hpp>
#include <core/searching/fuzzy_search/trie.hpp>
#include <core/utils/string_manipulation.hpp>

namespace dnd {

/**
 * @brief A content library that stores pointers to the content pieces, which are stored elsewhere.
 * @tparam T the type of the content pieces being referenced
 */
template <typename T>
requires isContentPieceType<T>
class ReferencingContentLibrary : public ContentLibrary<T> {
public:
    bool contains(const std::string& name) const override;
    bool empty() const override;
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
    const std::unordered_map<std::string, const T*>& get_all() const;
    /**
     * @brief Add a pointer to a content piece to the library
     * @param content_piece the content piece to add
     * @return "true" if the pointer was added, "false" if the content piece was already referenced
     */
    bool add(const T& content_piece);
    /**
     * @brief Get the root of the fuzzy search trie
     * @return a pointer to the root of the fuzzy search trie
     */
    const TrieNode<T>* get_trie_root() const override;
private:
    void save_in_trie(const T* content_piece);

    std::unordered_map<std::string, const T*> data;
    Trie<T> trie;
};


template <typename T>
requires isContentPieceType<T>
void ReferencingContentLibrary<T>::save_in_trie(const T* content_piece) {
    std::string lower_name = dnd::string_lowercase_copy(content_piece->get_name());

    trie.insert(lower_name, content_piece);
    for (size_t i = 0; i < lower_name.size(); ++i) {
        if (lower_name[i] == ' ' || lower_name[i] == '_' || lower_name[i] == '-') {
            std::string_view after_sep(lower_name.c_str() + i + 1, lower_name.size() - i - 1);
            trie.insert(after_sep, content_piece);
        }
        if (lower_name[i] == '(') { // do not include parentheses in trie
            break;
        }
    }
}

template <typename T>
requires isContentPieceType<T>
bool ReferencingContentLibrary<T>::contains(const std::string& name) const {
    return data.contains(name);
}

template <typename T>
requires isContentPieceType<T>
bool ReferencingContentLibrary<T>::empty() const {
    return data.empty();
}

template <typename T>
requires isContentPieceType<T>
size_t ReferencingContentLibrary<T>::size() const {
    return data.size();
}

template <typename T>
requires isContentPieceType<T>
const T& ReferencingContentLibrary<T>::get(size_t index) const {
    return *std::next(data.begin(), static_cast<long>(index))->second;
}

template <typename T>
requires isContentPieceType<T>
const T& ReferencingContentLibrary<T>::get(const std::string& name) const {
    return *data.at(name);
}

template <typename T>
requires isContentPieceType<T>
const std::unordered_map<std::string, const T*>& ReferencingContentLibrary<T>::get_all() const {
    return data;
}

template <typename T>
requires isContentPieceType<T>
bool ReferencingContentLibrary<T>::add(const T& content_piece) {
    const std::string name = content_piece.get_name();
    if (contains(name)) {
        return false;
    }
    data.emplace(name, &content_piece);
    save_in_trie(&content_piece);
    return true;
}

template <typename T>
requires isContentPieceType<T>
const TrieNode<T>* ReferencingContentLibrary<T>::get_trie_root() const {
    return trie.get_root();
}

} // namespace dnd

#endif // REFERENCING_CONTENT_LIBRARY_HPP_
