#include <dnd_config.hpp>

#include <core/validation/spell/spell_type_data.hpp>

#include <array>
#include <string>

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][spell]";

static constexpr std::array<const char*, 16> allowed_magic_school_spellings = {
    "abjuration",  "Abjuration",  "conjuration",   "Conjuration",   "divination", "Divination",
    "enchantment", "Enchantment", "evocation",     "Evocation",     "illusion",   "Illusion",
    "necromancy",  "Necromancy",  "transmutation", "Transmutation",
};

static constexpr std::array<const char*, 9> spell_levels = {"1st", "2nd", "3rd", "4th", "5th",
                                                            "6th", "7th", "8th", "9th"};

TEST_CASE("SpellTypeData::validate // valid spell type", tags) {
    ValidationDataMock parent;
    SpellTypeData data;
    Errors errors;
    SECTION("cantrips") {
        for (const char* spelling : allowed_magic_school_spellings) {
            data.str = fmt::format("{} cantrip", spelling);
            REQUIRE_NOTHROW(errors = validate_spell_type(data));
            REQUIRE(errors.ok());
        }
    }

    SECTION("non-cantrip spells") {
        for (const char* spelling : allowed_magic_school_spellings) {
            for (size_t level = 1; level <= 9; ++level) {
                data.str = fmt::format("{}-level {}", spell_levels[level - 1], spelling);
                REQUIRE_NOTHROW(errors = validate_spell_type(data));
                REQUIRE(errors.ok());

                data.str += " (ritual)";
                REQUIRE_NOTHROW(errors = validate_spell_type(data));
                REQUIRE(errors.ok());
            }
        }
    }
}

TEST_CASE("SpellTypeData::validate // invalid spell type", tags) {
    ValidationDataMock parent;
    SpellTypeData data;
    Errors errors;
    SECTION("other formats than standard D&D format not allowed") {
        data.str = "Level 9 illusion";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());

        data.str = "Cantrip - enchantment";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());

        data.str = "0-level divination";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());

        data.str = "9 level conjuration";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown magic school") {
        data.str = "Disappointment cantrip";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());

        data.str = "1st-level aspiration";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());

        data.str = "9th-level intention (ritual)";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("cantrips cannot be rituals") {
        data.str = "Abjuration cantrip (ritual)";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());

        data.str = "enchantment cantrip (ritual)";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("when provided, parentheses must contain 'ritual'") {
        data.str = "1st-level Enchantment (something)";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());

        data.str = "8th-level illusion (ritula)";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());

        data.str = "5th-level evocation (Ritual)";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("parentheses must be after a whitespace and need to open and close") {
        data.str = "2nd-level illusion(ritual)";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());

        data.str = "3rd-level Divination (ritual";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());

        data.str = "5th-level ritual)";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("wrong capitalisation or uppercase in-word") {
        data.str = "Abjuration Cantrip";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());

        data.str = "divination CanTrip";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());

        data.str = "ILLusion cantrip";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());

        data.str = "enchantment Cantrip";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());

        data.str = "3rD-level Divination";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());

        data.str = "5Th-level (ritual)";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());

        data.str = "2nd-LEVEL illusion";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());

        data.str = "4th-Level necromancy";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());

        data.str = "1st-level TransMutation (ritual)";
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());
    }
}

} // namespace dnd::test
