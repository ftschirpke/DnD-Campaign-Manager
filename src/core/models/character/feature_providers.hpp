#ifndef BASIS_HPP_
#define BASIS_HPP_

#include <dnd_config.hpp>

#include <functional>
#include <vector>

#include <core/utils/types.hpp>
#include <core/validation/character/feature_providers_data.hpp>

namespace dnd {

class Content;
class Species;
class Subspecies;
class Class;
class Subclass;

class FeatureProviders {
public:
    using Data = FeatureProvidersData;
    /**
     * @brief Constructs a FeatureProviders object
     * @param data the data to construct the FeatureProviders object from
     * @param content the content to use for the construction
     * @return the constructed FeatureProviders object
     * @throws invalid_data if the given data is invalid or is incompatible with the given content
     */
    static FeatureProviders create(Data&& data, const Content& content);

    const Species& get_species() const noexcept;
    OptCRef<Subspecies> get_subspecies() const noexcept;
    const Class& get_class() const noexcept;
    OptCRef<Subclass> get_subclass() const noexcept;

    bool has_subspecies() const noexcept;
    bool has_subclass() const noexcept;
private:
    FeatureProviders(
        const Species& species, OptCRef<Subspecies> subspecies, const Class& cls, OptCRef<Subclass> subclass
    ) noexcept;

    std::reference_wrapper<const Species> species;
    OptCRef<Subspecies> subspecies;
    std::reference_wrapper<const Class> cls;
    OptCRef<Subclass> subclass;
};

} // namespace dnd

#endif // BASIS_HPP_
