#ifndef CHARACTER_SUBRACE_HPP_
#define CHARACTER_SUBRACE_HPP_

#include <dnd_config.hpp>

#include <string>
#include <utility>
#include <vector>

#include <core/content_visitors/content_visitor.hpp>
#include <core/models/effect_holder/feature.hpp>
#include <core/models/feature_holder.hpp>

namespace dnd {

/**
 * @brief A class representing a character's subrace.
 */
class CharacterSubrace : public FeatureHolder {
public:
    /**
     * @brief Constructs a CharacterSubrace
     * @param name the name of the subrace
     * @param features a collection of features this subrace provides to a character
     * @param race_name the name of the race this is a subrace of
     */
    CharacterSubrace(
        const std::string& name, const std::filesystem::path& source_file_path, std::vector<Feature>&& features,
        const std::string& race_name
    ) noexcept;
    /**
     * @brief Accepts a visitor
     * @param visitor pointer to the visitor
     */
    virtual void accept(ContentVisitor* visitor) const override final;

    // the name of the race this is a subrace of
    const std::string race_name;
};

inline CharacterSubrace::CharacterSubrace(
    const std::string& name, const std::filesystem::path& source_file_path, std::vector<Feature>&& features,
    const std::string& race_name
) noexcept
    : FeatureHolder(name, source_file_path, std::move(features)), race_name(race_name) {}

inline void CharacterSubrace::accept(ContentVisitor* visitor) const { visitor->visit(this); }

} // namespace dnd

#endif // CHARACTER_SUBRACE_HPP_
