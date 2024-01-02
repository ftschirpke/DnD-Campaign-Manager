#ifndef SUBSPECIES_HPP_
#define SUBSPECIES_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <core/models/content_piece.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/source_info.hpp>
#include <core/models/species/species.hpp>
#include <core/utils/data_result.hpp>
#include <core/utils/types.hpp>

namespace dnd {

class Content;
class ContentVisitor;

class Subspecies : public ContentPiece {
public:
    class Data;

    static CreateResult<Subspecies> create_for(Data&& data, const Content& content);

    Subspecies(const Subspecies&) = delete;
    Subspecies& operator=(const Subspecies&) = delete;
    Subspecies(Subspecies&&) = default;
    Subspecies& operator=(Subspecies&&) = default;

    const std::string& get_name() const noexcept override;
    const std::string& get_description() const noexcept override;
    const SourceInfo& get_source_info() const noexcept override;
    const std::vector<Feature>& get_features() const noexcept;
    CRef<Species> get_species() const noexcept;

    virtual void accept_visitor(ContentVisitor& visitor) const override final;
private:
    Subspecies(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path,
        std::vector<Feature>&& features, CRef<Species> species
    ) noexcept;

    std::string name;
    std::string description;
    SourceInfo source_info;
    std::vector<Feature> features;
    CRef<Species> species;
};

class Subspecies::Data : public ValidationData {
public:
    std::strong_ordering operator<=>(const Subspecies::Data&) const noexcept = default;

    std::vector<Feature::Data> features_data;
    std::string species_name;
};

} // namespace dnd

#endif // SUBSPECIES_HPP_
