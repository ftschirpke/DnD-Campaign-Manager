#include <dnd_config.hpp>

#include "class_feature.hpp"

#include <filesystem>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/effects/effects.hpp>
#include <core/models/source_info.hpp>
#include <core/validation/effects_provider/class_feature_data.hpp>
#include <core/visitors/content/content_visitor.hpp>

dnd::ClassFeature dnd::ClassFeature::create(dnd::ClassFeatureData&& data, const dnd::Content& content) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create class feature from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw invalid_data("ClassFeature data is incompatible with the given content.");
    }

    Effects main_effects = Effects::create(std::move(data.main_effects_data), content);

    if (data.higher_level_effects_data.empty()) {
        return ClassFeature(
            std::move(data.name), std::move(data.description), std::move(data.source_path), data.level,
            std::move(main_effects)
        );
    }

    std::map<int, Effects> higher_level_effects;
    for (auto& [level, effects_data] : data.higher_level_effects_data) {
        higher_level_effects.emplace(level, Effects::create(std::move(effects_data), content));
    }
    return ClassFeature(
        std::move(data.name), std::move(data.description), std::move(data.source_path), data.level,
        std::move(main_effects), std::move(higher_level_effects)
    );
}

int dnd::ClassFeature::get_level() const noexcept { return level; }

const std::map<int, dnd::Effects>& dnd::ClassFeature::get_higher_level_effects() const noexcept {
    return higher_level_effects;
}

void dnd::ClassFeature::accept_visitor(dnd::ContentVisitor& visitor) const { visitor(*this); }

dnd::ClassFeature::ClassFeature(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, int level,
    Effects&& main_effects, std::map<int, Effects>&& higher_level_effects
) noexcept
    : Feature(std::move(name), std::move(description), std::move(source_path), std::move(main_effects)), level(level),
      higher_level_effects(std::move(higher_level_effects)) {}
