#ifndef FEATURE_HOLDER_HPP_
#define FEATURE_HOLDER_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>
#include <vector>

#include <core/models/content_piece.hpp>
#include <core/models/feature/feature.hpp>
#include <core/models/source_info.hpp>

namespace dnd {

class FeatureHolder : public ContentPiece {
public:
    FeatureHolder(const FeatureHolder&) = delete;
    FeatureHolder& operator=(const FeatureHolder&) = delete;
    FeatureHolder(FeatureHolder&&) = default;
    FeatureHolder& operator=(FeatureHolder&&) = default;

    const std::string& get_name() const noexcept override;
    const std::string& get_description() const noexcept override;
    const SourceInfo& get_source_info() const noexcept override;
    const std::vector<Feature>& get_features() const noexcept;
protected:
    FeatureHolder(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path,
        std::vector<Feature>&& features
    ) noexcept;
private:
    std::string name;
    std::string description;
    SourceInfo source_info;
    std::vector<Feature> features;
};

} // namespace dnd

#endif // FEATURE_HOLDER_HPP_
