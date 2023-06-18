#include <dnd_config.hpp>

#include "content_search.hpp"

#include <algorithm>
#include <cassert>
#include <stack>
#include <string>
#include <unordered_map>

#include <core/controllers/content_holder.hpp>
#include <core/controllers/searching/trie.hpp>
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

dnd::ContentSearch::ContentSearch(const ContentHolder& content_holder) {
    query.reserve(40);
    character_search_path.push(content_holder.characters.get_trie_root());
    character_class_search_path.push(content_holder.character_classes.get_trie_root());
    character_subclass_search_path.push(content_holder.character_subclasses.get_trie_root());
    character_race_search_path.push(content_holder.character_races.get_trie_root());
    character_subrace_search_path.push(content_holder.character_subraces.get_trie_root());
    item_search_path.push(content_holder.items.get_trie_root());
    spell_search_path.push(content_holder.spells.get_trie_root());
    feature_search_path.push(content_holder.features.get_trie_root());
    choosable_search_path.push(content_holder.choosable_features.get_trie_root());
}

dnd::ContentSearch::ContentSearch(const dnd::ContentHolder& content_holder, const std::string& initial_query)
    : ContentSearch(content_holder) {
    for (char c : initial_query) {
        add_character_to_query(c);
    }
}

void dnd::ContentSearch::set_search_query(const std::string& new_query) {
    if (new_query.empty()) {
        clear_query();
        return;
    }
    while (query.size() > new_query.size()) {
        remove_character_from_query();
    }
    assert(query.size() == new_query.size());

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
        assert(query[i] == new_query[i]);
    }
}

void dnd::ContentSearch::clear_query() {
    while (!query.empty()) {
        remove_character_from_query();
    }
}

void dnd::ContentSearch::add_character_to_query(char c) {
    query.push_back(c);

    // character_search_path.push_top_child(c);
    // character_class_search_path.push_top_child(c);
    // character_subclass_search_path.push_top_child(c);
    // character_race_search_path.push_top_child(c);
    // character_subrace_search_path.push_top_child(c);
    item_search_path.push_top_child(c);
    spell_search_path.push_top_child(c);
    // feature_search_path.push_top_child(c);
    // for (auto& [_, choosable_search_path] : choosable_search_paths) {
    //     choosable_search_path.push_top_child(c);
    // }
}

void dnd::ContentSearch::remove_character_from_query() {
    if (query.empty()) {
        return;
    }
    query.pop_back();

    // character_search_path.pop();
    // assert(character_search_path.size() >= 1);
    // character_class_search_path.pop();
    // assert(character_class_search_path.size() >= 1);
    // character_subclass_search_path.pop();
    // assert(character_subclass_search_path.size() >= 1);
    // character_race_search_path.pop();
    // assert(character_race_search_path.size() >= 1);
    // character_subrace_search_path.pop();
    // assert(character_subrace_search_path.size() >= 1);
    item_search_path.pop();
    assert(item_search_path.size() >= 1);
    spell_search_path.pop();
    assert(spell_search_path.size() >= 1);
    // feature_search_path.pop();
    // assert(feature_search_path.size() >= 1);
    // for (auto& [choosable_group_name, choosable_search_path] : choosable_search_paths) {
    //     choosable_search_path.pop();
    //     assert(choosable_search_path.size() >= 1);
    // }
}

std::vector<const dnd::ContentPiece*> dnd::ContentSearch::get_results() const {
    std::vector<const ContentPiece*> results;
    results.reserve(100);

    // character_search_path.insert_top_successors_into(results);
    // character_class_search_path.insert_top_successors_into(results);
    // character_subclass_search_path.insert_top_successors_into(results);
    // character_race_search_path.insert_top_successors_into(results);
    // character_subrace_search_path.insert_top_successors_into(results);
    item_search_path.insert_top_successors_into(results);
    spell_search_path.insert_top_successors_into(results);
    // feature_search_path.insert_top_successors_into(results);
    // for (const auto& [_, choosable_search_path] : choosable_search_paths) {
    //     choosable_search_path.insert_top_successors_into(results);
    // }

    return results;
}

std::vector<const dnd::ContentPiece*> dnd::ContentSearch::get_sorted_results() const {
    std::vector<const dnd::ContentPiece*> results = get_results();
    std::sort(results.begin(), results.end(), [](const ContentPiece* a, const ContentPiece* b) {
        return a->get_name() < b->get_name();
    });
    return results;
}
