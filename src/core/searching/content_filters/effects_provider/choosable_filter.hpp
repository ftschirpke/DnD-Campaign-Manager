#ifndef CHOOSABLE_FILTER_HPP_
#define CHOOSABLE_FILTER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/models/effects_provider/choosable.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

class ChoosableFilter : public ContentPieceFilter {
public:
    /**
     * @brief Determines whether all filters are set
     * @return "true" if all filters are set, "false" otherwise
     */
    bool has_all_filters() const noexcept override;

    /**
     * @brief Determines whether a choosable matches the filter
     * @param content the choosable to check
     * @return "true" if the choosable matches the filter, "false" otherwise
     */
    bool matches(const Choosable& choosable) const noexcept;

    /**
     * @brief Get all content pieces that match the filter
     * @param content the content to search through
     * @return a vector of pointers to the content pieces that match the filter
     */
    std::vector<const ContentPiece*> all_matches(const Content& content) const override;

    /**
     * @brief Clears all filter settings
     */
    void clear() override;

    StringFilter type_filter;
    BoolFilter has_prerequisites_filter;
};

} // namespace dnd

#endif // CHOOSABLE_FILTER_HPP_
