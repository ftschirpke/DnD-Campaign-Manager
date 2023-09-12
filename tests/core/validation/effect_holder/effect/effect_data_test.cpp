#include <dnd_config.hpp>

#include <core/validation/effect_holder/effect/effect_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data_mock.hpp>

static constexpr const char* tags = "[core][validation][effect_holder]";

TEST_CASE("dnd::EffectData::validate // valid effects", tags) {
    dndtest::ValidationDataMock parent;
    dnd::EffectData effect_data(&parent);
    dnd::Errors errors;

    SECTION("boolean effects") {
        effect_data.effect_str = "MY_BOOLEAN_VALUE earliest set false";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE(errors.ok());

        effect_data.effect_str = "ANOTHER_BOOLEAN_VALUE latest set true";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE(errors.ok());
    }

    SECTION("number effects") {
        effect_data.effect_str = "STR normal add 1";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE(errors.ok());

        effect_data.effect_str = "DEX early sub -2";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE(errors.ok());

        effect_data.effect_str = "SPEED late mult 1.5";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE(errors.ok());

        effect_data.effect_str = "MY_VALUE_1 latest div -2.35";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE(errors.ok());

        effect_data.effect_str = "SPEED earliest set 14.05";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE(errors.ok());

        effect_data.effect_str = "STR latest max 24";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE(errors.ok());
    }

    SECTION("attribute effects") {
        effect_data.effect_str = "MY_VALUE_1 earliest set STR_MOD";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE(errors.ok());

        effect_data.effect_str = "MY_VALUE_2 early add MY_VALUE_1";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE(errors.ok());

        effect_data.effect_str = "ANOTHER_VALUE normal sub MY_VALUE_2";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE(errors.ok());

        effect_data.effect_str = "SOME_VALUE late mult MY_VALUE_1";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE(errors.ok());

        effect_data.effect_str = "STR_SAVE latest div MY_VALUE_2";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE(errors.ok());

        effect_data.effect_str = "INT_MAX normal min MY_VALUE_1";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE(errors.ok());

        effect_data.effect_str = "CHA_MOD normal max MY_VALUE_2";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE(errors.ok());
    }
}

TEST_CASE("dnd::EffectData::validate // invalid effects", tags) {
    dndtest::ValidationDataMock parent;
    dnd::EffectData effect_data(&parent);
    dnd::Errors errors;

    SECTION("invalid attribute format (first word)") {
        effect_data.effect_str = "lowercase_not_allowed earliest set false";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "SPACES NOT ALLOWED latest set 4";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "SPECIAL_CHARS!NOT_ALLOWED normal add 1";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "123_MUST_START_WITH_LETTER normal sub 2";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY-VALUE_1 normal mult 1.5";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("invalid time operator (second word)") {
        effect_data.effect_str = "MY_VALUE_1 invalid set false";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_2 first add 1";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_3 at_the_end sub SPEED";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_4 EARLIEST mult 1.5";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_5 LATEST div STR_MOD";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_6 NORMAL max INT_SAVE";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("invalid operator name (third word)") {
        effect_data.effect_str = "MY_VALUE_1 earliest invalid false";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_2 early SET 10";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_3 early ADD 1";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_4 early substract 1";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_5 normal multiply 1.5";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_6 late divide 2";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_7 latest sin 24";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("invalid operator for boolean values (third word)") {
        effect_data.effect_str = "MY_VALUE_1 earliest add false";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_2 early sub true";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_3 normal mult false";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_4 late div true";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_5 latest max false";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_6 latest min true";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("invalid value (fourth position)") {
        effect_data.effect_str = "MY_VALUE_1 earliest set untrue";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_2 early add 12.521";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_3 normal sub +1.5";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_4 late mult -2.0";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_5 latest div 0";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_6 latest max 24.00";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("invalid attribute name (fourth word)") {
        effect_data.effect_str = "MY_VALUE_1 earliest set lowercase_not_allowed";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_2 early add SPACES NOT ALLOWED";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_3 normal sub SPECIAL_CHARS!NOT_ALLOWED";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_4 late mult 123_MUST_START_WITH_LETTER";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());

        effect_data.effect_str = "MY_VALUE_5 latest div MY-VALUE_0";
        REQUIRE_NOTHROW(errors = effect_data.validate());
        REQUIRE_FALSE(errors.ok());
    }
}
