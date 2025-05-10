#include <dnd_config.hpp>

#include "subclass_feature.hpp"

#include <filesystem>
#include <map>
#include <string>
#include <utility>

#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/effects/effects.hpp>
#include <core/models/source_info.hpp>
#include <core/validation/effects_provider/subclass_feature_validation.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

CreateResult<SubclassFeature> SubclassFeature::create_for(Data&& data, const Content& content) {
    Errors errors = validate_subclass_feature_nonrecursively(data);
    if (!errors.ok()) {
        return InvalidCreate<SubclassFeature>(std::move(data), std::move(errors));
    }

    CreateResult<Effects> main_effects_result = Effects::create_for(std::move(data.main_effects_data), content);
    if (!main_effects_result.is_valid()) {
        auto [_, sub_errors] = main_effects_result.data_and_errors();
        return InvalidCreate<SubclassFeature>(std::move(data), std::move(sub_errors));
    }
    Effects main_effects = main_effects_result.value();

    if (data.higher_level_effects_data.empty()) {
        return ValidCreate(SubclassFeature(
            std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(data.source_name),
            data.level, std::move(main_effects)
        ));
    }

    std::map<int, Effects> higher_level_effects;
    for (auto& [level, effects_data] : data.higher_level_effects_data) {
        CreateResult<Effects> effects_result = Effects::create_for(std::move(effects_data), content);
        if (!effects_result.is_valid()) {
            auto [_, sub_errors] = effects_result.data_and_errors();
            return InvalidCreate<SubclassFeature>(std::move(data), std::move(sub_errors));
        }
        higher_level_effects.emplace(level, effects_result.value());
    }
    return ValidCreate(SubclassFeature(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(data.source_name),
        data.level, std::move(main_effects), std::move(higher_level_effects)
    ));
}

std::string SubclassFeature::key(
    const std::string& name, const std::string& source_name, const std::string& subclass_short_name,
    const std::string& subclass_source_name, int level
) {
    return fmt::format("{}##{}|{}|{}|{}", name, source_name, subclass_short_name, subclass_source_name, level);
}

int SubclassFeature::get_level() const { return level; }

const std::map<int, Effects>& SubclassFeature::get_higher_level_effects() const { return higher_level_effects; }

std::string SubclassFeature::get_key() const {
    return key(get_name(), get_source_info().name, subclass_short_name, subclass_source_name, level);
}

void SubclassFeature::accept_visitor(ContentVisitor& visitor) const { visitor(*this); }

std::string SubclassFeature::Data::key(
    const std::string& name, const std::string& source_name, const std::string& subclass_short_name,
    const std::string& subclass_source_name, int level
) {
    return SubclassFeature::key(name, source_name, subclass_short_name, subclass_source_name, level);
}

SubclassFeature::SubclassFeature(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, std::string&& source_name,
    int level, Effects&& main_effects, std::map<int, Effects>&& higher_level_effects
)
    : Feature(
          std::move(name), std::move(description), std::move(source_path), std::move(source_name),
          std::move(main_effects)
      ),
      level(level), higher_level_effects(std::move(higher_level_effects)) {}

std::string SubclassFeature::Data::get_key() const {
    return key(name, source_name, subclass_short_name, subclass_source_name, level);
}

} // namespace dnd
