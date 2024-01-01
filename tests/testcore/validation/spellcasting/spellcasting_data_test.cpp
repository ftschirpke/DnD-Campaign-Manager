#include <dnd_config.hpp>

#include <core/validation/spellcasting/spellcasting_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][class]";

TEST_CASE("Validate Spellcasting // valid spellcasting data", tags) {
    ValidationDataMock parent;
    SpellcastingData data;
    Errors errors;

    SECTION("not a spellcaster") {
        data.is_spellcaster = false;
        REQUIRE_NOTHROW(errors = validate_spellcasting(data));
        REQUIRE(errors.ok());

        // even with data, it should be ok, and the data should be ignored
        data.ability = "INT";
        data.ritual_casting = true;
        data.is_spells_known_type = true;
        data.spells_known.fill(10);
        data.cantrips_known.fill(2);
        data.spell_slots[0].fill(3);
        data.spell_slots[1].fill(2);
        data.spell_slots[2].fill(1);
        REQUIRE_NOTHROW(errors = validate_spellcasting(data));
        REQUIRE(errors.ok());
    }

    SECTION("spells known spellcaster") {
        data.is_spellcaster = true;
        data.ability = "INT";
        data.ritual_casting = true;
        data.is_spells_known_type = true;
        data.spells_known.fill(10);
        data.cantrips_known.fill(2);
        data.spell_slots[0].fill(3);
        data.spell_slots[1].fill(2);
        data.spell_slots[2].fill(1);
        REQUIRE_NOTHROW(errors = validate_spellcasting(data));
        REQUIRE(errors.ok());
    }

    SECTION("half preparation caster") {
        data.is_spellcaster = true;
        data.ability = "WIS";
        data.ritual_casting = false;
        data.is_spells_known_type = false;
        data.preparation_spellcasting_type = "half";
        data.cantrips_known.fill(3);
        data.spell_slots[0].fill(4);
        data.spell_slots[1].fill(3);
        data.spell_slots[2].fill(2);
        data.spell_slots[3].fill(1);
        data.spell_slots[4] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        REQUIRE_NOTHROW(errors = validate_spellcasting(data));
        REQUIRE(errors.ok());
    }

    SECTION("full preparation caster") {
        data.is_spellcaster = true;
        data.ability = "CHA";
        data.ritual_casting = true;
        data.is_spells_known_type = false;
        data.preparation_spellcasting_type = "full";
        data.cantrips_known.fill(4);
        data.spell_slots[0].fill(5);
        data.spell_slots[1].fill(4);
        data.spell_slots[2].fill(4);
        data.spell_slots[3].fill(3);
        data.spell_slots[4].fill(3);
        data.spell_slots[5].fill(2);
        data.spell_slots[6].fill(2);
        data.spell_slots[7].fill(1);
        data.spell_slots[8] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        REQUIRE_NOTHROW(errors = validate_spellcasting(data));
        REQUIRE(errors.ok());
    }
}

TEST_CASE("Validate Spellcasting // invalid spellcasting data", tags) {
    ValidationDataMock parent;

    SpellcastingData data1;
    data1.is_spellcaster = true;
    data1.ability = "INT";
    data1.ritual_casting = true;
    data1.is_spells_known_type = true;
    data1.spells_known.fill(10);
    data1.cantrips_known.fill(2);
    data1.spell_slots[0].fill(3);
    data1.spell_slots[1].fill(2);
    data1.spell_slots[2].fill(1);

    SpellcastingData data2;
    data2.is_spellcaster = true;
    data2.ability = "WIS";
    data2.ritual_casting = false;
    data2.is_spells_known_type = false;
    data2.preparation_spellcasting_type = "half";
    data2.cantrips_known.fill(3);
    data2.spell_slots[0].fill(4);
    data2.spell_slots[1].fill(3);
    data2.spell_slots[2].fill(2);
    data2.spell_slots[3].fill(1);
    data2.spell_slots[4] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    Errors errors;
    REQUIRE_NOTHROW(errors = validate_spellcasting(data1));
    REQUIRE(errors.ok());
    REQUIRE_NOTHROW(errors = validate_spellcasting(data2));
    REQUIRE(errors.ok());

    SECTION("spellcaster but no/invalid ability") {
        data1.ability = "";
        REQUIRE_NOTHROW(errors = validate_spellcasting(data1));
        REQUIRE_FALSE(errors.ok());

        data2.ability = "int";
        REQUIRE_NOTHROW(errors = validate_spellcasting(data2));
        REQUIRE_FALSE(errors.ok());

        data2.ability = "ABC";
        REQUIRE_NOTHROW(errors = validate_spellcasting(data2));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("no/wrong preparation spellcasting type") {
        data2.preparation_spellcasting_type = "";
        REQUIRE_NOTHROW(errors = validate_spellcasting(data2));
        REQUIRE_FALSE(errors.ok());

        data2.preparation_spellcasting_type = "something";
        REQUIRE_NOTHROW(errors = validate_spellcasting(data2));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("spells known and preparation spellcasting type") {
        data1.preparation_spellcasting_type = "half";
        REQUIRE_NOTHROW(errors = validate_spellcasting(data1));
        REQUIRE_FALSE(errors.ok());

        data1.preparation_spellcasting_type = "full";
        REQUIRE_NOTHROW(errors = validate_spellcasting(data1));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("negative cantrips known") {
        data1.cantrips_known.fill(-1);
        REQUIRE_NOTHROW(errors = validate_spellcasting(data1));
        REQUIRE_FALSE(errors.ok());

        data2.cantrips_known[4] = -2;
        REQUIRE_NOTHROW(errors = validate_spellcasting(data2));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("negative spells known") {
        data1.spells_known[6] = -4;
        REQUIRE_NOTHROW(errors = validate_spellcasting(data1));
        REQUIRE_FALSE(errors.ok());

        data2.spells_known.fill(-2);
        REQUIRE_NOTHROW(errors = validate_spellcasting(data2));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("negative spell slot amounts") {
        data1.spell_slots[3][4] = -1;
        REQUIRE_NOTHROW(errors = validate_spellcasting(data1));
        REQUIRE_FALSE(errors.ok());

        data2.spell_slots[8][19] = -2;
        REQUIRE_NOTHROW(errors = validate_spellcasting(data2));
        REQUIRE_FALSE(errors.ok());

        data1.spell_slots[3].fill(-4);
        REQUIRE_NOTHROW(errors = validate_spellcasting(data1));
        REQUIRE_FALSE(errors.ok());

        data2.spell_slots[8].fill(-3);
        REQUIRE_NOTHROW(errors = validate_spellcasting(data2));
        REQUIRE_FALSE(errors.ok());
    }
}

} // namespace dnd::test
