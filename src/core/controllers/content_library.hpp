#ifndef CONTENT_LIBRARY_HPP_
#define CONTENT_LIBRARY_HPP_

#include "dnd_config.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <iostream>

#include "controllers/searching/trie.hpp"
#include "models/content_piece.hpp"

namespace dnd {

template <typename TrieT, typename DataT>
concept validContentLibraryTypes = ContentPieceType<TrieT>
                                   && std::is_same_v<TrieT, typename std::remove_pointer<DataT>::type>;
/**
 * @brief A library of content pieces
 * @tparam TrieT the trie type to use for searching
 * @tparam DataT the data type to use for retrieving content pieces
 */
template <typename TrieT, typename DataT>
    requires validContentLibraryTypes<TrieT, DataT>
class ContentLibrary {
public:
    /**
     * @brief Check whether piece of content with given name exists
     * @param name the name
     * @return true if such a piece of content exists, false otherwise
     */
    bool contains(const std::string& name) const;
    /**
     * @brief Returns whether the content library is empty
     * @return true if the content library is empty, false otherwise
     */
    bool empty() const;
    /**
     * @brief Get the amount of content pieces stored
     * @return the amount
     */
    size_t size() const;
    /**
     * @brief Get content piece by its name
     * @return reference to the piece of content if it exists
     * @throws std::out_of_range if the piece of content does not exist
     */
    DataT& get(const std::string& name);
    /**
     * @brief Get content piece by its name
     * @return constant reference to the piece of content if it exists
     * @throws std::out_of_range if the piece of content does not exist
     */
    const DataT& get(const std::string& name) const;
    /**
     * @brief Get all pieces of content matching a given prefix
     * @param prefix the prefix to the name (or the prefix to one word of the whole name)
     * @return all pieces of content matching the prefix (empty if none such piece exists)
     */
    std::unordered_set<TrieT*> prefix_get(const std::string& prefix);
    /**
     * @brief Get all pieces of content matching a given prefix in order
     * @param prefix the prefix to the name (or the prefix to one word of the whole name)
     * @return all pieces of content matching the prefix in order (empty if none such piece exists)
     */
    std::vector<TrieT*> sorted_prefix_get(const std::string& prefix);
    /**
     * @brief Get all pieces of content
     * @return the map containing the pieces of content
     */
    const std::unordered_map<std::string, DataT>& get_all() const;
    /**
     * @brief Add a content piece if no piece of content with the same name exists
     * @param name the name of the piece of content
     * @param new_content_piece the new piece of content
     * @return true, if piece was added, false if name already exists
     */
    bool add(const std::string& name, DataT&& new_content_piece);
    /**
     * @brief Create a content piece inplace if no piece of content with the same name exists
     * @tparam ...Args the types the constructor for the piece of content allows
     * @param name the name of the piece of content
     * @param ...constructor_args the constructor arguments to create the piece of content
     * @return true, if piece was created and added, false if name already exists
     */
    template <
        typename... Args,
        std::enable_if_t<std::is_constructible<DataT, std::string&, std::filesystem::path&, Args&&...>::value, int> = 0>
    bool create(const std::string& name, const std::filesystem::path& source_file_path, Args&&... constructor_args);
    /**
     * @brief Get the root of the search trie
     * @return a pointer the root node
     */
    const TrieNode<TrieT>* get_trie_root() const;
protected:
    /**
     * @brief Saves a piece of content that already exists in the data-map into the trie under multiple names
     * @param name the full name of the piece of content
     */
    void save_in_trie(const std::string& name);

