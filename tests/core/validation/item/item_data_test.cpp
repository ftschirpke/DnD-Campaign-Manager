#include <dnd_config.hpp>

#include <core/validation/item/item_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data_mock.hpp>

static constexpr const char* tags = "[core][validation][item]";

TEST_CASE("dnd::ItemData::validate", tags) {
    dnd::ItemData item_data;
    set_valid_mock_values(item_data, "Item");
    item_data.requires_attunement = false;

    dnd::Errors errors;
    SECTION("Valid data without optional fields") {
        REQUIRE_NOTHROW(errors = item_data.validate());
        REQUIRE(errors.ok());

        item_data.requires_attunement = true;
        REQUIRE_NOTHROW(errors = item_data.validate());
        REQUIRE(errors.ok());
    }

    SECTION("Valid data with optional fields") {
        item_data.cosmetic_description = "Cosmetic Description";
        REQUIRE_NOTHROW(errors = item_data.validate());
        REQUIRE(errors.ok());
    }
}
