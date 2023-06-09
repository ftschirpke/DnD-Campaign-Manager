#include <dnd_config.hpp>

#include "character_subclass.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <core/content_visitors/content_visitor.hpp>
#include <core/models/effect_holder/feature.hpp>
#include <core/models/feature_holder.hpp>

dnd::CharacterSubclass::CharacterSubclass(
    const std::string& name, const std::filesystem::path& source_path, std::vector<dnd::Feature>&& features,
    const std::string& class_name
) noexcept
    : FeatureHolder(name, source_path, std::move(features)), class_name(class_name) {}

void dnd::CharacterSubclass::accept(dnd::ContentVisitor* visitor) const { visitor->visit(this); }
