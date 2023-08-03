#include <dnd_config.hpp>

#include <core/validation/effect_holder/choice/choice_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/validation/minimal_testing_content.hpp>
#include <core/validation/validation_data_mock.hpp>

static constexpr const char* tags = "[core][validation][effect_holder]";

TEST_CASE("dnd::ChoiceData::validate // valid choice data", tags) {
    dndtest::ValidationDataMock parent;
    dnd::ChoiceData choice_data(&parent);
    dnd::Content content = dnd::minimal_testing_content();
    dnd::Errors errors;

    SECTION("attribute name implies group name") {
        choice_data.attribute_name = "languages";
        choice_data.amount = 2;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE(errors.ok());

        choice_data.attribute_name = "skill_proficiencies";
        choice_data.amount = 3;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE(errors.ok());

        choice_data.attribute_name = "cantrips_free";
        choice_data.amount = 1;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("explicit group names") {
        choice_data.attribute_name = "armor_proficiencies";
        choice_data.group_names = {"light armor", "medium armor"};
        choice_data.amount = 1;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE(errors.ok());

        choice_data.attribute_name = "cantrips_free";
        choice_data.group_names = {"Wizard cantrips"};
        choice_data.amount = 1;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE(errors.ok());

        choice_data.attribute_name = "spells_added_to_spell_list";
        choice_data.group_names = {"Wizard spells"};
        choice_data.amount = 3;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("explicit choices") {
        choice_data.attribute_name = "effects";
        choice_data.explicit_choices = {"CHA normal add 2", "INT normal add 2"};
        choice_data.amount = 1;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE(errors.ok());

        choice_data.attribute_name = "skill_proficiencies";
        choice_data.explicit_choices = {"acrobatics", "athletics", "stealth", "perception", "insight"};
        choice_data.amount = 3;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("group names and explicit choices") {
        choice_data.attribute_name = "spells_known";
        choice_data.group_names = {"Wizard spells"};
        choice_data.explicit_choices = {"Cure Wounds"};
        choice_data.amount = 2;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE(errors.ok());

        choice_data.attribute_name = "weapon_proficiencies";
        choice_data.group_names = {"simple melee weapons", "martial ranged weapons"};
        choice_data.explicit_choices = {"shortsword", "shortbow"};
        choice_data.amount = 2;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE(errors.ok());
    }
}

TEST_CASE("dnd::ChoiceData::validate // invalid choice data", tags) {
    dndtest::ValidationDataMock parent;
    dnd::ChoiceData choice_data(&parent);
    dnd::Content content = dnd::minimal_testing_content();
    dnd::Errors errors;

    SECTION("Empty attribute name") {
        choice_data.attribute_name = "";
        choice_data.amount = 2;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Non-positive amount") {
        choice_data.attribute_name = "languages";
        choice_data.amount = 0;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());

        choice_data.amount = -1;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Invalid attribute name") {
        choice_data.attribute_name = "invalid";
        choice_data.amount = 2;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Invalid choices for attribute") {
        choice_data.attribute_name = "languages";
        choice_data.explicit_choices = {"Dwarvish", "Common", "invalid"};
        choice_data.amount = 1;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());

        choice_data.attribute_name = "armor_proficiencies";
        choice_data.explicit_choices = {"shortbow", "longbow", "shortsword", "longsword"};
        choice_data.amount = 1;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Invalid group names for attribute") {
        choice_data.attribute_name = "armor_proficiencies";
        choice_data.group_names = {"light armor", "invalid"};
        choice_data.amount = 1;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());

        choice_data.attribute_name = "spells_added_to_spell_list";
        choice_data.group_names = {"armor", "weapons"};
        choice_data.amount = 3;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Provided group names where they are prohibited") {
        choice_data.attribute_name = "saving_throw_proficiencies";
        choice_data.group_names = {"some made-up group"};
        choice_data.amount = 2;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());

        choice_data.attribute_name = "skill_proficiencies";
        choice_data.group_names = {"some made-up group", "another made-up group"};
        choice_data.amount = 3;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());

        choice_data.attribute_name = "effects";
        choice_data.group_names = {"a totally new made-up group"};
        choice_data.amount = 1;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Cannot have spells for cantrips and vice versa") {
        choice_data.attribute_name = "cantrips_free";
        choice_data.group_names = {"Wizard spells"};
        choice_data.amount = 1;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());

        choice_data.attribute_name = "spells_added_to_spell_list";
        choice_data.group_names = {"Wizard cantrips"};
        choice_data.amount = 1;
        REQUIRE_NOTHROW(errors = choice_data.validate());
        REQUIRE_NOTHROW(errors += choice_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }
}
