#include <dnd_config.hpp>

#include "character_class.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <core/basic_mechanics/dice.hpp>
#include <core/content_visitors/content_visitor.hpp>
#include <core/models/effect_holder/feature.hpp>
#include <core/models/feature_holder.hpp>

dnd::CharacterClass::CharacterClass(
    const std::string& name, const std::filesystem::path& source_file_path, std::vector<dnd::Feature>&& features,
    const dnd::Dice hit_dice, const std::vector<int>& asi_levels, int subclass_level
) noexcept
    : FeatureHolder(name, source_file_path, std::move(features)), hit_dice(hit_dice), asi_levels(asi_levels),
      subclass_level(subclass_level) {}

void dnd::CharacterClass::accept(dnd::ContentVisitor* visitor) const { visitor->visit(this); }
