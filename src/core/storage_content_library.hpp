#ifndef STORAGE_CONTENT_LIBRARY_HPP_
#define STORAGE_CONTENT_LIBRARY_HPP_

#include <dnd_config.hpp>

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <core/content_library.hpp>
#include <core/errors/errors.hpp>
#include <core/models/content_piece.hpp>
#include <core/searching/fuzzy_search/trie.hpp>
#include <core/utils/data_result.hpp>
#include <core/utils/string_manipulation.hpp>
#include <core/utils/types.hpp>

namespace dnd {

/**
 * @brief A content library that stores the content pieces directly.
 * @tparam T the type of the content pieces being stored
 */
template <typename T>
requires isContentPieceType<T>
class StorageContentLibrary : public ContentLibrary<T> {
public:
    bool contains(const std::string& name) const override;
    bool empty() const override;
    size_t size() const override;
    OptCRef<T> get(size_t index) const override;
    OptCRef<T> get(const std::string& name) const override;
    const std::unordered_map<std::string, T>& get_all() const;
    const std::vector<std::pair<typename T::Data, Errors>>& get_drafts() const;
    OptCRef<T> add(T&& content_piece);
    void add_draft(std::pair<typename T::Data, Errors>&& draft);
    void add_draft(typename T::Data&& draft_data, Errors&& draft_errors);
    OptCRef<T> add_result(CreateResult<T>&& content_piece_result);
    /**
     * @brief Get the root of the trie
     * @return a pointer to the root of the trie
     */
    const TrieNode<T>* get_trie_root() const override;
private:
    void save_in_trie(const T& content_piece);

    std::unordered_map<std::string, T> data;
    Trie<T> trie;
    std::vector<std::pair<typename T::Data, Errors>> drafts;
};

template <typename T>
requires isContentPieceType<T>
void StorageContentLibrary<T>::save_in_trie(const T& content_piece) {
    std::string lower_name = string_lowercase_copy(content_piece.get_name());

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
OptCRef<T> StorageContentLibrary<T>::get(size_t index) const {
    if (index >= data.size()) {
        return std::nullopt;
    }
    const T& element = std::next(data.begin(), static_cast<long>(index))->second;
    return std::cref(element);
}

template <typename T>
requires isContentPieceType<T>
OptCRef<T> StorageContentLibrary<T>::get(const std::string& name) const {
    auto iterator = data.find(name);
    if (iterator == data.end()) {
        return std::nullopt;
    }
    return std::cref(iterator->second);
}

template <typename T>
requires isContentPieceType<T>
const std::unordered_map<std::string, T>& StorageContentLibrary<T>::get_all() const {
    return data;
}

template <typename T>
requires isContentPieceType<T>
const std::vector<std::pair<typename T::Data, Errors>>& StorageContentLibrary<T>::get_drafts() const {
    return drafts;
}

template <typename T>
requires isContentPieceType<T>
OptCRef<T> StorageContentLibrary<T>::add(T&& content_piece) {
    const std::string name = content_piece.get_name();
    auto [it, was_inserted] = data.emplace(name, std::move(content_piece));
    if (was_inserted) {
        save_in_trie(it->second);
        return std::cref(it->second);
    } else {
        return std::nullopt;
    }
}

template <typename T>
requires isContentPieceType<T>
OptCRef<T> StorageContentLibrary<T>::add_result(CreateResult<T>&& content_piece_result) {
    if (content_piece_result.is_valid()) {
        return add(std::move(content_piece_result.value()));
    } else {
        add_draft(std::move(content_piece_result.data_and_errors()));
        return std::nullopt;
    }
}

template <typename T>
requires isContentPieceType<T>
void StorageContentLibrary<T>::add_draft(std::pair<typename T::Data, Errors>&& draft) {
    drafts.push_back(std::move(draft));
}

template <typename T>
requires isContentPieceType<T>
void StorageContentLibrary<T>::add_draft(typename T::Data&& draft_data, Errors&& draft_errors) {
    drafts.emplace_back(std::move(draft_data), std::move(draft_errors));
}

template <typename T>
requires isContentPieceType<T>
const TrieNode<T>* StorageContentLibrary<T>::get_trie_root() const {
    return trie.get_root();
}

} // namespace dnd

#endif // STORAGE_CONTENT_LIBRARY_HPP_
