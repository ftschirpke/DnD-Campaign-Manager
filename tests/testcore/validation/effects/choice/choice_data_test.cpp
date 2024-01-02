#include <dnd_config.hpp>

#include <core/validation/effects/choice/choice_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][effects]";

TEST_CASE("Validate Choice // valid choice data", tags) {
    ValidationDataMock parent;
    Choice::Data data;
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("attribute name implies group name") {
        data.attribute_name = "languages";
        data.amount = 2;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE(errors.ok());

        data.attribute_name = "skill_proficiencies";
        data.amount = 3;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE(errors.ok());

        data.attribute_name = "cantrips_free";
        data.amount = 1;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("explicit group names") {
        data.attribute_name = "armor_proficiencies";
        data.group_names = {"light armor", "medium armor"};
        data.amount = 1;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE(errors.ok());

        data.attribute_name = "cantrips_free";
        data.group_names = {"Wizard cantrips"};
        data.amount = 1;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE(errors.ok());

        data.attribute_name = "spells_added_to_spell_list";
        data.group_names = {"Wizard spells"};
        data.amount = 3;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("explicit choices") {
        data.attribute_name = "stat_changes";
        data.explicit_choices = {"CHA normal add 2", "INT normal add 2"};
        data.amount = 1;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE(errors.ok());

        data.attribute_name = "skill_proficiencies";
        data.explicit_choices = {"acrobatics", "athletics", "stealth", "perception", "insight"};
        data.amount = 3;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("group names and explicit choices") {
        data.attribute_name = "spells_known";
        data.group_names = {"Wizard spells"};
        data.explicit_choices = {"Cure Wounds"};
        data.amount = 2;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE(errors.ok());

        data.attribute_name = "weapon_proficiencies";
        data.group_names = {"simple melee weapons", "martial ranged weapons"};
        data.explicit_choices = {"shortsword", "shortbow"};
        data.amount = 2;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE(errors.ok());
    }
}

TEST_CASE("Validate Choice // invalid choice data", tags) {
    ValidationDataMock parent;
    Choice::Data data;
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("Empty attribute name") {
        data.attribute_name = "";
        data.amount = 2;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Non-positive amount") {
        data.attribute_name = "languages";
        data.amount = 0;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.amount = -1;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Invalid attribute name") {
        data.attribute_name = "invalid";
        data.amount = 2;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Provided group names where they are prohibited") {
        data.attribute_name = "saving_throw_proficiencies";
        data.group_names = {"some made-up group"};
        data.amount = 2;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.attribute_name = "skill_proficiencies";
        data.group_names = {"some made-up group", "another made-up group"};
        data.amount = 3;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.attribute_name = "effects";
        data.group_names = {"a totally new made-up group"};
        data.amount = 1;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Provided group names where they are prohibited") {
        data.attribute_name = "saving_throw_proficiencies";
        data.group_names = {"some made-up group"};
        data.amount = 2;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.attribute_name = "skill_proficiencies";
        data.group_names = {"some made-up group", "another made-up group"};
        data.amount = 3;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.attribute_name = "effects";
        data.group_names = {"a totally new made-up group"};
        data.amount = 1;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("Validate Choice // invalid choice data relations", tags) {
    ValidationDataMock parent;
    Choice::Data data;
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("Invalid choices for attribute") {
        data.attribute_name = "languages";
        data.explicit_choices = {"Dwarvish", "Common", "invalid"};
        data.amount = 1;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.attribute_name = "armor_proficiencies";
        data.explicit_choices = {"shortbow", "longbow", "shortsword", "longsword"};
        data.amount = 1;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Invalid group names for attribute") {
        data.attribute_name = "armor_proficiencies";
        data.group_names = {"light armor", "invalid"};
        data.amount = 1;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.attribute_name = "spells_added_to_spell_list";
        data.group_names = {"armor", "weapons"};
        data.amount = 3;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Cannot have spells for cantrips and vice versa") {
        data.attribute_name = "cantrips_free";
        data.group_names = {"Wizard spells"};
        data.amount = 1;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.attribute_name = "spells_added_to_spell_list";
        data.group_names = {"Wizard cantrips"};
        data.amount = 1;
        REQUIRE_NOTHROW(errors = validate_choice_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }
}

} // namespace dnd::test
