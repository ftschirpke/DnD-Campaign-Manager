#include <dnd_config.hpp>

#include "item.hpp"

#include <filesystem>
#include <string>
#include <utility>

#include <core/content_visitors/content_visitor.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/content_piece.hpp>

dnd::Item dnd::Item::create(
    const std::string& name, const std::filesystem::path& source_path, bool requires_attunement,
    const std::string& description, const std::string& cosmetic_description
) {
    return create(ItemData{name, source_path, requires_attunement, description, cosmetic_description});
}

dnd::Item dnd::Item::create(ItemData&& item_data) {
    if (!item_data.validate().ok()) {
        throw invalid_data("Item cannot be created from invalid data.");
    }
    return Item(
        std::move(item_data.name), std::move(item_data.source_path), item_data.requires_attunement,
        std::move(item_data.description), std::move(item_data.cosmetic_description)
    );
}

void dnd::Item::accept(ContentVisitor* visitor) const { visitor->visit(this); }

dnd::Item::Item(
    std::string&& name, std::filesystem::path&& source_path, bool requires_attunement, std::string&& description,
    std::string&& cosmetic_description
) noexcept
    : ContentPiece(std::move(name), std::move(description), std::move(source_path)),
      cosmetic_description(std::move(cosmetic_description)), requires_attunement(requires_attunement) {}
