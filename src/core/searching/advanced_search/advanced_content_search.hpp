#ifndef ADVANCED_CONTENT_SEARCH_HPP_
#define ADVANCED_CONTENT_SEARCH_HPP_

#include <dnd_config.hpp>

#include <future>
#include <variant>
#include <vector>

#include <core/content.hpp>
#include <core/searching/content_filters/character/character_filter.hpp>
#include <core/searching/content_filters/character_class/character_class_filter.hpp>
#include <core/searching/content_filters/character_race/character_race_filter.hpp>
#include <core/searching/content_filters/character_subclass/character_subclass_filter.hpp>
#include <core/searching/content_filters/character_subrace/character_subrace_filter.hpp>
#include <core/searching/content_filters/content_filter.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>
#include <core/searching/content_filters/spell/spell_filter.hpp>
#include <core/visitors/filters/content_filter_visitor.hpp>

namespace dnd {

using ContentFilterVariant = std::variant<
    ContentPieceFilter, CharacterFilter, CharacterClassFilter, CharacterSubclassFilter, CharacterRaceFilter,
    CharacterSubraceFilter, SpellFilter>;

/**
 * @brief A class representing an advanced content search using a content filter.
 */
class AdvancedContentSearch {
public:
    /**
     * @brief Initialize the advanced search with the content that should be searched
     * @param content the content to search
     */
    AdvancedContentSearch(const Content& content) noexcept;
    /**
     * @brief Set the search filter
     * @param new_filter the filter to set
     */
    template <typename T>
    void set_filter(T&& new_filter);
    /**
     * @brief Get the search filter
     * @return the search filter
     */
    ContentFilterVariant& get_filter();
    /**
     * @brief Get the search results
     * @return the search results
     */
    const std::vector<const ContentPiece*>& get_search_results() const noexcept;

    /**
     * @brief Start searching the content for matches to the current set filter.
     */
    void start_searching();
    /**
     * @brief Determines if the search is currently searching and updates the search results if the search just finished
     * @return "true" if the search is currently searching, "false" otherwise
     */
    bool is_searching();
private:
    const Content& content;
    ContentFilterVariant filter;

    bool searching;
    std::future<std::vector<const ContentPiece*>> search_future;
    std::vector<const ContentPiece*> search_results;
};

} // namespace dnd

template <typename T>
void dnd::AdvancedContentSearch::set_filter(T&& new_filter) {
    filter = std::move(new_filter);
}

#endif // ADVANCED_CONTENT_SEARCH_HPP_