    std::unordered_map<std::string, DataT> data;
    Trie<TrieT> trie;
};

/**
 * @brief A content library that stores the content pieces directly.
 * @tparam T the type of the content pieces being stored
 */
template <typename T>
using StoringContentLibrary = ContentLibrary<T, T>;

/**
 * @brief A content library that stores pointers to the content pieces, which are stored elsewhere.
 * @tparam T the type of the content pieces being referenced
 */
template <typename T>
using ReferencingContentLibrary = ContentLibrary<T, T*>;


template <typename TrieT, typename DataT>
    requires validContentLibraryTypes<TrieT, DataT>
inline bool ContentLibrary<TrieT, DataT>::contains(const std::string& name) const {
    return data.contains(name);
}

template <typename TrieT, typename DataT>
    requires validContentLibraryTypes<TrieT, DataT>
inline bool ContentLibrary<TrieT, DataT>::empty() const {
    return data.empty();
}

template <typename TrieT, typename DataT>
    requires validContentLibraryTypes<TrieT, DataT>
inline size_t ContentLibrary<TrieT, DataT>::size() const {
    return data.size();
}

template <typename TrieT, typename DataT>
    requires validContentLibraryTypes<TrieT, DataT>
inline DataT& ContentLibrary<TrieT, DataT>::get(const std::string& name) {
    return data.at(name);
}

template <typename TrieT, typename DataT>
    requires validContentLibraryTypes<TrieT, DataT>
inline const DataT& ContentLibrary<TrieT, DataT>::get(const std::string& name) const {
    return data.at(name);
}

template <typename TrieT, typename DataT>
    requires validContentLibraryTypes<TrieT, DataT>
inline std::unordered_set<TrieT*> ContentLibrary<TrieT, DataT>::prefix_get(const std::string& prefix) {
    return trie.search_prefix(prefix);
}

template <typename TrieT, typename DataT>
    requires validContentLibraryTypes<TrieT, DataT>
inline std::vector<TrieT*> ContentLibrary<TrieT, DataT>::sorted_prefix_get(const std::string& prefix) {
    std::unordered_set<TrieT*> unsorted_result = prefix_get(prefix);
    std::vector<TrieT*> result(unsorted_result.begin(), unsorted_result.end());
    std::sort(result.begin(), result.end(), [](TrieT* a, TrieT* b) { return a->name < b->name; });
    return result;
}

template <typename TrieT, typename DataT>
    requires validContentLibraryTypes<TrieT, DataT>
inline const std::unordered_map<std::string, DataT>& ContentLibrary<TrieT, DataT>::get_all() const {
    return data;
}

template <typename TrieT, typename DataT>
    requires validContentLibraryTypes<TrieT, DataT>
bool ContentLibrary<TrieT, DataT>::add(const std::string& name, DataT&& new_content_piece) {
    if (contains(name)) {
        return false;
    }
    data.emplace(name, new_content_piece);
    save_in_trie(name);
    return true;
}

template <typename TrieT, typename DataT>
    requires validContentLibraryTypes<TrieT, DataT>
template <
    typename... Args,
    std::enable_if_t<std::is_constructible<DataT, std::string&, std::filesystem::path&, Args&&...>::value, int>>
bool ContentLibrary<TrieT, DataT>::create(
    const std::string& name, const std::filesystem::path& source_file_path, Args&&... constructor_args
) {
    if (contains(name)) {
        return false;
    }
    // const std::string name_copy = name;
    data.emplace(
        std::piecewise_construct, std::forward_as_tuple(name),
        std::forward_as_tuple(name, source_file_path, std::move(constructor_args)...)
    );
    save_in_trie(name);
    return true;
}

template <typename TrieT, typename DataT>
    requires validContentLibraryTypes<TrieT, DataT>
void ContentLibrary<TrieT, DataT>::save_in_trie(const std::string& name) {
    DataT& content_piece_ptr = data.at(name);

    auto tolower = [](unsigned char c) { return static_cast<unsigned char>(std::tolower(c)); };
    std::string lower_name = name;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), tolower);

    trie.insert(lower_name, content_piece_ptr);
    for (size_t i = 0; i < lower_name.size(); ++i) {
        if (lower_name[i] == ' ') {
            std::string_view after_space(lower_name.c_str() + i + 1, lower_name.size() - i - 1);
            trie.insert(after_space, content_piece_ptr);
        }
    }
}

template <typename TrieT, typename DataT>
    requires validContentLibraryTypes<TrieT, DataT>
const TrieNode<TrieT>* ContentLibrary<TrieT, DataT>::get_trie_root() const {
    return trie.get_root();
}

} // namespace dnd

#endif // CONTENT_LIBRARY_HPP_
