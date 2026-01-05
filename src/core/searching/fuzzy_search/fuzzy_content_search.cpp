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
        for (const auto& [_, character] : content.get_characters().get_all()) {
            int64_t match_score = fuzzy_match_string(search_query, character.get_name());
            if (match_score > min_match_score) {
                results.emplace_back(&character, match_score);
            }
        }
    }
    if (options.search_species) {
        for (const auto& [_, species] : content.get_species().get_all()) {
            int64_t match_score = fuzzy_match_string(search_query, species.get_name());
            if (match_score > min_match_score) {
                results.emplace_back(&species, match_score);
            }
        }
    }
    if (options.search_classes) {
        for (const auto& [_, cls] : content.get_classes().get_all()) {
            int64_t match_score = fuzzy_match_string(search_query, cls.get_name());
            if (match_score > min_match_score) {
                results.emplace_back(&cls, match_score);
            }
        }
    }
    if (options.search_subspecies) {
        for (const auto& [_, subspecies] : content.get_subspecies().get_all()) {
            int64_t match_score = fuzzy_match_string(search_query, subspecies.get_name());
            if (match_score > min_match_score) {
                results.emplace_back(&subspecies, match_score);
            }
        }
    }
    if (options.search_subclasses) {
        for (const auto& [_, subclass] : content.get_subclasses().get_all()) {
            int64_t match_score = fuzzy_match_string(search_query, subclass.get_name());
            if (match_score > min_match_score) {
                results.emplace_back(&subclass, match_score);
            }
        }
    }
    if (options.search_items) {
        for (const auto& [_, item] : content.get_items().get_all()) {
            int64_t match_score = fuzzy_match_string(search_query, item.get_name());
            if (match_score > min_match_score) {
                results.emplace_back(&item, match_score);
            }
        }
    }
    if (options.search_spells) {
        for (const auto& [_, spell] : content.get_spells().get_all()) {
            int64_t match_score = fuzzy_match_string(search_query, spell.get_name());
            if (match_score > min_match_score) {
                results.emplace_back(&spell, match_score);
            }
        }
    }
    if (options.search_features) {
        for (const auto& [_, feature] : content.get_features().get_all()) {
            int64_t match_score = fuzzy_match_string(search_query, feature.get().get_name());
            if (match_score > min_match_score) {
                results.emplace_back(&feature.get(), match_score);
            }
        }
        for (const auto& [_, feature] : content.get_class_features().get_all()) {
            int64_t match_score = fuzzy_match_string(search_query, feature.get().get_name());
            if (match_score > min_match_score) {
                results.emplace_back(&feature.get(), match_score);
            }
        }
        for (const auto& [_, feature] : content.get_subclass_features().get_all()) {
            int64_t match_score = fuzzy_match_string(search_query, feature.get().get_name());
            if (match_score > min_match_score) {
                results.emplace_back(&feature.get(), match_score);
            }
        }
    }
    if (options.search_choosables) {
        for (const auto& [_, choosable] : content.get_choosables().get_all()) {
            int64_t match_score = fuzzy_match_string(search_query, choosable.get_name());
            if (match_score > min_match_score) {
                results.emplace_back(&choosable, match_score);
            }
        }
    }

    return results;
}

} // namespace dnd
