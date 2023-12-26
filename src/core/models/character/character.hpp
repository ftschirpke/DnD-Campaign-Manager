#ifndef HPP_
#define HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>

#include <core/models/character/ability_scores.hpp>
#include <core/models/character/decision/decision.hpp>
#include <core/models/character/feature_providers.hpp>
#include <core/models/character/progression.hpp>
#include <core/models/class/class.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/source_info.hpp>
#include <core/models/species/species.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/validation/character/character_data.hpp>

namespace dnd {

class Content;
class ContentVisitor;

class Character : public ContentPiece {
public:
    /**
     * @brief Constructs a character from the given data and content
     * @param data the data to construct the character from
     * @param content the content to use for the construction
     * @return the constructed character
     * @throws dnd::invalid_data if the given data is invalid or is incompatible with the given content
     */
    static Character create(CharacterData&& data, const Content& content);

    Character(const Character&) = delete;
    Character& operator=(const Character&) = delete;
    Character(Character&&) = default;
    Character& operator=(Character&&) = default;

    const std::string& get_name() const noexcept override;
    const std::string& get_description() const noexcept override;
    const SourceInfo& get_source_info() const noexcept override;
    const std::vector<Feature>& get_features() const noexcept;
    const std::vector<Choosable>& get_choosables() const noexcept;
    const AbilityScores& get_base_ability_scores() const noexcept;
    const FeatureProviders& get_feature_providers() const noexcept;
    const Progression& get_progression() const noexcept;

    int get_proficiency_bonus() const noexcept;

    void for_all_effects_do(std::function<void(const Effects&)> func) const noexcept;

    virtual void accept_visitor(ContentVisitor& visitor) const override final;
private:
    Character(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path,
        std::vector<Feature>&& features, AbilityScores&& base_ability_scores, FeatureProviders&& feature_providers,
        Progression&& progression, std::vector<Decision>&& decisions
    ) noexcept;

    std::string name;
    std::string description;
    SourceInfo source_info;
    std::vector<Feature> features;
    std::vector<Choosable> choosables;
    AbilityScores base_ability_scores;
    FeatureProviders feature_providers;
    Progression progression;
    std::vector<Decision> decisions;
};

} // namespace dnd

#endif // HPP_
