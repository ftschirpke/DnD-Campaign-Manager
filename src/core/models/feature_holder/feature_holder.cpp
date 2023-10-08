#include <dnd_config.hpp>

#include "feature_holder.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <core/models/content_piece.hpp>
#include <core/models/feature/feature.hpp>
#include <core/models/source_info.hpp>

const std::string& dnd::FeatureHolder::get_name() const noexcept { return name; }

const std::string& dnd::FeatureHolder::get_description() const noexcept { return description; }

const dnd::SourceInfo& dnd::FeatureHolder::get_source_info() const noexcept { return source_info; }

const std::vector<dnd::Feature>& dnd::FeatureHolder::get_features() const noexcept { return features; }

dnd::FeatureHolder::FeatureHolder(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path,
    std::vector<dnd::Feature>&& features
) noexcept
    : name(std::move(name)), description(std::move(description)), source_info(std::move(source_path)),
      features(std::move(features)) {}
