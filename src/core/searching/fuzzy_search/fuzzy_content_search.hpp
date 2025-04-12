#ifndef FUZZY_CONTENT_SEARCH_HPP_
#define FUZZY_CONTENT_SEARCH_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <string>

#include <core/content.hpp>
#include <core/searching/search_result.hpp>

namespace dnd {

constexpr uint32_t FUZZY_SEARCH_MINIMUM_QUERY_LENGTH = 1;

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

std::vector<SearchResult> fuzzy_search_content(
    const Content& content, const std::string& search_query, const FuzzySearchOptions& options
);


} // namespace dnd

#endif // FUZZY_CONTENT_SEARCH_HPP_
