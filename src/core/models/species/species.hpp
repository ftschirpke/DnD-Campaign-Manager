#ifndef SPECIES_HPP_
#define SPECIES_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>
#include <vector>

#include <core/models/content_piece.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/source_info.hpp>
#include <core/utils/data_result.hpp>
#include <core/validation/species/species_data.hpp>

namespace dnd {

class Content;
class ContentVisitor;

class Species : public ContentPiece {
public:
    using Data = SpeciesData;

    static CreateResult<Species> create_for(Data&& data, const Content& content);

    Species(const Species&) = delete;
    Species& operator=(const Species&) = delete;
    Species(Species&&) = default;
    Species& operator=(Species&&) = default;

    const std::string& get_name() const noexcept override;
    const std::string& get_description() const noexcept override;
    const SourceInfo& get_source_info() const noexcept override;
    const std::vector<Feature>& get_features() const noexcept;
    bool has_subspecies() const noexcept;

    virtual void accept_visitor(ContentVisitor& visitor) const override final;
private:
    Species(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path,
        std::vector<Feature>&& features, bool has_subspecies
    ) noexcept;

    std::string name;
    std::string description;
    SourceInfo source_info;
    std::vector<Feature> features;
    bool subspecies;
};

} // namespace dnd

#endif // SPECIES_HPP_
