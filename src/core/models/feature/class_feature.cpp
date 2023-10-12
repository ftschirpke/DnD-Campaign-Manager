#include <dnd_config.hpp>

#include "class_feature.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/effect_holder/effect_holder.hpp>
#include <core/models/source_info.hpp>
#include <core/visitors/content/content_visitor.hpp>

dnd::ClassFeature dnd::ClassFeature::create(dnd::FeatureData&& data, const dnd::Content& content) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create feature from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw invalid_data("ClassFeature data is incompatible with the given content.");
    }

    EffectHolder main_part = EffectHolder::create(std::move(data.main_part_data), content);

    int level = 0; // TODO

    if (data.other_parts_data.empty()) {
        return ClassFeature(
            std::move(data.name), std::move(data.description), std::move(data.source_path), level, std::move(main_part)
        );
    }

    std::vector<EffectHolder> higher_level_parts;
    higher_level_parts.reserve(data.other_parts_data.size());
    for (auto& other_part_data : data.other_parts_data) {
        higher_level_parts.emplace_back(EffectHolder::create(std::move(other_part_data), content));
    }
    return ClassFeature(
        std::move(data.name), std::move(data.description), std::move(data.source_path), level, std::move(main_part),
        std::move(higher_level_parts)
    );
}

int dnd::ClassFeature::get_level() const noexcept { return level; }

const std::vector<dnd::EffectHolder>& dnd::ClassFeature::get_higher_level_parts() const noexcept {
    return higher_level_parts;
}

void dnd::ClassFeature::accept(dnd::ContentVisitor& visitor) const { visitor.visit(*this); }

dnd::ClassFeature::ClassFeature(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, int level,
    EffectHolder&& main_part, std::vector<EffectHolder>&& higher_level_parts
) noexcept
    : Feature(std::move(name), std::move(description), std::move(source_path), std::move(main_part)), level(level),
      higher_level_parts(std::move(higher_level_parts)) {}
