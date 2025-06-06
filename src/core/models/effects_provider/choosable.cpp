#include <dnd_config.hpp>

#include "choosable.hpp"

#include <memory>
#include <vector>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/effects/condition/condition.hpp>
#include <core/models/effects/condition/condition_factory.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/text/text.hpp>
#include <core/validation/effects_provider/choosable_validation.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

CreateResult<Choosable> Choosable::create_for(Data&& data, const Content& content) {
    Errors errors = validate_choosable_nonrecursively(data);
    if (!errors.ok()) {
        return InvalidCreate<Choosable>(std::move(data), std::move(errors));
    }
    std::vector<std::unique_ptr<Condition>> prerequisites;
    prerequisites.reserve(data.prerequisites_data.size());
    for (Condition::Data& prerequisite_data : data.prerequisites_data) {
        FactoryResult<Condition> prerequisite_result = create_condition(std::move(prerequisite_data));
        if (!prerequisite_result.is_valid()) {
            auto [_, sub_errors] = prerequisite_result.data_and_errors();
            return InvalidCreate<Choosable>(std::move(data), std::move(sub_errors));
        }
        prerequisites.push_back(prerequisite_result.value());
    }

    CreateResult<Effects> main_part_result = Effects::create_for(std::move(data.main_effects_data), content);
    if (!main_part_result.is_valid()) {
        auto [_, sub_errors] = main_part_result.data_and_errors();
        return InvalidCreate<Choosable>(std::move(data), std::move(sub_errors));
    }
    Effects main_part = main_part_result.value();

    return ValidCreate(Choosable(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(data.source_name),
        std::move(data.type), std::move(prerequisites), std::move(main_part)
    ));
}

const std::string& Choosable::get_name() const { return name; }

const Text& Choosable::get_description() const { return description; }

const SourceInfo& Choosable::get_source_info() const { return source_info; }

const Effects& Choosable::get_main_effects() const { return main_effects; }

const std::string& Choosable::get_type() const { return type; }

const std::vector<std::unique_ptr<Condition>>& Choosable::get_prerequisites() const { return prerequisites; }

void Choosable::accept_visitor(ContentVisitor& visitor) const { visitor(*this); }

Choosable::Choosable(
    std::string&& name, Text&& description, std::filesystem::path&& source_path, std::string&& source_name,
    std::string&& type, std::vector<std::unique_ptr<Condition>>&& prerequisites, Effects&& main_effects
)
    : name(std::move(name)), description(std::move(description)),
      source_info({.path = std::move(source_path), .name = std::move(source_name)}),
      main_effects(std::move(main_effects)), type(type), prerequisites(std::move(prerequisites)) {}

} // namespace dnd
