#include <dnd_config.hpp>

#include <core/validation/spell/spell_components_data.hpp>

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][spell]";

TEST_CASE("SpellComponentsData::validate // valid components", tags) {
    ValidationDataMock parent;
    SpellComponentsData data(&parent);
    Errors errors;

    std::string components_string = "A bit of this and a bit of that";

    SECTION("V") {
        data.str = "V";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());
    }
    SECTION("S") {
        data.str = "S";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());
    }
    SECTION("M") {
        data.str = fmt::format("M ({})", components_string);
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());
    }
    SECTION("V, S") {
        data.str = "V, S";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());
    }
    SECTION("V, M") {
        data.str = fmt::format("V, M ({})", components_string);
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());
    }
    SECTION("S, M") {
        data.str = fmt::format("S, M ({})", components_string);
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());
    }
    SECTION("V, S, M") {
        data.str = fmt::format("V, S, M ({})", components_string);
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());
    }
}

TEST_CASE("SpellComponentsData::validate // invalid components", tags) {
    ValidationDataMock parent;
    SpellComponentsData data(&parent);
    Errors errors;

    SECTION("completely wrong letters not allowed") {
        data.str = "A, B";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.str = "öikje";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("M needs an components string in parentheses") {
        data.str = "M";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.str = "V, M";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.str = "S, M";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.str = "V, S, M";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("but parentheses only when M is provided") {
        data.str = "V, S (something)";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("parentheses must be after a whitespace and need to open and close") {
        data.str = "V, M(another thing)";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.str = "M (nothing";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.str = "M and another)";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("letters VSM must be uppercase") {
        data.str = "m (a thread)";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.str = "v, s";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.str = "v, S";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.str = "V, s";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.str = "v, M (everything)";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }
}

} // namespace dnd::test
