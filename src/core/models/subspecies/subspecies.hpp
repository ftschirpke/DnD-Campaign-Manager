#ifndef SUBSPECIES_HPP_
#define SUBSPECIES_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <filesystem>
#include <string>
#include <vector>

#include <core/models/content_piece.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/source_info.hpp>
#include <core/models/species/species.hpp>
#include <core/data_result.hpp>
#include <core/types.hpp>

namespace dnd {

class Content;
class ContentVisitor;

class Subspecies : public ContentPiece {
public:
    struct Data;

    static CreateResult<Subspecies> create_for(Data&& data, const Content& content);

    Subspecies(const Subspecies&) = delete;
    Subspecies& operator=(const Subspecies&) = delete;
    Subspecies(Subspecies&&) noexcept = default;
    Subspecies& operator=(Subspecies&&) noexcept = default;

    const std::string& get_name() const override;
    const std::string& get_description() const override;
    const SourceInfo& get_source_info() const override;
    const std::vector<Feature>& get_features() const;
    CRef<Species> get_species() const;

    virtual void accept_visitor(ContentVisitor& visitor) const override final;
private:
    Subspecies(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path, std::string&& source_name,
        std::vector<Feature>&& features, CRef<Species> species
    );

    std::string name;
    std::string description;
    SourceInfo source_info;
    std::vector<Feature> features;
    CRef<Species> species;
};

struct Subspecies::Data : public ValidationData {
    std::strong_ordering operator<=>(const Subspecies::Data&) const = default;

    std::vector<Feature::Data> features_data;
    std::string species_key;
};

} // namespace dnd

#endif // SUBSPECIES_HPP_
