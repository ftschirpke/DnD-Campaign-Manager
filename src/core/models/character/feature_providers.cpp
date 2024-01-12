#include <dnd_config.hpp>

#include "feature_providers.hpp"

#include <functional>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/class/class.hpp>
#include <core/models/species/species.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/validation/character/feature_providers_validation.hpp>

namespace dnd {

CreateResult<FeatureProviders> FeatureProviders::create_for(Data&& data, const Content& content) {
    Errors errors = validate_feature_providers_for_content(data, content);
    if (!errors.ok()) {
        return InvalidCreate<FeatureProviders>(std::move(data), std::move(errors));
    }
    const Species& species = content.get_species().get(data.species_name).value().get();
    OptCRef<Subspecies> subspecies;
    if (!data.subspecies_name.empty()) {
        subspecies = content.get_subspecies().get(data.subspecies_name);
    }
    const Class& cls = content.get_classes().get(data.class_name).value().get();
    OptCRef<Subclass> subclass;
    if (!data.subclass_name.empty()) {
        subclass = content.get_subclasses().get(data.subclass_name);
    }
    return ValidCreate(FeatureProviders(species, subspecies, cls, subclass));
}

const Species& FeatureProviders::get_species() const { return species; }

OptCRef<Subspecies> FeatureProviders::get_subspecies() const { return subspecies; }

const Class& FeatureProviders::get_class() const { return cls; }

OptCRef<Subclass> FeatureProviders::get_subclass() const { return subclass; }

bool FeatureProviders::has_subspecies() const { return subspecies.has_value(); }

bool FeatureProviders::has_subclass() const { return subclass.has_value(); }

FeatureProviders::FeatureProviders(
    const Species& species, OptCRef<Subspecies> subspecies, const Class& cls, OptCRef<Subclass> subclass
)
    : species(std::cref(species)), subspecies(subspecies), cls(std::cref(cls)), subclass(subclass) {}

} // namespace dnd
