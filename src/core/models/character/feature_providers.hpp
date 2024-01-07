#ifndef BASIS_HPP_
#define BASIS_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <functional>
#include <string>

#include <core/utils/data_result.hpp>
#include <core/utils/types.hpp>

namespace dnd {

class Content;
class Species;
class Subspecies;
class Class;
class Subclass;

class FeatureProviders {
public:
    class Data;

    static CreateResult<FeatureProviders> create_for(Data&& data, const Content& content);

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

class FeatureProviders::Data {
public:
    std::strong_ordering operator<=>(const Data&) const noexcept = default;

    std::string species_name;
    std::string subspecies_name;
    std::string class_name;
    std::string subclass_name;
};


} // namespace dnd

#endif // BASIS_HPP_
