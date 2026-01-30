#ifndef STORAGE_CONTENT_LIBRARY_HPP_
#define STORAGE_CONTENT_LIBRARY_HPP_

#include <dnd_config.hpp>

#include <algorithm>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <fmt/format.h>

#include <core/content_library.hpp>
#include <core/data_result.hpp>
#include <core/errors/errors.hpp>
#include <core/models/content_piece.hpp>
#include <core/types.hpp>
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
    std::optional<size_t> find(const std::string& key) const;
    bool contains(const std::string& key) const override final;
    bool empty() const override final;
    size_t size() const override final;
    Opt<CRef<T>> get(size_t index) const override final;
    Opt<CRef<T>> get(const std::string& key) const override final;
    const std::vector<T>& get_all() const;
    const std::vector<std::pair<typename T::Data, Errors>>& get_drafts() const;
    /**
     * @brief Add a content piece to a content piece to the library
     * @param content_piece the content piece to add
     * @return reference to the inserted content piece, or std::nullopt if a content piece with that key already exists
     */
    std::optional<size_t> add(T&& content_piece);
    void add_draft(std::pair<typename T::Data, Errors>&& draft);
    void add_draft(typename T::Data&& draft_data, Errors&& draft_errors);
private:
    std::vector<T> data;
    std::vector<std::pair<typename T::Data, Errors>> drafts;
};


// === IMPLEMENTATION ===

template <typename T>
requires isContentPieceType<T>
inline std::optional<size_t> StorageContentLibrary<T>::find(const std::string& key) const {
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i].get_key() == key) {
            return i;
        }
    }
    return std::nullopt;
}


template <typename T>
requires isContentPieceType<T>
bool StorageContentLibrary<T>::contains(const std::string& key) const {
    return find(key).has_value();
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
Opt<CRef<T>> StorageContentLibrary<T>::get(size_t index) const {
    if (index >= data.size()) {
        return std::nullopt;
    }
    return std::cref(data[index]);
}

template <typename T>
requires isContentPieceType<T>
Opt<CRef<T>> StorageContentLibrary<T>::get(const std::string& key) const {
    std::optional<size_t> idx = find(key);
    if (!idx.has_value()) {
        return std::nullopt;
    }
    return data[idx.value()];
}

template <typename T>
requires isContentPieceType<T>
const std::vector<T>& StorageContentLibrary<T>::get_all() const {
    return data;
}

template <typename T>
requires isContentPieceType<T>
const std::vector<std::pair<typename T::Data, Errors>>& StorageContentLibrary<T>::get_drafts() const {
    return drafts;
}

template <typename T>
requires isContentPieceType<T>
std::optional<size_t> StorageContentLibrary<T>::add(T&& content_piece) {
    try {
        data.emplace_back(std::move(content_piece));
    } catch (const std::exception& e) {
        DND_UNUSED(e);
        return std::nullopt;
    }
    return data.size() - 1;
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

} // namespace dnd

#endif // STORAGE_CONTENT_LIBRARY_HPP_
