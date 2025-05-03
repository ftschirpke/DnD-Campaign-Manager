#ifndef BASIS_HPP_
#define BASIS_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <string>

#include <core/data_result.hpp>
#include <core/types.hpp>

namespace dnd {

class Content;
class Species;
class Subspecies;
class Class;
class Subclass;

class FeatureProviders {
public:
    struct Data;

    static CreateResult<FeatureProviders> create_for(Data&& data, const Content& content);

    const Species& get_species() const;
    OptCRef<Subspecies> get_subspecies() const;
    const Class& get_class() const;
    OptCRef<Subclass> get_subclass() const;

    bool has_subspecies() const;
    bool has_subclass() const;
private:
    FeatureProviders(
        const Species& species, OptCRef<Subspecies> subspecies, const Class& cls, OptCRef<Subclass> subclass
    );

    CRef<Species> species;
    OptCRef<Subspecies> subspecies;
    CRef<Class> cls;
    OptCRef<Subclass> subclass;
};

struct FeatureProviders::Data {
    std::strong_ordering operator<=>(const Data&) const = default;

    std::string species_key;
    std::string subspecies_key;
    std::string class_key;
    std::string subclass_key;
};


} // namespace dnd

#endif // BASIS_HPP_
