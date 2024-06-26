#include <dnd_config.hpp>

#include "fuzzy_content_search.hpp"

#include <array>
#include <cassert>
#include <string>
#include <unordered_set>

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
#include <core/utils/char_manipulation.hpp>

namespace dnd {

FuzzyContentSearch::FuzzyContentSearch(const Content& content) {
    query.reserve(40);
    character_search_path.push(content.get_characters().get_fuzzy_search_trie_root());
    class_search_path.push(content.get_classes().get_fuzzy_search_trie_root());
    subclass_search_path.push(content.get_subclasses().get_fuzzy_search_trie_root());
    species_search_path.push(content.get_species().get_fuzzy_search_trie_root());
    subspecies_search_path.push(content.get_subspecies().get_fuzzy_search_trie_root());
    item_search_path.push(content.get_items().get_fuzzy_search_trie_root());
    spell_search_path.push(content.get_spells().get_fuzzy_search_trie_root());
    feature_search_path.push(content.get_features().get_fuzzy_search_trie_root());
    choosable_search_path.push(content.get_choosables().get_fuzzy_search_trie_root());
}

FuzzyContentSearch::FuzzyContentSearch(const Content& content, const std::string& initial_query)
    : FuzzyContentSearch(content) {
    for (char c : initial_query) {
        add_character_to_query(c);
    }
}

void FuzzyContentSearch::set_search_query(const std::string& new_query) {
    if (new_query.empty()) {
        clear_query();
        return;
    }
    while (query.size() > new_query.size()) {
        remove_character_from_query();
    }
    assert(query.size() <= new_query.size());

    size_t common_length = 0;
    while (common_length < query.size() && query[common_length] == new_query[common_length]) {
        ++common_length;
    }

    while (query.size() > common_length) {
        remove_character_from_query();
    }
    assert(query.size() == common_length);

    for (size_t i = common_length; i < new_query.size(); ++i) {
        add_character_to_query(new_query[i]);
    }

    assert(query.size() == new_query.size());
    for (size_t i = 0; i < query.size(); ++i) {
        assert(query[i] == char_to_lowercase(new_query[i]));
    }
}

void FuzzyContentSearch::clear_query() {
    while (!query.empty()) {
        remove_character_from_query();
    }
}

void FuzzyContentSearch::add_character_to_query(char c) {
    c = char_to_lowercase(c);
    query.push_back(c);

    character_search_path.push_top_child(c);
    class_search_path.push_top_child(c);
    subclass_search_path.push_top_child(c);
    species_search_path.push_top_child(c);
    subspecies_search_path.push_top_child(c);
    item_search_path.push_top_child(c);
    spell_search_path.push_top_child(c);
    feature_search_path.push_top_child(c);
    choosable_search_path.push_top_child(c);
}

void FuzzyContentSearch::remove_character_from_query() {
    if (query.empty()) {
        return;
    }
    query.pop_back();

    character_search_path.pop();
    assert(character_search_path.size() >= 1);
    class_search_path.pop();
    assert(class_search_path.size() >= 1);
    subclass_search_path.pop();
    assert(subclass_search_path.size() >= 1);
    species_search_path.pop();
    assert(species_search_path.size() >= 1);
    subspecies_search_path.pop();
    assert(subspecies_search_path.size() >= 1);
    item_search_path.pop();
    assert(item_search_path.size() >= 1);
    spell_search_path.pop();
    assert(spell_search_path.size() >= 1);
    feature_search_path.pop();
    assert(feature_search_path.size() >= 1);
    choosable_search_path.pop();
    assert(choosable_search_path.size() >= 1);
}

std::unordered_set<const ContentPiece*> FuzzyContentSearch::get_results(const std::array<bool, 9>& options) const {
    DND_MEASURE_FUNCTION();
    std::unordered_set<const ContentPiece*> results;

    if (options[0]) {
        character_search_path.insert_top_successors_into(results);
    }
    if (options[1]) {
        species_search_path.insert_top_successors_into(results);
    }
    if (options[2]) {
        class_search_path.insert_top_successors_into(results);
    }
    if (options[3]) {
        subspecies_search_path.insert_top_successors_into(results);
    }
    if (options[4]) {
        subclass_search_path.insert_top_successors_into(results);
    }
    if (options[5]) {
        item_search_path.insert_top_successors_into(results);
    }
    if (options[6]) {
        spell_search_path.insert_top_successors_into(results);
    }
    if (options[7]) {
        feature_search_path.insert_top_successors_into(results);
    }
    if (options[8]) {
        choosable_search_path.insert_top_successors_into(results);
    }

    return results;
}

} // namespace dnd
