#ifndef SPECIES_HPP_
#define SPECIES_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <filesystem>
#include <string>
#include <vector>

#include <core/data_result.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/source_info.hpp>
#include <core/text/text.hpp>

namespace dnd {

class Content;
class ContentVisitor;

class Species : public ContentPiece {
public:
    struct Data;

    static CreateResult<Species> create_for(Data&& data, const Content& content);

    Species(const Species&) = delete;
    Species& operator=(const Species&) = delete;
    Species(Species&&) noexcept = default;
    Species& operator=(Species&&) noexcept = default;

    const std::string& get_name() const override;
    const Text& get_description() const override;
    const SourceInfo& get_source_info() const override;
    const std::string& get_key() const override;
    const std::vector<Feature>& get_features() const;
private:
    Species(
        std::string&& name, Text&& description, std::filesystem::path&& source_path, std::string&& source_name,
        std::string&& key, std::vector<Feature>&& features
    );

    std::string name;
    Text description;
    SourceInfo source_info;
    std::string key;
    std::vector<Feature> features;
};

struct Species::Data : public ValidationData {
    std::strong_ordering operator<=>(const Data&) const = default;

    std::vector<Feature::Data> features_data;
};

} // namespace dnd

#endif // SPECIES_HPP_
