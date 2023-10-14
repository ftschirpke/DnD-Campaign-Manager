#include <dnd_config.hpp>

#include "item.hpp"

#include <filesystem>
#include <string>
#include <utility>

#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/source_info.hpp>
#include <core/visitors/content/content_visitor.hpp>

dnd::Item dnd::Item::create(ItemData&& item_data) {
    if (!item_data.validate().ok()) {
        throw invalid_data("Cannot create Item from invalid data.");
    }
    return Item(
        std::move(item_data.name), std::move(item_data.description), std::move(item_data.source_path),
        std::move(item_data.cosmetic_description), item_data.requires_attunement
    );
}

const std::string& dnd::Item::get_name() const noexcept { return name; }

const std::string& dnd::Item::get_description() const noexcept { return description; }

const dnd::SourceInfo& dnd::Item::get_source_info() const noexcept { return source_info; }

const std::string& dnd::Item::get_cosmetic_description() const noexcept { return cosmetic_description; }

bool dnd::Item::requires_attunement() const noexcept { return attunement; }

void dnd::Item::accept(ContentVisitor& visitor) const { visitor.visit(*this); }

dnd::Item::Item(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path,
    std::string&& cosmetic_description, bool requires_attunement
) noexcept
    : name(std::move(name)), description(std::move(description)), source_info(std::move(source_path)),
      cosmetic_description(std::move(cosmetic_description)), attunement(requires_attunement) {}
