#ifndef CONTENT_SEARCH_HPP_
#define CONTENT_SEARCH_HPP_

#include "dnd_config.hpp"

#include <stack>
#include <string>
#include <unordered_map>

#include "controllers/content_holder.hpp"
#include "controllers/searching/trie.hpp"
#include "models/character.hpp"
#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/character_subclass.hpp"
#include "models/character_subrace.hpp"
#include "models/content_piece.hpp"
#include "models/effect_holder/choosable.hpp"
#include "models/effect_holder/feature.hpp"
#include "models/item.hpp"
#include "models/spell.hpp"

namespace dnd {

template <typename T>
using SearchPath = std::stack<const TrieNode<T>*>;

/**
 * @brief A class representing a content search query
 */
class ContentSearch {
public:
    /**
     * @brief Initialize the search with the content holder that should be searched
     * @param content_holder the content holder to search
     */
    ContentSearch(const ContentHolder& content_holder);
    /**
     * @brief Initialize the search with the content holder that should be searched and an initial query
     * @param content_holder the content holder to search
     * @param initial_query the initial search query
     */
    ContentSearch(const ContentHolder& content_holder, const std::string& initial_query);
    /**
     * @brief Set the search query
     * @param query the search query
     */
    void set_search_query(const std::string& new_query);
    /**
     * @brief Set the search query to an empty string
     */
    void clear_query();
    /**
     * @brief Add a character to the back of the search query
     * @param c the character to add
     */
    void add_character_to_query(char c);
    /**
     * @brief Remove the last character from the search query
     */
    void remove_character_from_query();
    /**
     * @brief Get the search results
     * @return the results for the current search query
     */
    std::vector<const ContentPiece*> get_results() const;
    /**
     * @brief Get the search results sorted by name
     * @return the results for the current search query sorted by name
     */
    std::vector<const ContentPiece*> get_sorted_results() const;
private:
    std::vector<char> query;
    SearchPath<Character> character_search_path;
    SearchPath<const CharacterClass> character_class_search_path;
    SearchPath<const CharacterSubclass> character_subclass_search_path;
    SearchPath<const CharacterRace> character_race_search_path;
    SearchPath<const CharacterSubrace> character_subrace_search_path;
    SearchPath<const Item> item_search_path;
    SearchPath<const Spell> spell_search_path;
    SearchPath<const Feature> feature_search_path;
    std::unordered_map<std::string, SearchPath<const Choosable>> choosable_search_paths;
};

} // namespace dnd

#endif // CONTENT_SEARCH_HPP_
