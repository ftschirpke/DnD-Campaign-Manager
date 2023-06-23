#ifndef FEATURE_HOLDER_HPP_
#define FEATURE_HOLDER_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>
#include <vector>

#include <core/models/content_piece.hpp>
#include <core/models/feature/feature.hpp>

namespace dnd {

class FeatureHolder : public ContentPiece {
public:
    FeatureHolder(const FeatureHolder&) = delete;
    FeatureHolder& operator=(const FeatureHolder&) = delete;
    FeatureHolder(FeatureHolder&&) = default;
    FeatureHolder& operator=(FeatureHolder&&) = default;

    const std::vector<Feature>& get_features() const noexcept;
protected:
    FeatureHolder(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path,
        std::vector<Feature>&& features
    ) noexcept;
private:
    std::vector<Feature> features;
};

} // namespace dnd

#endif // FEATURE_HOLDER_HPP_
