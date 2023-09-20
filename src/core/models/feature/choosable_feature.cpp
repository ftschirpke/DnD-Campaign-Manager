#include <dnd_config.hpp>

#include "choosable_feature.hpp"

#include <memory>
#include <vector>

#include <core/content.hpp>
#include <core/content_visitors/content_visitor.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/effect_holder/condition/condition.hpp>
#include <core/models/effect_holder/condition/condition_factory.hpp>
#include <core/models/feature/feature.hpp>
#include <core/validation/feature/choosable_feature_data.hpp>

dnd::ChoosableFeature dnd::ChoosableFeature::create(dnd::ChoosableFeatureData&& data, const dnd::Content& content) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create choosable feature from invalid data");
    }
    if (!data.validate_relations(content).ok()) {
        throw invalid_data("Choosable feature data is incompatible with the given content");
    }
    std::vector<std::unique_ptr<Condition>> prerequisites;
    prerequisites.reserve(data.prerequisites_data.size());
    for (auto& prerequisite_data : data.prerequisites_data) {
        prerequisites.emplace_back(create_condition(std::move(prerequisite_data)));
    }

    EffectHolder main_part = EffectHolder::create(std::move(data.main_part_data), content);
    if (data.other_parts_data.empty()) {
        return ChoosableFeature(
            std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(data.type),
            std::move(prerequisites), std::move(main_part)
        );
    }

    std::vector<EffectHolder> other_parts;
    other_parts.reserve(data.other_parts_data.size());
    for (auto& other_part_data : data.other_parts_data) {
        other_parts.emplace_back(EffectHolder::create(std::move(other_part_data), content));
    }

    return ChoosableFeature(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(data.type),
        std::move(prerequisites), std::move(main_part), std::move(other_parts)
    );
}

const std::string& dnd::ChoosableFeature::get_type() const noexcept { return type; }

const std::vector<std::unique_ptr<dnd::Condition>>& dnd::ChoosableFeature::get_prerequisites() const noexcept {
    return prerequisites;
}

void dnd::ChoosableFeature::accept(dnd::ContentVisitor* visitor) const { visitor->visit(this); }

dnd::ChoosableFeature::ChoosableFeature(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, std::string&& type,
    std::vector<std::unique_ptr<Condition>>&& prerequisites, EffectHolder&& main_part,
    std::vector<EffectHolder>&& other_parts
) noexcept
    : Feature(
        std::move(name), std::move(description), std::move(source_path), std::move(main_part), std::move(other_parts)
    ),
      type(type), prerequisites(std::move(prerequisites)) {}
