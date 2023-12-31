#ifndef FEATURE_HPP_
#define FEATURE_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <functional>
#include <string>

#include <core/models/content_piece.hpp>
#include <core/models/effects/effects.hpp>
#include <core/models/effects_provider/effects_provider.hpp>
#include <core/models/source_info.hpp>
#include <core/validation/effects_provider/feature_data.hpp>

namespace dnd {

class Content;
class ContentVisitor;

/**
 * @brief A class representing a simple feature provided by a class, subclass, species, subspecies, or character.
 */
class Feature : public ContentPiece, public EffectsProvider {
public:
    using Data = FeatureData;

    static CreateResult<Feature> create_for(Data&& data, const Content& content);

    Feature(const Feature&) = delete;
    Feature& operator=(const Feature&) = delete;
    Feature(Feature&&) = default;
    Feature& operator=(Feature&&) = default;

    const std::string& get_name() const noexcept override;
    const std::string& get_description() const noexcept override;
    const SourceInfo& get_source_info() const noexcept override;
    const Effects& get_main_effects() const noexcept override;

    virtual void accept_visitor(ContentVisitor& visitor) const override;
protected:
    Feature(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path, Effects&& main_effects
    ) noexcept;
private:
    std::string name;
    std::string description;
    SourceInfo source_info;
    Effects main_effects;
};

} // namespace dnd

#endif // FEATURE_HPP_
