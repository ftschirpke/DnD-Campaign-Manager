#include <dnd_config.hpp>

#include "feature_data.hpp"

#include <memory>

#include <core/errors/errors.hpp>
#include <core/validation/effect_holder/effect_holder_data.hpp>
#include <core/validation/validation_data.hpp>

dnd::FeatureData::FeatureData(const ValidationData* parent) noexcept
    : ValidationData(), main_part_data(parent == nullptr ? this : parent), parent(parent) {}

std::unique_ptr<dnd::ValidationData> dnd::FeatureData::pack() const { return std::make_unique<FeatureData>(*this); }

dnd::Errors dnd::FeatureData::validate() const {
    Errors errors = ValidationData::validate();
    errors += main_part_data.validate();
    for (const auto& other_part_data : other_parts_data) {
        errors += other_part_data.validate();
    }
    return errors;
}

dnd::Errors dnd::FeatureData::validate_relations(const ContentHolder& content) const {
    Errors errors = main_part_data.validate_relations(content);
    for (const auto& other_part_data : other_parts_data) {
        errors += other_part_data.validate_relations(content);
    }
    return errors;
}

const dnd::ValidationData* dnd::FeatureData::get_parent() const noexcept { return parent; }
