#ifndef FUZZY_CONTENT_SEARCH_HPP_
#define FUZZY_CONTENT_SEARCH_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <string>
#include <vector>

#include <core/content.hpp>
#include <core/models/character/character.hpp>
#include <core/models/class/class.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/species/species.hpp>
#include <core/models/spell/spell.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/searching/fuzzy_search/fuzzy_search_path.hpp>
#include <core/searching/fuzzy_search/trie.hpp>
#include <core/searching/search_result.hpp>

namespace dnd {

struct FuzzySearchOptions {
    bool search_characters;
    bool search_classes;
    bool search_subclasses;
    bool search_species;
    bool search_subspecies;
    bool search_items;
    bool search_spells;
    bool search_features;
    bool search_choosables;

    std::strong_ordering operator<=>(const FuzzySearchOptions&) const = default;

    void set_all(bool value) {
        search_characters = value;
        search_classes = value;
        search_subclasses = value;
        search_species = value;
        search_subspecies = value;
        search_items = value;
        search_spells = value;
        search_features = value;
        search_choosables = value;
    }
};

/**
 * @brief A class representing a content search query
 */
class FuzzyContentSearch {
public:
    FuzzyContentSearch(const Content& content);
    FuzzyContentSearch(const Content& content, const std::string& initial_query);

    void set_search_query(const std::string& new_query);
    void clear_query();
    void add_character_to_query(char c);
    void remove_character_from_query();

    std::vector<SearchResult> get_results(const FuzzySearchOptions& options) const;
private:
    const Content& content;
    std::string search_query;
    std::vector<char> query;
    FuzzySearchPath<Character> character_search_path;
    FuzzySearchPath<Class> class_search_path;
    FuzzySearchPath<Subclass> subclass_search_path;
    FuzzySearchPath<Species> species_search_path;
    FuzzySearchPath<Subspecies> subspecies_search_path;
    FuzzySearchPath<Item> item_search_path;
    FuzzySearchPath<Spell> spell_search_path;
    FuzzySearchPath<Feature> feature_search_path;
    FuzzySearchPath<Choosable> choosable_search_path;
};


} // namespace dnd

#endif // FUZZY_CONTENT_SEARCH_HPP_
