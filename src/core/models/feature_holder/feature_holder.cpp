#include <dnd_config.hpp>

#include "feature_holder.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <core/models/content_piece.hpp>
#include <core/models/feature/feature.hpp>

const std::vector<dnd::Feature>& dnd::FeatureHolder::get_features() const noexcept { return features; }

dnd::FeatureHolder::FeatureHolder(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path,
    std::vector<dnd::Feature>&& features
) noexcept
    : ContentPiece(std::move(name), std::move(description), std::move(source_path)), features(std::move(features)) {}
