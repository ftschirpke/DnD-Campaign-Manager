#ifndef CHARACTER_SUBCLASS_HPP_
#define CHARACTER_SUBCLASS_HPP_

#include "dnd_config.hpp"

#include <string>
#include <utility>
#include <vector>

#include "models/effect_holder/feature.hpp"
#include "models/feature_holder.hpp"

namespace dnd {

/**
 * @brief A class representing a character's subclass
 */
class CharacterSubclass : public FeatureHolder {
public:
    /**
     * @brief Constructs a CharacterSubclass
     * @param name the name of the subclass
     * @param features the collection of features this subclass provides to a character
     * @param class_name the name of the class this is a subclass of
     */
    CharacterSubclass(
        const std::string& name, const std::filesystem::path& source_file_path, std::vector<Feature>&& features,
        const std::string& class_name
    ) noexcept;
    /**
     * @brief Accepts a visitor
     * @param visitor pointer to the visitor
     */
    virtual void accept(Visitor* visitor) const override final;

    // the name of the class this is a subclass of
    const std::string class_name;
};

inline CharacterSubclass::CharacterSubclass(
    const std::string& name, const std::filesystem::path& source_file_path, std::vector<Feature>&& features,
    const std::string& class_name
) noexcept
    : FeatureHolder(name, source_file_path, std::move(features)), class_name(class_name) {}

inline void CharacterSubclass::accept(Visitor* visitor) const { visitor->visit(this); }

} // namespace dnd

#endif // CHARACTER_SUBCLASS_HPP_
