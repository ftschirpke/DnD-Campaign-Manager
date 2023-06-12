#include <dnd_config.hpp>

#include "feature.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/effect_holder/effect_holder.hpp>

dnd::Feature dnd::Feature::create(dnd::FeatureData&& data, const dnd::ContentHolder* content) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create feature from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw invalid_data("Feature data is incompatible with the given content.");
    }

    EffectHolder main_part = EffectHolder::create(std::move(data.main_part_data), content);
    if (data.other_parts_data.empty()) {
        return Feature(
            std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(main_part)
        );
    }

    std::vector<EffectHolder> other_parts;
    other_parts.reserve(data.other_parts_data.size());
    for (auto& other_part_data : data.other_parts_data) {
        other_parts.emplace_back(EffectHolder::create(std::move(other_part_data), content));
    }
    return Feature(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(main_part),
        std::move(other_parts)
    );
}

const dnd::EffectHolder& dnd::Feature::get_main_part() const noexcept { return main_part; }

const std::vector<dnd::EffectHolder>& dnd::Feature::get_other_parts() const noexcept { return other_parts; }

void dnd::Feature::accept(dnd::ContentVisitor* visitor) const { visitor->visit(this); }

dnd::Feature::Feature(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, EffectHolder&& main_part,
    std::vector<EffectHolder>&& other_parts
) noexcept
    : ContentPiece(name, description, source_path), main_part(std::move(main_part)),
      other_parts(std::move(other_parts)) {}
