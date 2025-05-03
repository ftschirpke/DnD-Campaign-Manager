#ifndef FEATURE_HPP_
#define FEATURE_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <filesystem>
#include <string>

#include <core/models/content_piece.hpp>
#include <core/models/effects/effects.hpp>
#include <core/models/effects_provider/effects_provider.hpp>
#include <core/models/source_info.hpp>
#include <core/text/text.hpp>
#include <core/validation/effects/effects_validation.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class Content;
class ContentVisitor;

class Feature : public ContentPiece, public EffectsProvider {
public:
    struct Data;

    static CreateResult<Feature> create_for(Data&& data, const Content& content);

    Feature(const Feature&) = delete;
    Feature& operator=(const Feature&) = delete;
    Feature(Feature&&) noexcept = default;
    Feature& operator=(Feature&&) noexcept = default;

    const std::string& get_name() const override;
    const Text& get_description() const override;
    const SourceInfo& get_source_info() const override;
    const Effects& get_main_effects() const override;

    virtual void accept_visitor(ContentVisitor& visitor) const override;
protected:
    Feature(
        std::string&& name, Text&& description, std::filesystem::path&& source_path, std::string&& source_name,
        Effects&& main_effects
    );
private:
    std::string name;
    Text description;
    SourceInfo source_info;
    Effects main_effects;
};

struct Feature::Data : public ValidationData {
    std::strong_ordering operator<=>(const Data&) const = default;

    Effects::Data main_effects_data;
};

} // namespace dnd

#endif // FEATURE_HPP_
