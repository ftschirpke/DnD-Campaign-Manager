#include <dnd_config.hpp>

#include <core/validation/spell/spell_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/validation/validation_data_mock.hpp>

static constexpr const char* tags = "[core][validation][spell]";

static void set_valid_components_and_type(dnd::SpellData& data) {
    data.components_data.str = "V, S, M (a bit of this and a bit of that)";
    data.type_data.str = "Evocation cantrip";
}

TEST_CASE("dnd::SpellData::validate // valid spells", tags) {
    dnd::SpellData data;
    set_valid_mock_values(data, "Spell");
    set_valid_components_and_type(data);
    dnd::Errors errors;

    SECTION("Spell 1") {
        data.casting_time = "1 action";
        data.range = "120 feet";
        data.duration = "Instantaneous";
        data.classes = {"Sorcerer", "Wizard"};
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());
    }

    SECTION("Spell 2") {
        data.casting_time = "1 bonus action";
        data.range = "150m";
        data.duration = "3 minutes";
        data.classes = {"Bard"};
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());
    }
}

TEST_CASE("dnd::SpellData::validate // invalid spells", tags) {
    dnd::SpellData data;
    set_valid_mock_values(data, "Spell");
    set_valid_components_and_type(data);
    dnd::Errors errors;

    SECTION("Empty casting time") {
        data.casting_time = "";
        data.range = "range";
        data.duration = "duration";
        data.classes = {"class1", "class2"};
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Empty range") {
        data.casting_time = "casting time";
        data.range = "";
        data.duration = "duration";
        data.classes = {"class1", "class2"};
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Empty duration") {
        data.casting_time = "casting time";
        data.range = "range";
        data.duration = "";
        data.classes = {"class1", "class2"};
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("No classes") {
        data.casting_time = "casting time";
        data.range = "range";
        data.duration = "duration";
        data.classes = {};
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }
}
