#ifndef REFERENCING_CONTENT_LIBRARY_HPP_
#define REFERENCING_CONTENT_LIBRARY_HPP_

#include <dnd_config.hpp>

#include <string>
#include <unordered_map>

#include <core/content_library.hpp>
#include <core/searching/fuzzy_search/trie.hpp>
#include <core/utils/string_manipulation.hpp>
#include <core/utils/types.hpp>

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
    OptCRef<T> get(size_t index) const override;
    OptCRef<T> get(const std::string& name) const override;
    const std::unordered_map<std::string, std::reference_wrapper<const T>>& get_all() const;
    /**
     * @brief Add a content piece to a content piece to the library
     * @param content_piece the content piece to add
     * @return reference to the inserted content piece, or std::nullopt if a content piece with that name already exists
     */
    OptCRef<T> add(const T& content_piece);
    const TrieNode<T>* get_fuzzy_search_trie_root() const override;
private:
    void save_in_trie(const T* content_piece);

    std::unordered_map<std::string, std::reference_wrapper<const T>> data;
    Trie<T> trie;
};


// === IMPLEMENTATION ===

template <typename T>
requires isContentPieceType<T>
void ReferencingContentLibrary<T>::save_in_trie(const T* content_piece) {
    std::string lower_name = string_lowercase_copy(content_piece->get_name());

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
OptCRef<T> ReferencingContentLibrary<T>::get(size_t index) const {
    if (index >= data.size()) {
        return std::nullopt;
    }
    std::reference_wrapper<const T> element = std::next(data.begin(), static_cast<long>(index))->second;
    return element;
}

template <typename T>
requires isContentPieceType<T>
OptCRef<T> ReferencingContentLibrary<T>::get(const std::string& name) const {
    auto iterator = data.find(name);
    if (iterator == data.end()) {
        return std::nullopt;
    }
    return std::cref(iterator->second);
}

template <typename T>
requires isContentPieceType<T>
const std::unordered_map<std::string, std::reference_wrapper<const T>>& ReferencingContentLibrary<T>::get_all() const {
    return data;
}

template <typename T>
requires isContentPieceType<T>
OptCRef<T> ReferencingContentLibrary<T>::add(const T& content_piece) {
    const std::string name = content_piece.get_name();
    auto [it, was_inserted] = data.emplace(name, std::cref(content_piece));
    if (was_inserted) {
        save_in_trie(&it->second.get());
        return std::cref(content_piece);
    } else {
        return std::nullopt;
    }
}

template <typename T>
requires isContentPieceType<T>
const TrieNode<T>* ReferencingContentLibrary<T>::get_fuzzy_search_trie_root() const {
    return trie.get_root();
}

} // namespace dnd

#endif // REFERENCING_CONTENT_LIBRARY_HPP_
