#include <dnd_config.hpp>

#include "choosable.hpp"

#include <memory>
#include <vector>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/effects/condition/condition.hpp>
#include <core/models/effects/condition/condition_factory.hpp>
#include <core/models/feature/feature.hpp>
#include <core/validation/feature/choosable_data.hpp>
#include <core/visitors/content/content_visitor.hpp>

dnd::Choosable dnd::Choosable::create(dnd::ChoosableData&& data, const dnd::Content& content) {
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

    Effects main_part = Effects::create(std::move(data.main_part_data), content);
    return Choosable(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(data.type),
        std::move(prerequisites), std::move(main_part)
    );
}

const std::string& dnd::Choosable::get_type() const noexcept { return type; }

const std::vector<std::unique_ptr<dnd::Condition>>& dnd::Choosable::get_prerequisites() const noexcept {
    return prerequisites;
}

void dnd::Choosable::accept(dnd::ContentVisitor& visitor) const { visitor.visit(*this); }

dnd::Choosable::Choosable(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, std::string&& type,
    std::vector<std::unique_ptr<Condition>>&& prerequisites, Effects&& main_part
) noexcept
    : Feature(std::move(name), std::move(description), std::move(source_path), std::move(main_part)), type(type),
      prerequisites(std::move(prerequisites)) {}
