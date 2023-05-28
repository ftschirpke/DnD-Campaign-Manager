#ifndef CONTENT_LIBRARY_HPP_
#define CONTENT_LIBRARY_HPP_

#include "dnd_config.hpp"

#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "controllers/trie.hpp"

namespace dnd {

template <typename T>
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
    T& get(const std::string& name);
    /**
     * @brief Get content piece by its name
     * @return constant reference to the piece of content if it exists
     * @throws std::out_of_range if the piece of content does not exist
     */
    const T& get(const std::string& name) const;
    /**
     * @brief Get all pieces of content matching a given prefix
     * @param prefix the prefix to the name (or the prefix to one word of the whole name)
     * @return all pieces of content matching the prefix (empty if none such piece exists)
     */
    std::unordered_set<T*> prefix_get(const std::string& prefix);
    /**
     * @brief Get all pieces of content matching a given prefix in order
     * @param prefix the prefix to the name (or the prefix to one word of the whole name)
     * @return all pieces of content matching the prefix in order (empty if none such piece exists)
     */
    std::vector<T*> sorted_prefix_get(const std::string& prefix);
    /**
     * @brief Get all pieces of content
     * @return the map containing the pieces of content
     */
    const std::unordered_map<std::string, T>& get_all() const;
    /**
     * @brief Add a content piece if no piece of content with the same name exists
     * @param name the name of the piece of content
     * @param new_content_piece the new piece of content
     * @return true, if piece was added, false if name already exists
     */
    bool add(const std::string& name, T&& new_content_piece);
    /**
     * @brief Create a content piece inplace if no piece of content with the same name exists
     * @tparam ...Args the types the constructor for the piece of content allows
     * @param name the name of the piece of content
     * @param ...constructor_args the constructor arguments to create the piece of content
     * @return true, if piece was created and added, false if name already exists
     */
    template <typename... Args, std::enable_if_t<std::is_constructible<T, Args&&...>::value, int> = 0>
    bool create(const std::string& name, Args&&... constructor_args);
protected:
    /**
     * @brief Saves a piece of content that already exists in the data-map into the trie under multiple names
     * @param name the full name of the piece of content
     */
    void save_in_trie(const std::string& name);

    std::unordered_map<std::string, T> data;
    Trie<T> trie;
};

template <typename T>
inline bool ContentLibrary<T>::contains(const std::string& name) const {
    return data.contains(name);
}

template <typename T>
inline bool ContentLibrary<T>::empty() const {
    return data.empty();
}

template <typename T>
inline size_t ContentLibrary<T>::size() const {
    return data.size();
}

template <typename T>
inline T& ContentLibrary<T>::get(const std::string& name) {
    return data.at(name);
}

template <typename T>
inline const T& ContentLibrary<T>::get(const std::string& name) const {
    return data.at(name);
}

template <typename T>
inline std::unordered_set<T*> ContentLibrary<T>::prefix_get(const std::string& prefix) {
    return trie.search_prefix(prefix);
}

template <typename T>
inline const std::unordered_map<std::string, T>& ContentLibrary<T>::get_all() const {
    return data;
}

template <typename T>
bool ContentLibrary<T>::add(const std::string& name, T&& new_content_piece) {
    if (contains(name)) {
        return false;
    }
    data.emplace(name, new_content_piece);
    save_in_trie(name);
    return true;
}

template <typename T>
template <typename... Args, std::enable_if_t<std::is_constructible<T, Args&&...>::value, int>>
bool ContentLibrary<T>::create(const std::string& name, Args&&... constructor_args) {
    if (contains(name)) {
        return false;
    }
    data.emplace(
        std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(std::move(constructor_args)...)
    );
    save_in_trie(name);
    return true;
}

template <typename T>
void ContentLibrary<T>::save_in_trie(const std::string& name) {
    T* content_piece_ptr = &data.at(name);

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

} // namespace dnd

#endif // CONTENT_LIBRARY_HPP_
