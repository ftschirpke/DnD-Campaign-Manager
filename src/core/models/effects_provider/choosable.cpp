#include <dnd_config.hpp>

#include "choosable.hpp"

#include <memory>
#include <vector>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/effects/condition/condition.hpp>
#include <core/models/effects/condition/condition_factory.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/validation/effects_provider/choosable_data.hpp>
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

    Effects main_part = Effects::create(std::move(data.main_effects_data), content);
    return Choosable(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(data.type),
        std::move(prerequisites), std::move(main_part)
    );
}

const std::string& dnd::Choosable::get_name() const noexcept { return name; }

const std::string& dnd::Choosable::get_description() const noexcept { return description; }

const dnd::SourceInfo& dnd::Choosable::get_source_info() const noexcept { return source_info; }

const dnd::Effects& dnd::Choosable::get_main_effects() const noexcept { return main_effects; }

std::vector<const dnd::Effects*> dnd::Choosable::get_all_effects() const {
    return std::vector<const Effects*>{&main_effects};
}

const std::string& dnd::Choosable::get_type() const noexcept { return type; }

const std::vector<std::unique_ptr<dnd::Condition>>& dnd::Choosable::get_prerequisites() const noexcept {
    return prerequisites;
}

void dnd::Choosable::accept(dnd::ContentVisitor& visitor) const { visitor.visit(*this); }

dnd::Choosable::Choosable(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, std::string&& type,
    std::vector<std::unique_ptr<Condition>>&& prerequisites, Effects&& main_effects
) noexcept
    : name(std::move(name)), description(std::move(description)), source_info(std::move(source_path)),
      main_effects(std::move(main_effects)), type(type), prerequisites(std::move(prerequisites)) {}
