#ifndef TRIE_CONTENT_SEARCH_HPP_
#define TRIE_CONTENT_SEARCH_HPP_

#include <dnd_config.hpp>

#include <stack>
#include <string>
#include <unordered_map>

#include <core/content.hpp>
#include <core/models/character/character.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/models/character_subrace/character_subrace.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/feature/choosable_feature.hpp>
#include <core/models/feature/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/spell/spell.hpp>
#include <core/searching/trie_search/trie.hpp>
#include <core/searching/trie_search/trie_search_path.hpp>

namespace dnd {

/**
 * @brief A class representing a content search query
 */
class TrieContentSearch {
public:
    /**
     * @brief Initialize the search with the content holder that should be searched
     * @param content_holder the content holder to search
     */
    TrieContentSearch(const Content& content_holder);
    /**
     * @brief Initialize the search with the content holder that should be searched and an initial query
     * @param content_holder the content holder to search
     * @param initial_query the initial search query
     */
    TrieContentSearch(const Content& content_holder, const std::string& initial_query);
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
    TrieSearchPath<Character> character_search_path;
    TrieSearchPath<const CharacterClass> character_class_search_path;
    TrieSearchPath<const CharacterSubclass> character_subclass_search_path;
    TrieSearchPath<const CharacterRace> character_race_search_path;
    TrieSearchPath<const CharacterSubrace> character_subrace_search_path;
    TrieSearchPath<const Item> item_search_path;
    TrieSearchPath<const Spell> spell_search_path;
    TrieSearchPath<const Feature> feature_search_path;
    TrieSearchPath<const ChoosableFeature> choosable_search_path;
};


} // namespace dnd

#endif // TRIE_CONTENT_SEARCH_HPP_
