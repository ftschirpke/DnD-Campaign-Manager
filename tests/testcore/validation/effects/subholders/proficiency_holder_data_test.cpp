#include <dnd_config.hpp>

#include <core/validation/effects/subholders/proficiency_holder_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][effects]";

TEST_CASE("Validate ProficiencyHolder // valid proficiency holder", tags) {
    ValidationDataMock parent;
    ProficiencyHolder::Data data;
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("empty proficiency holder") {
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("only armor") {
        data.armor = {"leather"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.armor = {"light armor", "hide"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("only weapons") {
        data.weapons = {"dagger"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.weapons = {"simple weapons", "shortsword"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("only tools") {
        data.tools = {"thieves' tools"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.tools = {"thieves' tools", "disguise kit"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("only skills") {
        data.skills = {"Acrobatics"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.skills = {"Athletics", "Stealth"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("only saving throws") {
        data.saving_throws = {"STR"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.saving_throws = {"DEX", "CON"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("only languages") {
        data.languages = {"Common"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.languages = {"Common", "Dwarvish", "Elvish"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("only senses") {
        data.senses = {"Darkvision"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.senses = {"Blindsight", "Truesight"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("a combination") {
        data.armor = {"light armor", "hide"};
        data.weapons = {"simple weapons", "shortsword"};
        data.tools = {"thieves' tools", "disguise kit"};
        data.skills = {"Athletics", "Stealth"};
        data.saving_throws = {"DEX", "CON"};
        data.languages = {"Common", "Dwarvish", "Elvish"};
        data.senses = {"Blindsight", "Truesight"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }
}

TEST_CASE("Validate ProficiencyHolder // invalid proficiency holder", tags) {
    ValidationDataMock parent;
    ProficiencyHolder::Data data;
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("empty armor") {
        data.armor = {""};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.armor = {"light armor", ""};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty weapons") {
        data.weapons = {""};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.weapons = {"simple weapons", ""};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty tools") {
        data.tools = {""};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.tools = {"thieves' tools", ""};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty skills") {
        data.skills = {""};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.skills = {"Athletics", ""};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty saving throws") {
        data.saving_throws = {""};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.saving_throws = {"Dexterity", ""};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty languages") {
        data.languages = {""};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.languages = {"Common", ""};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty senses") {
        data.senses = {""};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.senses = {"Blindsight", ""};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown skills") {
        data.skills = {"unknown skill"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.skills = {"Athletics", "unknown skill"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown saving throws") {
        data.saving_throws = {"unknown saving throw"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.saving_throws = {"DEX", "unknown saving throw"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("Validate ProficiencyHolder // invalid proficiency holder relations", tags) {
    ValidationDataMock parent;
    ProficiencyHolder::Data data;
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("unknown armor") {
        data.armor = {"unknown armor"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.armor = {"light armor", "unknown armor"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown weapons") {
        data.weapons = {"unknown weapon"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.weapons = {"simple weapons", "unknown weapon"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown tools") {
        data.tools = {"unknown tool"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.tools = {"thieves' tools", "unknown tool"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown languages") {
        data.languages = {"unknown language"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.languages = {"Common", "unknown language"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown senses") {
        data.senses = {"unknown sense"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.senses = {"Blindsight", "unknown sense"};
        REQUIRE_NOTHROW(errors = validate_proficiency_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("ProficiencyHolder::Data::empty", tags) {
    ValidationDataMock parent;
    ProficiencyHolder::Data data;
    Errors errors;

    REQUIRE(data.empty());

    SECTION("only armor") {
        data.armor = {"light armor", "hide"};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("only weapons") {
        data.weapons = {"simple weapons", "shortsword"};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("only tools") {
        data.tools = {"thieves' tools", "disguise kit"};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("only skills") {
        data.skills = {"Athletics", "Stealth"};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("only saving throws") {
        data.saving_throws = {"Dexterity", "Constitution"};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("only languages") {
        data.languages = {"Common", "Dwarvish", "Elvish"};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("only senses") {
        data.senses = {"Blindsight", "Truesight"};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("full") {
        data.armor = {"light armor", "hide"};
        data.weapons = {"simple weapons", "shortsword"};
        data.tools = {"thieves' tools", "disguise kit"};
        data.skills = {"Athletics", "Stealth"};
        data.saving_throws = {"Dexterity", "Constitution"};
        data.languages = {"Common", "Dwarvish", "Elvish"};
        data.senses = {"Blindsight", "Truesight"};
        REQUIRE_FALSE(data.empty());
    }
}

} // namespace dnd::test
