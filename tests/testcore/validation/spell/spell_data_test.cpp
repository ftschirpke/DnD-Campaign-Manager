#include <dnd_config.hpp>

#include <core/validation/spell/spell_components_data.hpp>

#include <cassert>

#include <catch2/catch_test_macros.hpp>

#include <core/validation/spell/spell_data.hpp>
#include <core/validation/spell/spell_type_data.hpp>
#include <testcore/validation/validation_data_mock.hpp>

static constexpr const char* tags = "[core][validation][spell]";

static constexpr const char* fixed_components = "V, S, M (a bit of this and a bit of that)";
static constexpr const char* fixed_type = "Evocation cantrip";

static bool validate_fixed_values() {
    dndtest::ValidationDataMock parent;
    dnd::SpellComponentsData components_data(&parent);
    components_data.str = fixed_components;
    dnd::SpellTypeData type_data(&parent);
    type_data.str = fixed_type;
    assert(components_data.validate().ok());
    assert(type_data.validate().ok());
    return components_data.validate().ok() && type_data.validate().ok();
}

static void set_valid_components_and_type(dnd::SpellData& data) {
#if DND_DEBUG
    static bool fixed_values_valid = validate_fixed_values();
    assert(fixed_values_valid);
#endif
    data.components_data.str = fixed_components;
    data.type_data.str = fixed_type;
}

TEST_CASE("dnd::SpellData::validate // valid spells", tags) {
    dnd::SpellData data;
    dndtest::set_valid_mock_values(data, "Spell");
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
    dndtest::set_valid_mock_values(data, "Spell");
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
