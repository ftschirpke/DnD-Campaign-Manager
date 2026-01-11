#ifndef REFERENCING_CONTENT_LIBRARY_HPP_
#define REFERENCING_CONTENT_LIBRARY_HPP_

#include <dnd_config.hpp>

#include <optional>
#include <string>
#include <vector>

#include <fmt/format.h>

#include <core/content_library.hpp>
#include <core/models/effects_provider/class_feature.hpp>
#include <core/types.hpp>
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
    std::optional<size_t> find(const std::string& key) const;
    bool contains(const std::string& key) const override;
    bool empty() const override;
    size_t size() const override;
    Opt<CRef<T>> get(size_t index) const override;
    Opt<CRef<T>> get(const std::string& key) const override;
    const std::vector<std::reference_wrapper<const T>>& get_all() const;
    /**
     * @brief Add a content piece to a content piece to the library
     * @param content_piece the content piece to add
     * @return reference to the inserted content piece, or std::nullopt if a content piece with that key already exists
     */
    std::optional<size_t> add(const T& content_piece);
private:
    std::vector<std::reference_wrapper<const T>> data;
};


// === IMPLEMENTATION ===

template <typename T>
requires isContentPieceType<T>
inline std::optional<size_t> ReferencingContentLibrary<T>::find(const std::string& key) const {
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i].get().get_key() == key) {
            return i;
        }
    }
    return std::nullopt;
}

template <typename T>
requires isContentPieceType<T>
inline bool ReferencingContentLibrary<T>::contains(const std::string& key) const {
    return find(key).has_value();
}

template <typename T>
requires isContentPieceType<T>
inline bool ReferencingContentLibrary<T>::empty() const {
    return data.empty();
}

template <typename T>
requires isContentPieceType<T>
inline size_t ReferencingContentLibrary<T>::size() const {
    return data.size();
}

template <typename T>
requires isContentPieceType<T>
inline Opt<CRef<T>> ReferencingContentLibrary<T>::get(size_t index) const {
    if (index >= data.size()) {
        return std::nullopt;
    }
    return data[index];
}

template <typename T>
requires isContentPieceType<T>
inline Opt<CRef<T>> ReferencingContentLibrary<T>::get(const std::string& key) const {
    std::optional<size_t> idx = find(key);
    if (!idx.has_value()) {
        return std::nullopt;
    }
    return data[idx.value()];
}

template <typename T>
requires isContentPieceType<T>
inline const std::vector<std::reference_wrapper<const T>>& ReferencingContentLibrary<T>::get_all() const {
    return data;
}

template <typename T>
requires isContentPieceType<T>
inline std::optional<size_t> ReferencingContentLibrary<T>::add(const T& content_piece) {
    try {
        data.emplace_back(std::cref(content_piece));
    } catch (const std::exception& _) {
        return std::nullopt;
    }
    return data.size() - 1;
}

} // namespace dnd

#endif // REFERENCING_CONTENT_LIBRARY_HPP_
