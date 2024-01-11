#include <dnd_config.hpp>

#include "item.hpp"

#include <filesystem>
#include <string>
#include <utility>

#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/source_info.hpp>
#include <core/utils/data_result.hpp>
#include <core/validation/item/item_validation.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

CreateResult<Item> Item::create(Data&& data) {
    Errors errors = validate_item(data);
    if (!errors.ok()) {
        return InvalidCreate<Item>(std::move(data), std::move(errors));
    }
    return ValidCreate(Item(
        std::move(data.name), std::move(data.description), std::move(data.source_path),
        std::move(data.cosmetic_description), data.requires_attunement
    ));
}

const std::string& Item::get_name() const noexcept { return name; }

const std::string& Item::get_description() const noexcept { return description; }

const SourceInfo& Item::get_source_info() const noexcept { return source_info; }

const std::string& Item::get_cosmetic_description() const noexcept { return cosmetic_description; }

bool Item::requires_attunement() const noexcept { return attunement; }

void Item::accept_visitor(ContentVisitor& visitor) const { visitor(*this); }

Item::Item(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path,
    std::string&& cosmetic_description, bool requires_attunement
) noexcept
    : name(std::move(name)), description(std::move(description)), source_info(std::move(source_path)),
      cosmetic_description(std::move(cosmetic_description)), attunement(requires_attunement) {}

} // namespace dnd
