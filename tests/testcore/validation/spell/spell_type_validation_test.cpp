#include <dnd_config.hpp>

#include <core/validation/spell/spell_type_validation.hpp>

#include <array>

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][spell]";

static constexpr std::array<char, 8> allowed_magic_school_chars = {
    'A', 'C', 'D', 'E', 'V', 'I', 'N', 'T',
};

TEST_CASE("SpellType::Data::validate // valid spell type", tags) {
    ValidationDataMock parent;
    SpellType::Data data;
    Errors errors;

    data.ritual = false;

    SECTION("cantrips") {
        for (char c : allowed_magic_school_chars) {
            data.level = 0;
            data.magic_school_char = c;
            REQUIRE_NOTHROW(errors = validate_spell_type(data));
            REQUIRE(errors.ok());
        }
    }

    SECTION("non-cantrip spells") {
        for (char c : allowed_magic_school_chars) {
            for (size_t level = 1; level <= 9; ++level) {
                data.level = 1;
                data.magic_school_char = c;
                REQUIRE_NOTHROW(errors = validate_spell_type(data));
                REQUIRE(errors.ok());

                data.ritual = true;
                REQUIRE_NOTHROW(errors = validate_spell_type(data));
                REQUIRE(errors.ok());
            }
        }
    }
}

TEST_CASE("SpellType::Data::validate // invalid spell type", tags) {
    ValidationDataMock parent;
    SpellType::Data data;
    Errors errors;

    data.ritual = false;

    SECTION("unknown magic school") {
        data.level = 0;
        data.magic_school_char = 'O';
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());

        data.level = 1;
        data.magic_school_char = 'P';
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());

        data.level = 9;
        data.magic_school_char = 'W';
        data.ritual = true;
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("cantrips cannot be rituals") {
        data.level = 0;
        data.magic_school_char = 'A';
        data.ritual = true;
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());

        data.level = 0;
        data.magic_school_char = 'E';
        data.ritual = true;
        REQUIRE_NOTHROW(errors = validate_spell_type(data));
        REQUIRE_FALSE(errors.ok());
    }
}

} // namespace dnd::test
