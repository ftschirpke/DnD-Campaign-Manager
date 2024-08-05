#include <cstdint>
#include <dnd_config.hpp>

#include "fuzzy_content_search.hpp"

#include <array>
#include <cassert>
#include <string>

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
#include <core/searching/fuzzy_search/fuzzy_string_search.hpp>
#include <core/searching/search_result.hpp>
#include <core/utils/char_manipulation.hpp>

namespace dnd {

FuzzyContentSearch::FuzzyContentSearch(const Content& content) : content(content) {
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

void FuzzyContentSearch::set_search_query(const std::string& new_query) { search_query = new_query; }

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

std::vector<SearchResult> FuzzyContentSearch::get_results(const FuzzySearchOptions& options) const {
    DND_MEASURE_FUNCTION();
    std::vector<SearchResult> results;

    int64_t min_match_score = 0;

    if (options.search_characters) {
        for (const auto& [character_name, character] : content.get_characters().get_all()) {
            int64_t match_score = fuzzy_match_string(search_query, character_name);
            if (match_score > min_match_score) {
                results.emplace_back(&character, match_score);
            }
        }
    }
    if (options.search_species) {
        for (const auto& [species_name, species] : content.get_species().get_all()) {
            int64_t match_score = fuzzy_match_string(search_query, species_name);
            if (match_score > min_match_score) {
                results.emplace_back(&species, match_score);
            }
        }
    }
    if (options.search_classes) {
        for (const auto& [class_name, cls] : content.get_classes().get_all()) {
            int64_t match_score = fuzzy_match_string(search_query, class_name);
            if (match_score > min_match_score) {
                results.emplace_back(&cls, match_score);
            }
        }
    }
    if (options.search_subspecies) {
        for (const auto& [subspecies_name, subspecies] : content.get_subspecies().get_all()) {
            int64_t match_score = fuzzy_match_string(search_query, subspecies_name);
            if (match_score > min_match_score) {
                results.emplace_back(&subspecies, match_score);
            }
        }
    }
    if (options.search_subclasses) {
        for (const auto& [subclass_name, subclass] : content.get_subclasses().get_all()) {
            int64_t match_score = fuzzy_match_string(search_query, subclass_name);
            if (match_score > min_match_score) {
                results.emplace_back(&subclass, match_score);
            }
        }
    }
    if (options.search_items) {
        for (const auto& [item_name, item] : content.get_items().get_all()) {
            int64_t match_score = fuzzy_match_string(search_query, item_name);
            if (match_score > min_match_score) {
                results.emplace_back(&item, match_score);
            }
        }
    }
    if (options.search_spells) {
        for (const auto& [spell_name, spell] : content.get_spells().get_all()) {
            int64_t match_score = fuzzy_match_string(search_query, spell_name);
            if (match_score > min_match_score) {
                results.emplace_back(&spell, match_score);
            }
        }
    }
    if (options.search_features) {
        for (const auto& [feature_name, feature] : content.get_features().get_all()) {
            int64_t match_score = fuzzy_match_string(search_query, feature_name);
            if (match_score > min_match_score) {
                results.emplace_back(&feature.get(), match_score);
            }
        }
    }
    if (options.search_choosables) {
        for (const auto& [choosable_name, choosable] : content.get_choosables().get_all()) {
            int64_t match_score = fuzzy_match_string(search_query, choosable_name);
            if (match_score > min_match_score) {
                results.emplace_back(&choosable, match_score);
            }
        }
    }

    return results;
}

} // namespace dnd
