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
    /**
     * @brief Constructs a feature from the given data and content
     * @param data the data to construct the feature from
     * @param content the content to use for the construction
     * @return the constructed feature
     * @throws dnd::invalid_data if the given data is invalid or is incompatible with the given content
     */
    static Feature create(FeatureData&& data, const Content& content);

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
