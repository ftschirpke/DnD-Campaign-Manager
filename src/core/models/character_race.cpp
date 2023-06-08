#include <dnd_config.hpp>

#include "character_race.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <core/content_visitors/content_visitor.hpp>
#include <core/models/effect_holder/feature.hpp>
#include <core/models/feature_holder.hpp>

dnd::CharacterRace::CharacterRace(
    const std::string& name, const std::filesystem::path& source_file_path, std::vector<dnd::Feature>&& features,
    const bool has_subraces
) noexcept
    : FeatureHolder(name, source_file_path, std::move(features)), has_subraces(has_subraces) {}

void dnd::CharacterRace::accept(dnd::ContentVisitor* visitor) const { visitor->visit(this); }
