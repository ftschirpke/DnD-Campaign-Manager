#ifndef SUBSPECIES_HPP_
#define SUBSPECIES_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>
#include <vector>

#include <core/models/content_piece.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/source_info.hpp>
#include <core/models/species/species.hpp>
#include <core/utils/data_result.hpp>
#include <core/validation/subspecies/subspecies_data.hpp>

namespace dnd {

class Content;
class ContentVisitor;

class Subspecies : public ContentPiece {
public:
    using Data = SubspeciesData;

    static CreateResult<Subspecies> create_for(Data&& data, const Content& content);

    Subspecies(const Subspecies&) = delete;
    Subspecies& operator=(const Subspecies&) = delete;
    Subspecies(Subspecies&&) = default;
    Subspecies& operator=(Subspecies&&) = default;

    const std::string& get_name() const noexcept override;
    const std::string& get_description() const noexcept override;
    const SourceInfo& get_source_info() const noexcept override;
    const std::vector<Feature>& get_features() const noexcept;
    const Species* get_species() const noexcept;

    virtual void accept_visitor(ContentVisitor& visitor) const override final;
private:
    Subspecies(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path,
        std::vector<Feature>&& features, const Species* species
    ) noexcept;

    std::string name;
    std::string description;
    SourceInfo source_info;
    std::vector<Feature> features;
    const Species* species;
};

} // namespace dnd

#endif // SUBSPECIES_HPP_
