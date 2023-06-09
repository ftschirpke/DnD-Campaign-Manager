#include <dnd_config.hpp>

#include "character_subrace.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <core/content_visitors/content_visitor.hpp>
#include <core/models/effect_holder/feature.hpp>
#include <core/models/feature_holder.hpp>

dnd::CharacterSubrace::CharacterSubrace(
    const std::string& name, const std::filesystem::path& source_path, std::vector<Feature>&& features,
    const std::string& race_name
) noexcept
    : FeatureHolder(name, source_path, std::move(features)), race_name(race_name) {}

void dnd::CharacterSubrace::accept(ContentVisitor* visitor) const { visitor->visit(this); }
