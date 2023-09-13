#include <dnd_config.hpp>

#include <core/validation/effect_holder/subholders/action_holder_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <testcore/validation/validation_data_mock.hpp>

static constexpr const char* tags = "[core][validation][effect_holder]";

TEST_CASE("dnd::ActionHolderData::validate // valid action holders", tags) {
    dndtest::ValidationDataMock parent;
    dnd::ActionHolderData action_holder_data(&parent);
    dnd::Errors errors;

    SECTION("empty action holders") {
        REQUIRE_NOTHROW(errors = action_holder_data.validate());
        REQUIRE(errors.ok());
    }

    SECTION("only action") {
        action_holder_data.actions["Some action"] = "Do something";
        REQUIRE_NOTHROW(errors = action_holder_data.validate());
        REQUIRE(errors.ok());
    }

    SECTION("only bonus action") {
        action_holder_data.bonus_actions["Some bonus action"] = "Do something else";
        REQUIRE_NOTHROW(errors = action_holder_data.validate());
        REQUIRE(errors.ok());
    }

    SECTION("only reaction") {
        action_holder_data.reactions["Some reaction"] = "Do something else again";
        REQUIRE_NOTHROW(errors = action_holder_data.validate());
        REQUIRE(errors.ok());
    }

    SECTION("all three") {
        action_holder_data.actions["Some action"] = "Do something";
        action_holder_data.bonus_actions["Some bonus action"] = "Do something else";
        action_holder_data.reactions["Some reaction"] = "Do something else again";
        REQUIRE_NOTHROW(errors = action_holder_data.validate());
        REQUIRE(errors.ok());
    }

    SECTION("two actions, one bonus action") {
        action_holder_data.actions["Some action"] = "Do something";
        action_holder_data.actions["Some other action"] = "Do something else";
        action_holder_data.bonus_actions["Some bonus action"] = "Do something else";
        REQUIRE_NOTHROW(errors = action_holder_data.validate());
        REQUIRE(errors.ok());
    }
}

TEST_CASE("dnd::ActionHolderData::validate // invalid action holders", tags) {
    dndtest::ValidationDataMock parent;
    dnd::ActionHolderData action_holder_data(&parent);
    dnd::Errors errors;

    SECTION("empty action name") {
        action_holder_data.actions[""] = "Do something";
        REQUIRE_NOTHROW(errors = action_holder_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty bonus action name") {
        action_holder_data.bonus_actions[""] = "Do something";
        REQUIRE_NOTHROW(errors = action_holder_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty reaction name") {
        action_holder_data.reactions[""] = "Do something";
        REQUIRE_NOTHROW(errors = action_holder_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty action description") {
        action_holder_data.actions["Some action"] = "";
        REQUIRE_NOTHROW(errors = action_holder_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty bonus action description") {
        action_holder_data.bonus_actions["Some action"] = "";
        REQUIRE_NOTHROW(errors = action_holder_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty reaction description") {
        action_holder_data.reactions["Some action"] = "";
        REQUIRE_NOTHROW(errors = action_holder_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty action names and descriptions in combination 1") {
        action_holder_data.actions[""] = "";
        REQUIRE_NOTHROW(errors = action_holder_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty action names and descriptions in combination 2") {
        action_holder_data.actions[""] = "Do something";
        action_holder_data.actions["Some action"] = "";
        REQUIRE_NOTHROW(errors = action_holder_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty action names and descriptions in combination 3") {
        action_holder_data.actions[""] = "Do something";
        action_holder_data.actions["Some action"] = "";
        action_holder_data.actions["Some other action"] = "";
        REQUIRE_NOTHROW(errors = action_holder_data.validate());
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("dnd::ActionHolderData::empty", tags) {
    dndtest::ValidationDataMock parent;
    dnd::ActionHolderData action_holder_data(&parent);
    dnd::Errors errors;

    REQUIRE(action_holder_data.empty());

    SECTION("only action") {
        action_holder_data.actions["Some action"] = "Do something";
        REQUIRE_FALSE(action_holder_data.empty());
    }

    SECTION("only bonus action") {
        action_holder_data.bonus_actions["Some bonus action"] = "Do something else";
        REQUIRE_FALSE(action_holder_data.empty());
    }

    SECTION("only reaction") {
        action_holder_data.reactions["Some reaction"] = "Do something else again";
        REQUIRE_FALSE(action_holder_data.empty());
    }

    SECTION("all three") {
        action_holder_data.actions["Some action"] = "Do something";
        action_holder_data.bonus_actions["Some bonus action"] = "Do something else";
        action_holder_data.reactions["Some reaction"] = "Do something else again";
        REQUIRE_FALSE(action_holder_data.empty());
    }
}
