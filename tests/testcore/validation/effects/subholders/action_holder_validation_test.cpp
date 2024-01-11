#include <dnd_config.hpp>

#include <core/validation/effects/subholders/action_holder_validation.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][effects]";

TEST_CASE("Validate ActionHolder // valid action holders", tags) {
    ValidationDataMock parent;
    ActionHolder::Data data;
    Errors errors;

    SECTION("empty action holders") {
        REQUIRE_NOTHROW(errors = validate_actions_holder(data));
        REQUIRE(errors.ok());
    }

    SECTION("only action") {
        data.actions["Some action"] = "Do something";
        REQUIRE_NOTHROW(errors = validate_actions_holder(data));
        REQUIRE(errors.ok());
    }

    SECTION("only bonus action") {
        data.bonus_actions["Some bonus action"] = "Do something else";
        REQUIRE_NOTHROW(errors = validate_actions_holder(data));
        REQUIRE(errors.ok());
    }

    SECTION("only reaction") {
        data.reactions["Some reaction"] = "Do something else again";
        REQUIRE_NOTHROW(errors = validate_actions_holder(data));
        REQUIRE(errors.ok());
    }

    SECTION("all three") {
        data.actions["Some action"] = "Do something";
        data.bonus_actions["Some bonus action"] = "Do something else";
        data.reactions["Some reaction"] = "Do something else again";
        REQUIRE_NOTHROW(errors = validate_actions_holder(data));
        REQUIRE(errors.ok());
    }

    SECTION("two actions, one bonus action") {
        data.actions["Some action"] = "Do something";
        data.actions["Some other action"] = "Do something else";
        data.bonus_actions["Some bonus action"] = "Do something else";
        REQUIRE_NOTHROW(errors = validate_actions_holder(data));
        REQUIRE(errors.ok());
    }
}

TEST_CASE("Validate ActionHolder // invalid action holders", tags) {
    ValidationDataMock parent;
    ActionHolder::Data data;
    Errors errors;

    SECTION("empty action name") {
        data.actions[""] = "Do something";
        REQUIRE_NOTHROW(errors = validate_actions_holder(data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty bonus action name") {
        data.bonus_actions[""] = "Do something";
        REQUIRE_NOTHROW(errors = validate_actions_holder(data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty reaction name") {
        data.reactions[""] = "Do something";
        REQUIRE_NOTHROW(errors = validate_actions_holder(data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty action description") {
        data.actions["Some action"] = "";
        REQUIRE_NOTHROW(errors = validate_actions_holder(data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty bonus action description") {
        data.bonus_actions["Some action"] = "";
        REQUIRE_NOTHROW(errors = validate_actions_holder(data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty reaction description") {
        data.reactions["Some action"] = "";
        REQUIRE_NOTHROW(errors = validate_actions_holder(data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty action names and descriptions in combination 1") {
        data.actions[""] = "";
        REQUIRE_NOTHROW(errors = validate_actions_holder(data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty action names and descriptions in combination 2") {
        data.actions[""] = "Do something";
        data.actions["Some action"] = "";
        REQUIRE_NOTHROW(errors = validate_actions_holder(data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty action names and descriptions in combination 3") {
        data.actions[""] = "Do something";
        data.actions["Some action"] = "";
        data.actions["Some other action"] = "";
        REQUIRE_NOTHROW(errors = validate_actions_holder(data));
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("ActionHolder::Data::empty", tags) {
    ValidationDataMock parent;
    ActionHolder::Data data;
    Errors errors;

    REQUIRE(data.empty());

    SECTION("only action") {
        data.actions["Some action"] = "Do something";
        REQUIRE_FALSE(data.empty());
    }

    SECTION("only bonus action") {
        data.bonus_actions["Some bonus action"] = "Do something else";
        REQUIRE_FALSE(data.empty());
    }

    SECTION("only reaction") {
        data.reactions["Some reaction"] = "Do something else again";
        REQUIRE_FALSE(data.empty());
    }

    SECTION("all three") {
        data.actions["Some action"] = "Do something";
        data.bonus_actions["Some bonus action"] = "Do something else";
        data.reactions["Some reaction"] = "Do something else again";
        REQUIRE_FALSE(data.empty());
    }
}

} // namespace dnd::test
