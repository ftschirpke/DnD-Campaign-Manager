#include <dnd_config.hpp>

#include <core/validation/effects/condition/condition_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][effects]";

TEST_CASE("ConditionData::validate // valid conditions", tags) {
    ValidationDataMock parent;
    ConditionData condition_data(&parent);
    Errors errors;

    SECTION("boolean conditions") {
        condition_data.condition_str = "HAS_ARMOR == true";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE(errors.ok());

        condition_data.condition_str = "SOME_BOOLEAN == false";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE(errors.ok());
    }

    SECTION("number conditions") {
        condition_data.condition_str = "CLASS_LEVEL == 1";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE(errors.ok());

        condition_data.condition_str = "INT != 0";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE(errors.ok());

        condition_data.condition_str = "CLASS_LEVEL >= 3";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE(errors.ok());

        condition_data.condition_str = "AC <= 10";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE(errors.ok());

        condition_data.condition_str = "MY_VALUE_1 > 2.35";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE(errors.ok());

        condition_data.condition_str = "MY_VALUE_2 < -4";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE(errors.ok());
    }

    SECTION("attribute conditions") {
        condition_data.condition_str = "INT_MOD == WIS_MOD";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE(errors.ok());

        condition_data.condition_str = "STR_MOD != DEX_MOD";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE(errors.ok());

        condition_data.condition_str = "DEX_MOD >= CON_MOD";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE(errors.ok());

        condition_data.condition_str = "DEX_MOD <= AC";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE(errors.ok());

        condition_data.condition_str = "MY_VALUE_3 > CLASS_LEVEL";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE(errors.ok());

        condition_data.condition_str = "MY_VALUE_4 < MY_VALUE_5";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE(errors.ok());
    }
}

TEST_CASE("ConditionData::validate // invalid conditions", tags) {
    ValidationDataMock parent;
    ConditionData condition_data(&parent);
    Errors errors;

    SECTION("invalid attribute format (left side)") {
        condition_data.condition_str = "lowercase_not_allowed > 4";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());

        condition_data.condition_str = "SPACES NOT ALLOWED == 5";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());

        condition_data.condition_str = "SPECIAL_CHARS!NOT_ALLOWED == true";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());

        condition_data.condition_str = "123_MUST_START_WITH_LETTER != -2.5";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());

        condition_data.condition_str = "MY-VALUE_1 < 2.35";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("invalid attribute format (right side)") {
        condition_data.condition_str = "AC > lowercase_not_allowed";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());

        condition_data.condition_str = "CLASS_LEVEL == SPACES NOT ALLOWED";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());

        condition_data.condition_str = "INT == SPECIAL_CHARS!NOT_ALLOWED";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());

        condition_data.condition_str = "CON_MOD != 123_MUST_START_WITH_LETTER";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());

        condition_data.condition_str = "MAX_HP < MY-VALUE_1";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("invalid number format") {
        condition_data.condition_str = "MY_VALUE_2 != +2";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());

        condition_data.condition_str = "MY_VALUE_1 > 2.356";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());

        condition_data.condition_str = "MY_VALUE_2 < -4.0123";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("invalid operator") {
        condition_data.condition_str = "MY_VALUE_1 = 2.35";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());

        condition_data.condition_str = "MY_VALUE_2 ! 4";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());

        condition_data.condition_str = "MY_VALUE_3 >== 5";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());

        condition_data.condition_str = "MY_VALUE_4 <== 6";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());

        condition_data.condition_str = "LEVEL is 7";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("only == for boolean") {
        condition_data.condition_str = "ARMOR_ON != true";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());

        condition_data.condition_str = "HAS_SPELLCASTING >= false";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());

        condition_data.condition_str = "MY_VALUE_3 <= true";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());

        condition_data.condition_str = "MY_VALUE_4 > false";
        REQUIRE_NOTHROW(errors = condition_data.validate());
        REQUIRE_FALSE(errors.ok());
    }
}

} // namespace dnd::test
