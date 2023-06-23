#include <core/validation/basic_mechanics/dice_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/basic_mechanics/dice.hpp>

static constexpr const char* tags = "[core/validation/basic_mechanics/dice_data]";

TEST_CASE("dnd::DiceData", tags) {
    dnd::DiceData dice_data(nullptr);

    SECTION("Valid dice") {
        dice_data.str = "d6";
        dnd::Errors errors = dice_data.validate();
        REQUIRE(errors.ok());

        dice_data.str = "d20";
        errors = dice_data.validate();
        REQUIRE(errors.ok());

        dice_data.str = "d100";
        errors = dice_data.validate();
        REQUIRE(errors.ok());
    }

    SECTION("Invalid dice") {
        dice_data.str = "d0";
        dnd::Errors errors = dice_data.validate();
        REQUIRE_FALSE(errors.ok());
        REQUIRE_FALSE(errors.get_validation_errors().empty());
        REQUIRE(errors.get_parsing_errors().empty());

        dice_data.str = "d-1";
        errors = dice_data.validate();
        REQUIRE_FALSE(errors.ok());
        REQUIRE_FALSE(errors.get_validation_errors().empty());
        REQUIRE(errors.get_parsing_errors().empty());

        dice_data.str = "xyz";
        errors = dice_data.validate();
        REQUIRE_FALSE(errors.ok());
        REQUIRE_FALSE(errors.get_validation_errors().empty());
        REQUIRE(errors.get_parsing_errors().empty());
    }
}
