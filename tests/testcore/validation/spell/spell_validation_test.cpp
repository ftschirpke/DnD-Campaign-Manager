#include <dnd_config.hpp>

#include <core/validation/spell/spell_components_validation.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/validation/spell/spell_validation.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][spell]";

TEST_CASE("Validate Spell // valid spells", tags) {
    Spell::Data data;
    set_valid_mock_values(data, "Spell");
    Errors errors;

    SECTION("Spell 1") {
        data.casting_time = "1 action";
        data.range = "120 feet";
        data.duration = "Instantaneous";
        data.classes = {"Sorcerer", "Wizard"};
        REQUIRE_NOTHROW(errors = validate_spell_nonrecursively(data));
        REQUIRE(errors.ok());
    }

    SECTION("Spell 2") {
        data.casting_time = "1 bonus action";
        data.range = "150m";
        data.duration = "3 minutes";
        data.classes = {"Bard"};
        REQUIRE_NOTHROW(errors = validate_spell_nonrecursively(data));
        REQUIRE(errors.ok());
    }
}

TEST_CASE("Validate Spell // invalid spells", tags) {
    Spell::Data data;
    set_valid_mock_values(data, "Spell");
    Errors errors;

    SECTION("Empty casting time") {
        data.casting_time = "";
        data.range = "range";
        data.duration = "duration";
        data.classes = {"class1", "class2"};
        REQUIRE_NOTHROW(errors = validate_spell_nonrecursively(data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Empty range") {
        data.casting_time = "casting time";
        data.range = "";
        data.duration = "duration";
        data.classes = {"class1", "class2"};
        REQUIRE_NOTHROW(errors = validate_spell_nonrecursively(data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Empty duration") {
        data.casting_time = "casting time";
        data.range = "range";
        data.duration = "";
        data.classes = {"class1", "class2"};
        REQUIRE_NOTHROW(errors = validate_spell_nonrecursively(data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("No classes") {
        data.casting_time = "casting time";
        data.range = "range";
        data.duration = "duration";
        data.classes = {};
        REQUIRE_NOTHROW(errors = validate_spell_nonrecursively(data));
        REQUIRE_FALSE(errors.ok());
    }
}

} // namespace dnd::test
