#include <dnd_config.hpp>

#include "class_feature.hpp"

#include <filesystem>
#include <map>
#include <string>
#include <utility>

#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/effects/effects.hpp>
#include <core/models/source_info.hpp>
#include <core/validation/effects_provider/class_feature_validation.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

CreateResult<ClassFeature> ClassFeature::create_for(Data&& data, const Content& content) {
    Errors errors = validate_class_feature_nonrecursively(data);
    if (!errors.ok()) {
        return InvalidCreate<ClassFeature>(std::move(data), std::move(errors));
    }

    CreateResult<Effects> main_effects_result = Effects::create_for(std::move(data.main_effects_data), content);
    if (!main_effects_result.is_valid()) {
        auto [_, sub_errors] = main_effects_result.data_and_errors();
        return InvalidCreate<ClassFeature>(std::move(data), std::move(sub_errors));
    }
    Effects main_effects = main_effects_result.value();

    if (data.higher_level_effects_data.empty()) {
        return ValidCreate(ClassFeature(
            std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(data.source_name),
            data.level, std::move(main_effects)
        ));
    }

    std::map<int, Effects> higher_level_effects;
    for (auto& [level, effects_data] : data.higher_level_effects_data) {
        CreateResult<Effects> effects_result = Effects::create_for(std::move(effects_data), content);
        if (!effects_result.is_valid()) {
            auto [_, sub_errors] = effects_result.data_and_errors();
            return InvalidCreate<ClassFeature>(std::move(data), std::move(sub_errors));
        }
        higher_level_effects.emplace(level, effects_result.value());
    }
    return ValidCreate(ClassFeature(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(data.source_name),
        data.level, std::move(main_effects), std::move(higher_level_effects)
    ));
}

std::string ClassFeature::key(
    const std::string& name, const std::string& source_name, const std::filesystem::path& source_path, int level
) {
    // TODO: make this key independent of the filesystem path
    return std::format("{}|{}|{}|{}", name, source_name, source_path.stem().string(), level);
}

int ClassFeature::get_level() const { return level; }

const std::map<int, Effects>& ClassFeature::get_higher_level_effects() const { return higher_level_effects; }

std::string ClassFeature::get_key() const {
    return key(get_name(), get_source_info().name, get_source_info().path, level);
}

void ClassFeature::accept_visitor(ContentVisitor& visitor) const { visitor(*this); }

std::string ClassFeature::Data::key(
    const std::string& name, const std::string& source_name, const std::filesystem::path& source_path, int level
) {
    return ClassFeature::key(name, source_name, source_path, level);
}

ClassFeature::ClassFeature(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, std::string&& source_name,
    int level, Effects&& main_effects, std::map<int, Effects>&& higher_level_effects
)
    : Feature(
          std::move(name), std::move(description), std::move(source_path), std::move(source_name),
          std::move(main_effects)
      ),
      level(level), higher_level_effects(std::move(higher_level_effects)) {}

std::string ClassFeature::Data::get_key() const { return key(name, source_name, source_path, level); }

} // namespace dnd
