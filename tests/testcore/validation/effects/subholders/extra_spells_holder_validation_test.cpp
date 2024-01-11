#include <dnd_config.hpp>

#include <core/validation/effects/subholders/extra_spells_holder_validation.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][effects]";

TEST_CASE("Validate ExtraSpellsHolder // valid extra spell holders", tags) {
    ValidationDataMock parent;
    ExtraSpellsHolder::Data data;
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("empty extra spell holders") {
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("free cantrips only") {
        data.free_cantrips = {"Dancing Lights"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("at will spells only") {
        data.at_will = {"Fireball"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.at_will.insert("Cure Wounds");
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("innate spells only") {
        data.innate = {"Cure Wounds"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.innate.insert("Fireball");
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("free once a day spells only") {
        data.free_once_a_day = {"Fireball"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.free_once_a_day.insert("Cure Wounds");
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("known spells only") {
        data.spells_known = {"Cure Wounds"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.spells_known.insert("Fireball");
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("known spells (included) only") {
        data.spells_known_included = {"Fireball"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.spells_known_included.insert("Cure Wounds");
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("spells added to spell list only") {
        data.added_to_spell_list = {"Cure Wounds"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.added_to_spell_list.insert("Fireball");
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("combination") {
        data.free_cantrips = {"Dancing Lights"};
        data.spells_known = {"Cure Wounds"};
        data.added_to_spell_list = {"Fireball"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }
}

TEST_CASE("Validate ExtraSpellsHolder // invalid extra spell holders", tags) {
    ValidationDataMock parent;
    ExtraSpellsHolder::Data data;
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("empty free cantrip names") {
        data.free_cantrips = {""};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.free_cantrips = {"", "Dancing Lights"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty at will spell names") {
        data.at_will = {""};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.at_will = {"", "Fireball"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty innate spell names") {
        data.innate = {""};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.innate = {"", "Cure Wounds"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty free once a day spell names") {
        data.free_once_a_day = {""};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.free_once_a_day = {"", "Fireball"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty known spell names") {
        data.spells_known = {""};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.spells_known = {"", "Cure Wounds"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty known spell (included) names") {
        data.spells_known_included = {""};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.spells_known_included = {"", "Fireball"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty added to spell list names") {
        data.added_to_spell_list = {""};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.added_to_spell_list = {"", "Cure Wounds"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("Validate ExtraSpellsHolder // invalid extra spell holder relations", tags) {
    ValidationDataMock parent;
    ExtraSpellsHolder::Data data;
    Content content = minimal_testing_content();
    Errors errors;

    REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
    REQUIRE(errors.ok());

    SECTION("unknown free cantrips") {
        data.free_cantrips = {"Dancing Lights", "Some other cantrip"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown at will spells") {
        data.at_will = {"Fireball", "Some other spell"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown innate spells") {
        data.innate = {"Cure Wounds", "Some other spell"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown free once a day spells") {
        data.free_once_a_day = {"Fireball", "Some other spell"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown known spells") {
        data.spells_known = {"Cure Wounds", "Some other spell"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown known spells (included)") {
        data.spells_known_included = {"Fireball", "Some other spell"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown spells added to spell list") {
        data.added_to_spell_list = {"Cure Wounds", "Some other spell"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("non-cantrip spell as free cantrip") {
        data.free_cantrips = {"Cure Wounds"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.free_cantrips = {"Cure Wounds", "Dancing Lights"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("cantrip as at will spell") {
        data.at_will = {"Dancing Lights"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.at_will = {"Dancing Lights", "Fireball"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("cantrip as at innate spell") {
        data.innate = {"Dancing Lights"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.innate = {"Dancing Lights", "Fireball"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("cantrip as free once a day spell") {
        data.free_once_a_day = {"Dancing Lights"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.free_once_a_day = {"Dancing Lights", "Cure Wounds"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("cantrip as known spell") {
        data.spells_known = {"Dancing Lights"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.spells_known = {"Dancing Lights", "Cure Wounds"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("cantrip as known spell (included)") {
        data.spells_known_included = {"Dancing Lights"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.spells_known_included = {"Dancing Lights", "Fireball"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("cantrip added to spell list") {
        data.added_to_spell_list = {"Dancing Lights"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.added_to_spell_list = {"Dancing Lights", "Cure Wounds"};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("ExtraSpellsHolder::Data::emtpy") {
    ValidationDataMock parent;
    ExtraSpellsHolder::Data data;

    REQUIRE(data.empty());

    SECTION("free cantrips") {
        data.free_cantrips = {"Dancing Lights"};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("at will spells") {
        data.at_will = {"Fireball"};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("innate spells") {
        data.innate = {"Cure Wounds"};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("free once a day spells") {
        data.free_once_a_day = {"Fireball"};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("known spells") {
        data.spells_known = {"Cure Wounds"};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("known spells (included)") {
        data.spells_known_included = {"Fireball"};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("spells added to spell list") {
        data.added_to_spell_list = {"Cure Wounds"};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("combination") {
        data.free_cantrips = {"Dancing Lights"};
        data.spells_known = {"Cure Wounds"};
        data.added_to_spell_list = {"Fireball"};
        REQUIRE_FALSE(data.empty());
    }
}

} // namespace dnd::test
