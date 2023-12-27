#ifndef ADVANCED_CONTENT_SEARCH_HPP_
#define ADVANCED_CONTENT_SEARCH_HPP_

#include <dnd_config.hpp>

#include <future>
#include <variant>
#include <vector>

#include <core/content.hpp>
#include <core/searching/content_filters/character/character_filter.hpp>
#include <core/searching/content_filters/class/class_filter.hpp>
#include <core/searching/content_filters/content_filter.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>
#include <core/searching/content_filters/effects_provider/choosable_filter.hpp>
#include <core/searching/content_filters/effects_provider/feature_filter.hpp>
#include <core/searching/content_filters/item/item_filter.hpp>
#include <core/searching/content_filters/species/species_filter.hpp>
#include <core/searching/content_filters/spell/spell_filter.hpp>
#include <core/searching/content_filters/subclass/subclass_filter.hpp>
#include <core/searching/content_filters/subspecies/subspecies_filter.hpp>

namespace dnd {

using ContentFilterVariant = std::variant<
    ContentPieceFilter, CharacterFilter, ClassFilter, SubclassFilter, SpeciesFilter, SubspeciesFilter, ItemFilter,
    SpellFilter, FeatureFilter, ChoosableFilter>;

/**
 * @brief A class representing an advanced content search using a content filter.
 */
class AdvancedContentSearch {
public:
    AdvancedContentSearch(const Content& content) noexcept;
    template <typename T>
    void set_filter(T&& new_filter);
    ContentFilterVariant& get_filter();
    const std::vector<const ContentPiece*>& get_search_results() const noexcept;

    void start_searching();
    /**
     * @brief Check if the search results are available and store them if they are
     * @return true if the search results are available, false otherwise
     * @throws std::exception if any exception is thrown by the search thread
     */
    bool search_results_available();
private:
    const Content& content;
    ContentFilterVariant filter;
    bool searching;
    std::future<std::vector<const ContentPiece*>> search_future;
    std::vector<const ContentPiece*> search_results;
};

} // namespace dnd

template <typename T>
void AdvancedContentSearch::set_filter(T&& new_filter) {
    filter = std::move(new_filter);
}

#endif // ADVANCED_CONTENT_SEARCH_HPP_
