#include <dnd_config.hpp>

#include "feature_providers.hpp"

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/class/class.hpp>
#include <core/models/species/species.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/types.hpp>
#include <core/validation/character/feature_providers_validation.hpp>

namespace dnd {

CreateResult<FeatureProviders> FeatureProviders::create_for(Data&& data, const Content& content) {
    Errors errors = validate_feature_providers_for_content(data, content);
    if (!errors.ok()) {
        return InvalidCreate<FeatureProviders>(std::move(data), std::move(errors));
    }
    Id species_id = content.find_species(data.species_key).value();
    Opt<Id> subspecies_id = content.find_subspecies(data.subspecies_key);
    Id class_id = content.find_class(data.class_key).value();
    Opt<Id> subclass_id = content.find_subclass(data.subclass_key);
    return ValidCreate(FeatureProviders(species_id, subspecies_id, class_id, subclass_id));
}

Id FeatureProviders::get_species_id() const { return species_id; }

Opt<Id> FeatureProviders::get_subspecies_id() const { return subspecies_id; }

Id FeatureProviders::get_class_id() const { return class_id; }

Opt<Id> FeatureProviders::get_subclass_id() const { return subclass_id; }

bool FeatureProviders::has_subspecies() const { return subspecies_id.has_value(); }

bool FeatureProviders::has_subclass() const { return subclass_id.has_value(); }

FeatureProviders::FeatureProviders(
    Id species_id, Opt<Id> subspecies_id, Id class_id, Opt<Id> subclass_id
)
    : species_id(species_id), subspecies_id(subspecies_id), class_id(class_id), subclass_id(subclass_id) {}

} // namespace dnd
