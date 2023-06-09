#ifndef RACE_HPP_
#define RACE_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>
#include <vector>

#include <core/content_visitors/content_visitor.hpp>
#include <core/models/effect_holder/feature.hpp>
#include <core/models/feature_holder.hpp>

namespace dnd {

/**
 * @brief A class representing a character's race.
 */
class CharacterRace : public FeatureHolder {
public:
    /**
     * @brief Constructs a CharacterRace
     * @param name the name of the race
     * @param features a collection of features this race provides to a character
     * @param has_subraces "true" if this race has subraces, "false" otherwise
     */
    CharacterRace(
        const std::string& name, const std::filesystem::path& source_path, std::vector<Feature>&& features,
        const bool has_subraces
    ) noexcept;
    /**
     * @brief Accepts a visitor
     * @param visitor pointer to the visitor
     */
    virtual void accept(ContentVisitor* visitor) const override final;

    // "true" if this race has subraces, "false" otherwise
    const bool has_subraces;
};

} // namespace dnd

#endif // RACE_HPP_
