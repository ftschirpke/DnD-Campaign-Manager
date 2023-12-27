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
#include <core/validation/character/feature_providers_data.hpp>

namespace dnd {

FeatureProviders FeatureProviders::create(FeatureProvidersData&& data, const Content& content) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create FeatureProviders object from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw invalid_data("FeatureProviders data is incompatible with the given content.");
    }
    const Species& species = content.get_species().get(data.species_name).value().get();
    OptCRef<Subspecies> subspecies;
    const Class& cls = content.get_classes().get(data.class_name).value().get();
    OptCRef<Subclass> subclass;
    if (!data.subspecies_name.empty()) {
        subspecies = content.get_subspecies().get(data.subspecies_name).value().get();
    }
    if (!data.subclass_name.empty()) {
        subclass = content.get_subclasses().get(data.subclass_name).value().get();
    }
    return FeatureProviders(species, subspecies, cls, subclass);
}

const Species& FeatureProviders::get_species() const noexcept { return species; }

OptCRef<Subspecies> FeatureProviders::get_subspecies() const noexcept { return subspecies; }

const Class& FeatureProviders::get_class() const noexcept { return cls; }

OptCRef<Subclass> FeatureProviders::get_subclass() const noexcept { return subclass; }

bool FeatureProviders::has_subspecies() const noexcept { return subspecies.has_value(); }

bool FeatureProviders::has_subclass() const noexcept { return subclass.has_value(); }

FeatureProviders::FeatureProviders(
    const Species& species, OptCRef<Subspecies> subspecies, const Class& cls, OptCRef<Subclass> subclass
) noexcept
    : species(std::cref(species)), subspecies(subspecies), cls(std::cref(cls)), subclass(subclass) {}

} // namespace dnd