#ifndef CONTENT_HPP_
#define CONTENT_HPP_

#include "dnd_config.hpp"

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "controllers/trie.hpp"

namespace dnd {

template <typename T>
class Content {
public:
    /**
     * @brief Get content piece by its name
     * @return the piece of content if it exists
     * @throws std::out_of_range if the piece of content does not exist
     */
    T& get(const std::string& name);
    /**
     * @brief Get all pieces of content matching a given prefix
     * @param prefix the prefix to the name (or the prefix to one word of the whole name)
     * @return all pieces of content matching the prefix (empty if none such piece exists)
     */
    std::vector<T*> prefix_get(const std::string& prefix);
    /**
     * @brief Get all pieces of content
     * @return the map containing the pieces of content
     */
    const std::unordered_map<std::string, T>& get_all() const;
    /**
     * @brief Add a content piece if no piece of content with the same name already exists
     * @param name the name of the piece of content
     * @param new_content_piece the new piece of content
     * @return true, if piece was added, false if name already exists
     */
    bool add(const std::string& name, T&& new_content_piece);
protected:
    std::unordered_map<std::string, T> data;
    Trie<T> trie;
private:
    /**
     * @brief Inserts the content piece into the trie
     * @param name the name of the piece of content
     * @param content_piece a pointer to the piece of content to insert
     */
    void trie_insert(std::string_view name, T* content_piece);
};

template <typename T>
inline T& Content<T>::get(const std::string& name) {
    return data.at(name);
}

template <typename T>
inline std::vector<T*> Content<T>::prefix_get(const std::string& prefix) {
    return trie.search_prefix(prefix);
}

template <typename T>
inline const std::unordered_map<std::string, T>& Content<T>::get_all() const {
    return data;
}

template <typename T>
bool Content<T>::add(const std::string& name, T&& new_content_piece) {
    if (data.contains(name)) {
        return false;
    }
    data.emplace(name, new_content_piece);

    size_t i = 0;
    T* content_piece_ptr = &data.at(name);
    trie.insert(name, content_piece_ptr);
    for (size_t i = 0; i < name.size(); ++i) {
        if (name[i] == ' ') {
            std::string_view after_space(name.c_str(), name.size() - i - 1);
            trie.insert(after_space, content_piece_ptr);
        }
    }
}

} // namespace dnd

#endif // CONTENT_HPP_
