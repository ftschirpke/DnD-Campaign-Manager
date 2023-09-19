#include <dnd_config.hpp>

#include <core/validation/effect_holder/subholders/proficiency_holder_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

static constexpr const char* tags = "[core][validation][effect_holder]";

TEST_CASE("dnd::ProficiencyHolderData::validate and ::validate_relations // valid proficiency holder", tags) {
    dndtest::ValidationDataMock parent;
    dnd::ProficiencyHolderData proficiency_holder(&parent);
    dnd::Content content = dndtest::minimal_testing_content();
    dnd::Errors errors;

    SECTION("empty proficiency holder") {
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_NOTHROW(errors += proficiency_holder.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("only armor") {
        proficiency_holder.armor = {"leather"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_NOTHROW(errors += proficiency_holder.validate_relations(content));
        REQUIRE(errors.ok());

        proficiency_holder.armor = {"light armor", "hide"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_NOTHROW(errors += proficiency_holder.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("only weapons") {
        proficiency_holder.weapons = {"dagger"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_NOTHROW(errors += proficiency_holder.validate_relations(content));
        REQUIRE(errors.ok());

        proficiency_holder.weapons = {"simple weapons", "shortsword"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_NOTHROW(errors += proficiency_holder.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("only tools") {
        proficiency_holder.tools = {"thieves' tools"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_NOTHROW(errors += proficiency_holder.validate_relations(content));
        REQUIRE(errors.ok());

        proficiency_holder.tools = {"thieves' tools", "disguise kit"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_NOTHROW(errors += proficiency_holder.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("only skills") {
        proficiency_holder.skills = {"Acrobatics"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_NOTHROW(errors += proficiency_holder.validate_relations(content));
        REQUIRE(errors.ok());

        proficiency_holder.skills = {"Athletics", "Stealth"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_NOTHROW(errors += proficiency_holder.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("only saving throws") {
        proficiency_holder.saving_throws = {"STR"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_NOTHROW(errors += proficiency_holder.validate_relations(content));
        REQUIRE(errors.ok());

        proficiency_holder.saving_throws = {"DEX", "CON"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_NOTHROW(errors += proficiency_holder.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("only languages") {
        proficiency_holder.languages = {"Common"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_NOTHROW(errors += proficiency_holder.validate_relations(content));
        REQUIRE(errors.ok());

        proficiency_holder.languages = {"Common", "Dwarvish", "Elvish"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_NOTHROW(errors += proficiency_holder.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("only senses") {
        proficiency_holder.senses = {"Darkvision"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_NOTHROW(errors += proficiency_holder.validate_relations(content));
        REQUIRE(errors.ok());

        proficiency_holder.senses = {"Blindsight", "Truesight"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_NOTHROW(errors += proficiency_holder.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("a combination") {
        proficiency_holder.armor = {"light armor", "hide"};
        proficiency_holder.weapons = {"simple weapons", "shortsword"};
        proficiency_holder.tools = {"thieves' tools", "disguise kit"};
        proficiency_holder.skills = {"Athletics", "Stealth"};
        proficiency_holder.saving_throws = {"DEX", "CON"};
        proficiency_holder.languages = {"Common", "Dwarvish", "Elvish"};
        proficiency_holder.senses = {"Blindsight", "Truesight"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_NOTHROW(errors += proficiency_holder.validate_relations(content));
        REQUIRE(errors.ok());
    }
}

TEST_CASE("dnd::ProficiencyHolderData::validate // invalid proficiency holder", tags) {
    dndtest::ValidationDataMock parent;
    dnd::ProficiencyHolderData proficiency_holder(&parent);
    dnd::Errors errors;

    SECTION("empty armor") {
        proficiency_holder.armor = {""};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_FALSE(errors.ok());

        proficiency_holder.armor = {"light armor", ""};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty weapons") {
        proficiency_holder.weapons = {""};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_FALSE(errors.ok());

        proficiency_holder.weapons = {"simple weapons", ""};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty tools") {
        proficiency_holder.tools = {""};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_FALSE(errors.ok());

        proficiency_holder.tools = {"thieves' tools", ""};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty skills") {
        proficiency_holder.skills = {""};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_FALSE(errors.ok());

        proficiency_holder.skills = {"Athletics", ""};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty saving throws") {
        proficiency_holder.saving_throws = {""};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_FALSE(errors.ok());

        proficiency_holder.saving_throws = {"Dexterity", ""};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty languages") {
        proficiency_holder.languages = {""};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_FALSE(errors.ok());

        proficiency_holder.languages = {"Common", ""};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty senses") {
        proficiency_holder.senses = {""};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_FALSE(errors.ok());

        proficiency_holder.senses = {"Blindsight", ""};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown skills") {
        proficiency_holder.skills = {"unknown skill"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_FALSE(errors.ok());

        proficiency_holder.skills = {"Athletics", "unknown skill"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown saving throws") {
        proficiency_holder.saving_throws = {"unknown saving throw"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_FALSE(errors.ok());

        proficiency_holder.saving_throws = {"DEX", "unknown saving throw"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate());
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("dnd::ProficiencyHolderData::validate_relations // invalid proficiency holder relations", tags) {
    dndtest::ValidationDataMock parent;
    dnd::ProficiencyHolderData proficiency_holder(&parent);
    dnd::Content content = dndtest::minimal_testing_content();
    dnd::Errors errors;

    SECTION("unknown armor") {
        proficiency_holder.armor = {"unknown armor"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate_relations(content));
        REQUIRE_FALSE(errors.ok());

        proficiency_holder.armor = {"light armor", "unknown armor"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown weapons") {
        proficiency_holder.weapons = {"unknown weapon"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate_relations(content));
        REQUIRE_FALSE(errors.ok());

        proficiency_holder.weapons = {"simple weapons", "unknown weapon"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown tools") {
        proficiency_holder.tools = {"unknown tool"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate_relations(content));
        REQUIRE_FALSE(errors.ok());

        proficiency_holder.tools = {"thieves' tools", "unknown tool"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown languages") {
        proficiency_holder.languages = {"unknown language"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate_relations(content));
        REQUIRE_FALSE(errors.ok());

        proficiency_holder.languages = {"Common", "unknown language"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown senses") {
        proficiency_holder.senses = {"unknown sense"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate_relations(content));
        REQUIRE_FALSE(errors.ok());

        proficiency_holder.senses = {"Blindsight", "unknown sense"};
        REQUIRE_NOTHROW(errors = proficiency_holder.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("dnd::ProficiencyHolderData::empty", tags) {
    dndtest::ValidationDataMock parent;
    dnd::ProficiencyHolderData proficiency_holder(&parent);
    dnd::Errors errors;

    REQUIRE(proficiency_holder.empty());

    SECTION("only armor") {
        proficiency_holder.armor = {"light armor", "hide"};
        REQUIRE_FALSE(proficiency_holder.empty());
    }

    SECTION("only weapons") {
        proficiency_holder.weapons = {"simple weapons", "shortsword"};
        REQUIRE_FALSE(proficiency_holder.empty());
    }

    SECTION("only tools") {
        proficiency_holder.tools = {"thieves' tools", "disguise kit"};
        REQUIRE_FALSE(proficiency_holder.empty());
    }

    SECTION("only skills") {
        proficiency_holder.skills = {"Athletics", "Stealth"};
        REQUIRE_FALSE(proficiency_holder.empty());
    }

    SECTION("only saving throws") {
        proficiency_holder.saving_throws = {"Dexterity", "Constitution"};
        REQUIRE_FALSE(proficiency_holder.empty());
    }

    SECTION("only languages") {
        proficiency_holder.languages = {"Common", "Dwarvish", "Elvish"};
        REQUIRE_FALSE(proficiency_holder.empty());
    }

    SECTION("only senses") {
        proficiency_holder.senses = {"Blindsight", "Truesight"};
        REQUIRE_FALSE(proficiency_holder.empty());
    }

    SECTION("full") {
        proficiency_holder.armor = {"light armor", "hide"};
        proficiency_holder.weapons = {"simple weapons", "shortsword"};
        proficiency_holder.tools = {"thieves' tools", "disguise kit"};
        proficiency_holder.skills = {"Athletics", "Stealth"};
        proficiency_holder.saving_throws = {"Dexterity", "Constitution"};
        proficiency_holder.languages = {"Common", "Dwarvish", "Elvish"};
        proficiency_holder.senses = {"Blindsight", "Truesight"};
        REQUIRE_FALSE(proficiency_holder.empty());
    }
}
