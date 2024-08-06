#include <cstdint>
#include <dnd_config.hpp>

#include "fuzzy_content_search.hpp"

#include <array>
#include <string>
#include <vector>

#include <core/content.hpp>
#include <core/searching/fuzzy_search/fuzzy_string_search.hpp>
#include <core/searching/search_result.hpp>

namespace dnd {

std::vector<SearchResult> fuzzy_search_content(
    const Content& content, const std::string& search_query, const FuzzySearchOptions& options
) {
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
