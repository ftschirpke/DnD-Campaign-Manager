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
#include <core/models/source_info.hpp>
#include <core/visitors/content/content_visitor.hpp>

dnd::Feature dnd::Feature::create(dnd::FeatureData&& data, const dnd::Content& content) {
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

const std::string& dnd::Feature::get_name() const noexcept { return name; }

const std::string& dnd::Feature::get_description() const noexcept { return description; }

const dnd::SourceInfo& dnd::Feature::get_source_info() const noexcept { return source_info; }

const dnd::EffectHolder& dnd::Feature::get_main_part() const noexcept { return main_part; }

const std::vector<dnd::EffectHolder>& dnd::Feature::get_other_parts() const noexcept { return other_parts; }

void dnd::Feature::accept(dnd::ContentVisitor& visitor) const { visitor.visit(*this); }

dnd::Feature::Feature(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, EffectHolder&& main_part,
    std::vector<EffectHolder>&& other_parts
) noexcept
    : name(std::move(name)), description(std::move(description)), source_info(std::move(source_path)),
      main_part(std::move(main_part)), other_parts(std::move(other_parts)) {}
