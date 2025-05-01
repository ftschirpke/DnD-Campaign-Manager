#include <dnd_config.hpp>

#include <core/validation/effects/subholders/extra_spells_holder_validation.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* valid_cantrip1 = "Dancing Lights##dummy";
static constexpr const char* valid_spell1 = "Fireball##dummy";
static constexpr const char* valid_spell2 = "Cure Wounds##dummy";

static constexpr const char* invalid_cantrip1 = "Some other cantrip##dummy";
static constexpr const char* invalid_spell1 = "Some other spell##dummy";

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
        data.free_cantrips = {valid_cantrip1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("at will spells only") {
        data.at_will = {valid_spell1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.at_will.insert(valid_spell2);
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("innate spells only") {
        data.innate = {valid_spell2};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.innate.insert(valid_spell1);
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("free once a day spells only") {
        data.free_once_a_day = {valid_spell1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.free_once_a_day.insert(valid_spell2);
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("known spells only") {
        data.spells_known = {valid_spell2};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.spells_known.insert(valid_spell1);
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("known spells (included) only") {
        data.spells_known_included = {valid_spell1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.spells_known_included.insert(valid_spell2);
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("spells added to spell list only") {
        data.added_to_spell_list = {valid_spell2};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.added_to_spell_list.insert(valid_spell1);
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("combination") {
        data.free_cantrips = {valid_cantrip1};
        data.spells_known = {valid_spell2};
        data.added_to_spell_list = {valid_spell1};
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

        data.free_cantrips = {"", valid_cantrip1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty at will spell names") {
        data.at_will = {""};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.at_will = {"", valid_spell1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty innate spell names") {
        data.innate = {""};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.innate = {"", valid_spell2};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty free once a day spell names") {
        data.free_once_a_day = {""};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.free_once_a_day = {"", valid_spell1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty known spell names") {
        data.spells_known = {""};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.spells_known = {"", valid_spell2};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty known spell (included) names") {
        data.spells_known_included = {""};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.spells_known_included = {"", valid_spell1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("empty added to spell list names") {
        data.added_to_spell_list = {""};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.added_to_spell_list = {"", valid_spell2};
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
        data.free_cantrips = {valid_cantrip1, invalid_cantrip1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown at will spells") {
        data.at_will = {valid_spell1, invalid_spell1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown innate spells") {
        data.innate = {valid_spell2, invalid_spell1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown free once a day spells") {
        data.free_once_a_day = {valid_spell1, invalid_spell1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown known spells") {
        data.spells_known = {valid_spell2, invalid_spell1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown known spells (included)") {
        data.spells_known_included = {valid_spell1, invalid_spell1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown spells added to spell list") {
        data.added_to_spell_list = {valid_spell2, invalid_spell1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("non-cantrip spell as free cantrip") {
        data.free_cantrips = {valid_spell2};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.free_cantrips = {valid_spell2, valid_cantrip1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("cantrip as at will spell") {
        data.at_will = {valid_cantrip1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.at_will = {valid_cantrip1, valid_spell1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("cantrip as at innate spell") {
        data.innate = {valid_cantrip1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.innate = {valid_cantrip1, valid_spell1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("cantrip as free once a day spell") {
        data.free_once_a_day = {valid_cantrip1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.free_once_a_day = {valid_cantrip1, valid_spell2};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("cantrip as known spell") {
        data.spells_known = {valid_cantrip1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.spells_known = {valid_cantrip1, valid_spell2};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("cantrip as known spell (included)") {
        data.spells_known_included = {valid_cantrip1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.spells_known_included = {valid_cantrip1, valid_spell1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("cantrip added to spell list") {
        data.added_to_spell_list = {valid_cantrip1};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.added_to_spell_list = {valid_cantrip1, valid_spell2};
        REQUIRE_NOTHROW(errors = validate_extra_spells_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("ExtraSpellsHolder::Data::empty") {
    ValidationDataMock parent;
    ExtraSpellsHolder::Data data;

    REQUIRE(data.empty());

    SECTION("free cantrips") {
        data.free_cantrips = {valid_cantrip1};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("at will spells") {
        data.at_will = {valid_spell1};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("innate spells") {
        data.innate = {valid_spell2};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("free once a day spells") {
        data.free_once_a_day = {valid_spell1};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("known spells") {
        data.spells_known = {valid_spell2};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("known spells (included)") {
        data.spells_known_included = {valid_spell1};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("spells added to spell list") {
        data.added_to_spell_list = {valid_spell2};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("combination") {
        data.free_cantrips = {valid_cantrip1};
        data.spells_known = {valid_spell2};
        data.added_to_spell_list = {valid_spell1};
        REQUIRE_FALSE(data.empty());
    }
}

} // namespace dnd::test
