#include <dnd_config.hpp>

#include "trie_content_search.hpp"

#include <algorithm>
#include <array>
#include <cassert>
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
#include <core/utils/char_manipulation.hpp>

dnd::TrieContentSearch::TrieContentSearch(const Content& content) {
    query.reserve(40);
    character_search_path.push(content.get_characters().get_trie_root());
    character_class_search_path.push(content.get_character_classes().get_trie_root());
    character_subclass_search_path.push(content.get_character_subclasses().get_trie_root());
    character_race_search_path.push(content.get_character_races().get_trie_root());
    character_subrace_search_path.push(content.get_character_subraces().get_trie_root());
    item_search_path.push(content.get_items().get_trie_root());
    spell_search_path.push(content.get_spells().get_trie_root());
    feature_search_path.push(content.get_features().get_trie_root());
    choosable_search_path.push(content.get_choosable_features().get_trie_root());
}

dnd::TrieContentSearch::TrieContentSearch(const dnd::Content& content, const std::string& initial_query)
    : TrieContentSearch(content) {
    for (char c : initial_query) {
        add_character_to_query(c);
    }
}

void dnd::TrieContentSearch::set_search_query(const std::string& new_query) {
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
        assert(query[i] == dnd::char_to_lowercase(new_query[i]));
    }
}

void dnd::TrieContentSearch::clear_query() {
    while (!query.empty()) {
        remove_character_from_query();
    }
}

void dnd::TrieContentSearch::add_character_to_query(char c) {
    c = dnd::char_to_lowercase(c);
    query.push_back(c);

    character_search_path.push_top_child(c);
    character_class_search_path.push_top_child(c);
    character_subclass_search_path.push_top_child(c);
    character_race_search_path.push_top_child(c);
    character_subrace_search_path.push_top_child(c);
    item_search_path.push_top_child(c);
    spell_search_path.push_top_child(c);
    feature_search_path.push_top_child(c);
    choosable_search_path.push_top_child(c);
}

void dnd::TrieContentSearch::remove_character_from_query() {
    if (query.empty()) {
        return;
    }
    query.pop_back();

    character_search_path.pop();
    assert(character_search_path.size() >= 1);
    character_class_search_path.pop();
    assert(character_class_search_path.size() >= 1);
    character_subclass_search_path.pop();
    assert(character_subclass_search_path.size() >= 1);
    character_race_search_path.pop();
    assert(character_race_search_path.size() >= 1);
    character_subrace_search_path.pop();
    assert(character_subrace_search_path.size() >= 1);
    item_search_path.pop();
    assert(item_search_path.size() >= 1);
    spell_search_path.pop();
    assert(spell_search_path.size() >= 1);
    feature_search_path.pop();
    assert(feature_search_path.size() >= 1);
    choosable_search_path.pop();
    assert(choosable_search_path.size() >= 1);
}

std::vector<const dnd::ContentPiece*> dnd::TrieContentSearch::get_results(const std::array<bool, 9>& options) const {
    DND_MEASURE_FUNCTION();
    std::vector<const ContentPiece*> results;
    results.reserve(500);

    if (options[0]) {
        character_search_path.insert_top_successors_into(results);
    }
    if (options[1]) {
        character_race_search_path.insert_top_successors_into(results);
    }
    if (options[2]) {
        character_class_search_path.insert_top_successors_into(results);
    }
    if (options[3]) {
        character_subrace_search_path.insert_top_successors_into(results);
    }
    if (options[4]) {
        character_subclass_search_path.insert_top_successors_into(results);
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
