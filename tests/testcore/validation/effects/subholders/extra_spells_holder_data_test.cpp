#include <dnd_config.hpp>

#include <core/validation/effects/subholders/extra_spells_holder_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][effects]";

TEST_CASE("ExtraSpellsHolderData::validate and ::validate_relations // valid extra spell holders", tags) {
    ValidationDataMock parent;
    ExtraSpellsHolderData extra_spells_holder_data(&parent);
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("empty extra spell holders") {
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_NOTHROW(errors += extra_spells_holder_data.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("free cantrips only") {
        extra_spells_holder_data.free_cantrips = {"Dancing Lights"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_NOTHROW(errors += extra_spells_holder_data.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("at will spells only") {
        extra_spells_holder_data.at_will = {"Fireball"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_NOTHROW(errors += extra_spells_holder_data.validate_relations(content));
        REQUIRE(errors.ok());

        extra_spells_holder_data.at_will.insert("Cure Wounds");
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_NOTHROW(errors += extra_spells_holder_data.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("innate spells only") {
        extra_spells_holder_data.innate = {"Cure Wounds"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_NOTHROW(errors += extra_spells_holder_data.validate_relations(content));
        REQUIRE(errors.ok());

        extra_spells_holder_data.innate.insert("Fireball");
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_NOTHROW(errors += extra_spells_holder_data.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("free once a day spells only") {
        extra_spells_holder_data.free_once_a_day = {"Fireball"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_NOTHROW(errors += extra_spells_holder_data.validate_relations(content));
        REQUIRE(errors.ok());

        extra_spells_holder_data.free_once_a_day.insert("Cure Wounds");
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_NOTHROW(errors += extra_spells_holder_data.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("known spells only") {
        extra_spells_holder_data.spells_known = {"Cure Wounds"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_NOTHROW(errors += extra_spells_holder_data.validate_relations(content));
        REQUIRE(errors.ok());

        extra_spells_holder_data.spells_known.insert("Fireball");
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_NOTHROW(errors += extra_spells_holder_data.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("known spells (included) only") {
        extra_spells_holder_data.spells_known_included = {"Fireball"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_NOTHROW(errors += extra_spells_holder_data.validate_relations(content));
        REQUIRE(errors.ok());

        extra_spells_holder_data.spells_known_included.insert("Cure Wounds");
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_NOTHROW(errors += extra_spells_holder_data.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("spells added to spell list only") {
        extra_spells_holder_data.added_to_spell_list = {"Cure Wounds"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_NOTHROW(errors += extra_spells_holder_data.validate_relations(content));
        REQUIRE(errors.ok());

        extra_spells_holder_data.added_to_spell_list.insert("Fireball");
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_NOTHROW(errors += extra_spells_holder_data.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("combination") {
        extra_spells_holder_data.free_cantrips = {"Dancing Lights"};
        extra_spells_holder_data.spells_known = {"Cure Wounds"};
        extra_spells_holder_data.added_to_spell_list = {"Fireball"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_NOTHROW(errors += extra_spells_holder_data.validate_relations(content));
        REQUIRE(errors.ok());
    }
}

TEST_CASE("ExtraSpellsHolderData::validate // invalid extra spell holders", tags) {
    ValidationDataMock parent;
    ExtraSpellsHolderData extra_spells_holder_data(&parent);
    Errors errors;

    SECTION("empty free cantrip names") {
        extra_spells_holder_data.free_cantrips = {""};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_FALSE(errors.ok());

        extra_spells_holder_data.free_cantrips = {"", "Dancing Lights"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty at will spell names") {
        extra_spells_holder_data.at_will = {""};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_FALSE(errors.ok());

        extra_spells_holder_data.at_will = {"", "Fireball"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty innate spell names") {
        extra_spells_holder_data.innate = {""};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_FALSE(errors.ok());

        extra_spells_holder_data.innate = {"", "Cure Wounds"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty free once a day spell names") {
        extra_spells_holder_data.free_once_a_day = {""};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_FALSE(errors.ok());

        extra_spells_holder_data.free_once_a_day = {"", "Fireball"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty known spell names") {
        extra_spells_holder_data.spells_known = {""};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_FALSE(errors.ok());

        extra_spells_holder_data.spells_known = {"", "Cure Wounds"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty known spell (included) names") {
        extra_spells_holder_data.spells_known_included = {""};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_FALSE(errors.ok());

        extra_spells_holder_data.spells_known_included = {"", "Fireball"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty added to spell list names") {
        extra_spells_holder_data.added_to_spell_list = {""};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_FALSE(errors.ok());

        extra_spells_holder_data.added_to_spell_list = {"", "Cure Wounds"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("ExtraSpellsHolderData::validate_relations // invalid extra spell holder relations", tags) {
    ValidationDataMock parent;
    ExtraSpellsHolderData extra_spells_holder_data(&parent);
    Content content = minimal_testing_content();
    Errors errors;

    REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate());
    REQUIRE(errors.ok());

    SECTION("unknown free cantrips") {
        extra_spells_holder_data.free_cantrips = {"Dancing Lights", "Some other cantrip"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown at will spells") {
        extra_spells_holder_data.at_will = {"Fireball", "Some other spell"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown innate spells") {
        extra_spells_holder_data.innate = {"Cure Wounds", "Some other spell"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown free once a day spells") {
        extra_spells_holder_data.free_once_a_day = {"Fireball", "Some other spell"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown known spells") {
        extra_spells_holder_data.spells_known = {"Cure Wounds", "Some other spell"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown known spells (included)") {
        extra_spells_holder_data.spells_known_included = {"Fireball", "Some other spell"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown spells added to spell list") {
        extra_spells_holder_data.added_to_spell_list = {"Cure Wounds", "Some other spell"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("non-cantrip spell as free cantrip") {
        extra_spells_holder_data.free_cantrips = {"Cure Wounds"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());

        extra_spells_holder_data.free_cantrips = {"Cure Wounds", "Dancing Lights"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("cantrip as at will spell") {
        extra_spells_holder_data.at_will = {"Dancing Lights"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());

        extra_spells_holder_data.at_will = {"Dancing Lights", "Fireball"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("cantrip as at innate spell") {
        extra_spells_holder_data.innate = {"Dancing Lights"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());

        extra_spells_holder_data.innate = {"Dancing Lights", "Fireball"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("cantrip as free once a day spell") {
        extra_spells_holder_data.free_once_a_day = {"Dancing Lights"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());

        extra_spells_holder_data.free_once_a_day = {"Dancing Lights", "Cure Wounds"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("cantrip as known spell") {
        extra_spells_holder_data.spells_known = {"Dancing Lights"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());

        extra_spells_holder_data.spells_known = {"Dancing Lights", "Cure Wounds"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("cantrip as known spell (included)") {
        extra_spells_holder_data.spells_known_included = {"Dancing Lights"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());

        extra_spells_holder_data.spells_known_included = {"Dancing Lights", "Fireball"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("cantrip added to spell list") {
        extra_spells_holder_data.added_to_spell_list = {"Dancing Lights"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());

        extra_spells_holder_data.added_to_spell_list = {"Dancing Lights", "Cure Wounds"};
        REQUIRE_NOTHROW(errors = extra_spells_holder_data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("ExtraSpellsHolderData::emtpy") {
    ValidationDataMock parent;
    ExtraSpellsHolderData extra_spells_holder_data(&parent);

    REQUIRE(extra_spells_holder_data.empty());

    SECTION("free cantrips") {
        extra_spells_holder_data.free_cantrips = {"Dancing Lights"};
        REQUIRE_FALSE(extra_spells_holder_data.empty());
    }

    SECTION("at will spells") {
        extra_spells_holder_data.at_will = {"Fireball"};
        REQUIRE_FALSE(extra_spells_holder_data.empty());
    }

    SECTION("innate spells") {
        extra_spells_holder_data.innate = {"Cure Wounds"};
        REQUIRE_FALSE(extra_spells_holder_data.empty());
    }

    SECTION("free once a day spells") {
        extra_spells_holder_data.free_once_a_day = {"Fireball"};
        REQUIRE_FALSE(extra_spells_holder_data.empty());
    }

    SECTION("known spells") {
        extra_spells_holder_data.spells_known = {"Cure Wounds"};
        REQUIRE_FALSE(extra_spells_holder_data.empty());
    }

    SECTION("known spells (included)") {
        extra_spells_holder_data.spells_known_included = {"Fireball"};
        REQUIRE_FALSE(extra_spells_holder_data.empty());
    }

    SECTION("spells added to spell list") {
        extra_spells_holder_data.added_to_spell_list = {"Cure Wounds"};
        REQUIRE_FALSE(extra_spells_holder_data.empty());
    }

    SECTION("combination") {
        extra_spells_holder_data.free_cantrips = {"Dancing Lights"};
        extra_spells_holder_data.spells_known = {"Cure Wounds"};
        extra_spells_holder_data.added_to_spell_list = {"Fireball"};
        REQUIRE_FALSE(extra_spells_holder_data.empty());
    }
}

} // namespace dnd::test
