#include <dnd_config.hpp>

#include "feature.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/effects/effects.hpp>
#include <core/models/source_info.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

Feature Feature::create_for(Data&& data, const Content& content) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create feature from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw invalid_data("Feature data is incompatible with the given content.");
    }

    Effects main_part = Effects::create_for(std::move(data.main_effects_data), content);
    return Feature(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(main_part)
    );
}

const std::string& Feature::get_name() const noexcept { return name; }

const std::string& Feature::get_description() const noexcept { return description; }

const SourceInfo& Feature::get_source_info() const noexcept { return source_info; }

const Effects& Feature::get_main_effects() const noexcept { return main_effects; }

void Feature::accept_visitor(ContentVisitor& visitor) const { visitor(*this); }

Feature::Feature(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, Effects&& main_effects
) noexcept
    : name(std::move(name)), description(std::move(description)), source_info(std::move(source_path)),
      main_effects(std::move(main_effects)) {}

} // namespace dnd
