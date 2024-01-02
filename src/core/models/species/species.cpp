#include <dnd_config.hpp>

#include "species.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/source_info.hpp>
#include <core/validation/species/species_data.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

CreateResult<Species> Species::create_for(Data&& data, const Content& content) {
    Errors errors = validate_species_nonrecursively_for_content(data, content);
    if (!errors.ok()) {
        return InvalidCreate<Species>(std::move(data), std::move(errors));
    }
    std::vector<Feature> features;
    features.reserve(data.features_data.size());
    for (Feature::Data& feature_data : data.features_data) {
        CreateResult<Feature> feature_result = Feature::create_for(std::move(feature_data), content);
        if (!feature_result.is_valid()) {
            auto [_, sub_errors] = feature_result.data_and_errors();
            return InvalidCreate<Species>(std::move(data), std::move(sub_errors));
        }
        features.emplace_back(feature_result.value());
    }
    return ValidCreate(Species(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(features),
        data.subspecies
    ));
}

const std::string& Species::get_name() const noexcept { return name; }

const std::string& Species::get_description() const noexcept { return description; }

const SourceInfo& Species::get_source_info() const noexcept { return source_info; }

const std::vector<Feature>& Species::get_features() const noexcept { return features; }

bool Species::has_subspecies() const noexcept { return subspecies; }

void Species::accept_visitor(ContentVisitor& visitor) const { visitor(*this); }

Species::Species(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, std::vector<Feature>&& features,
    bool has_subspecies
) noexcept
    : name(std::move(name)), description(std::move(description)), source_info(std::move(source_path)),
      features(std::move(features)), subspecies(has_subspecies) {}

} // namespace dnd
