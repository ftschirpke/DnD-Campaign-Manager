#ifndef FUZZY_CONTENT_SEARCH_HPP_
#define FUZZY_CONTENT_SEARCH_HPP_

#include <dnd_config.hpp>

#include <array>
#include <stack>
#include <string>
#include <unordered_map>

#include <core/content.hpp>
#include <core/models/character/character.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_species/character_species.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/models/character_subspecies/character_subspecies.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/spell/spell.hpp>
#include <core/searching/fuzzy_search/fuzzy_search_path.hpp>
#include <core/searching/fuzzy_search/trie.hpp>

namespace dnd {

/**
 * @brief A class representing a content search query
 */
class FuzzyContentSearch {
public:
    /**
     * @brief Initialize the search with the content that should be searched
     * @param content the content to search
     */
    FuzzyContentSearch(const Content& content);
    /**
     * @brief Initialize the search with the content that should be searched and an initial query
     * @param content the content to search
     * @param initial_query the initial search query
     */
    FuzzyContentSearch(const Content& content, const std::string& initial_query);
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
    std::vector<const ContentPiece*> get_results(const std::array<bool, 9>& options) const;
private:
    std::vector<char> query;
    FuzzySearchPath<Character> character_search_path;
    FuzzySearchPath<CharacterClass> character_class_search_path;
    FuzzySearchPath<CharacterSubclass> character_subclass_search_path;
    FuzzySearchPath<CharacterSpecies> character_species_search_path;
    FuzzySearchPath<CharacterSubspecies> character_subspecies_search_path;
    FuzzySearchPath<Item> item_search_path;
    FuzzySearchPath<Spell> spell_search_path;
    FuzzySearchPath<Feature> feature_search_path;
    FuzzySearchPath<Choosable> choosable_search_path;
};


} // namespace dnd

#endif // FUZZY_CONTENT_SEARCH_HPP_
