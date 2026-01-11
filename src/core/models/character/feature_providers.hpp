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

    Id get_species_id() const;
    Opt<Id> get_subspecies_id() const;
    Id get_class_id() const;
    Opt<Id> get_subclass_id() const;

    bool has_subspecies() const;
    bool has_subclass() const;
private:
    FeatureProviders(Id species_id, Opt<Id> subspecies_id, Id class_id, Opt<Id> subclass_id);

    Id species_id;
    Opt<Id> subspecies_id;
    Id class_id;
    Opt<Id> subclass_id;
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
