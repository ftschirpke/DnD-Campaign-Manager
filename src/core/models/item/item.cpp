#include <dnd_config.hpp>

#include "item.hpp"

#include <filesystem>
#include <string>
#include <utility>

#include <core/content_visitors/content_visitor.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/content_piece.hpp>

dnd::Item dnd::Item::create(
    const std::string& name, const std::filesystem::path& source_path, const std::string& description,
    const std::string& cosmetic_description, bool requires_attunement
) {
    ItemData item_data{name, source_path, description, cosmetic_description, requires_attunement};
    return create(std::move(item_data));
}

dnd::Item dnd::Item::create(ItemData&& item_data) {
    if (!item_data.validate().ok()) {
        throw invalid_data("Cannot create Item from invalid data.");
    }
    return Item(
        std::move(item_data.name), std::move(item_data.source_path), item_data.requires_attunement,
        std::move(item_data.description), std::move(item_data.cosmetic_description)
    );
}

const std::string& dnd::Item::get_cosmetic_description() const noexcept { return cosmetic_description; }

bool dnd::Item::requires_attunement() const noexcept { return attunement; }

void dnd::Item::accept(ContentVisitor* visitor) const { visitor->visit(this); }

dnd::Item::Item(
    std::string&& name, std::filesystem::path&& source_path, std::string&& description,
    std::string&& cosmetic_description, bool requires_attunement
) noexcept
    : ContentPiece(std::move(name), std::move(description), std::move(source_path)),
      cosmetic_description(std::move(cosmetic_description)), attunement(requires_attunement) {}
