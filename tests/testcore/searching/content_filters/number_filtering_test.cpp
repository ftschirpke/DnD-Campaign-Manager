#include <dnd_config.hpp>

#include <core/searching/content_filters/number_filtering.hpp>

#include <catch2/catch_test_macros.hpp>

static constexpr const char* tags = "[core][searching][content_filters]";

TEST_CASE("dnd::matches_number // matching numbers with number filters", tags) {
    SECTION("matching NONE always returns true") {
        REQUIRE(dnd::matches_number(1, dnd::NumberFilterType::NONE, 1));
        REQUIRE(dnd::matches_number(-1, dnd::NumberFilterType::NONE, 2));
        REQUIRE(dnd::matches_number(42.3f, dnd::NumberFilterType::NONE, 42.3f));
        REQUIRE(dnd::matches_number(-140.13f, dnd::NumberFilterType::NONE, 33.3f));
    }

    SECTION("matching EQUAL") {
        REQUIRE(dnd::matches_number(1, dnd::NumberFilterType::EQUAL, 1));
        REQUIRE(dnd::matches_number(-4, dnd::NumberFilterType::EQUAL, -4));
        REQUIRE_FALSE(dnd::matches_number(1, dnd::NumberFilterType::EQUAL, 2));
        REQUIRE_FALSE(dnd::matches_number(-14, dnd::NumberFilterType::EQUAL, 37));

        REQUIRE(dnd::matches_number(7.0f, dnd::NumberFilterType::EQUAL, 7.0f));
        REQUIRE(dnd::matches_number(-1314.0f, dnd::NumberFilterType::EQUAL, -1314.0f));
        REQUIRE_FALSE(dnd::matches_number(7.0f, dnd::NumberFilterType::EQUAL, 7.1f));
        REQUIRE_FALSE(dnd::matches_number(-5432.0f, dnd::NumberFilterType::EQUAL, 171.1771f));
    }

    SECTION("matching NOT_EQUAL") {
        REQUIRE(dnd::matches_number(1, dnd::NumberFilterType::NOT_EQUAL, 2));
        REQUIRE(dnd::matches_number(-14, dnd::NumberFilterType::NOT_EQUAL, 37));
        REQUIRE_FALSE(dnd::matches_number(1, dnd::NumberFilterType::NOT_EQUAL, 1));
        REQUIRE_FALSE(dnd::matches_number(-4, dnd::NumberFilterType::NOT_EQUAL, -4));

        REQUIRE(dnd::matches_number(7.0f, dnd::NumberFilterType::NOT_EQUAL, 7.1f));
        REQUIRE(dnd::matches_number(-5432.0f, dnd::NumberFilterType::NOT_EQUAL, 171.1771f));
        REQUIRE_FALSE(dnd::matches_number(7.0f, dnd::NumberFilterType::NOT_EQUAL, 7.0f));
        REQUIRE_FALSE(dnd::matches_number(-1314.0f, dnd::NumberFilterType::NOT_EQUAL, -1314.0f));
    }

    SECTION("matching LESS_THAN") {
        REQUIRE(dnd::matches_number(1, dnd::NumberFilterType::LESS_THAN, 2));
        REQUIRE(dnd::matches_number(-14, dnd::NumberFilterType::LESS_THAN, 37));
        REQUIRE_FALSE(dnd::matches_number(1, dnd::NumberFilterType::LESS_THAN, 1));
        REQUIRE_FALSE(dnd::matches_number(-4, dnd::NumberFilterType::LESS_THAN, -4));
        REQUIRE_FALSE(dnd::matches_number(23, dnd::NumberFilterType::LESS_THAN, 22));
        REQUIRE_FALSE(dnd::matches_number(100, dnd::NumberFilterType::LESS_THAN, -27));

        REQUIRE(dnd::matches_number(7.0f, dnd::NumberFilterType::LESS_THAN, 7.1f));
        REQUIRE(dnd::matches_number(-5432.0f, dnd::NumberFilterType::LESS_THAN, 171.1771f));
        REQUIRE_FALSE(dnd::matches_number(7.0f, dnd::NumberFilterType::LESS_THAN, 7.0f));
        REQUIRE_FALSE(dnd::matches_number(-1314.0f, dnd::NumberFilterType::LESS_THAN, -1314.0f));
        REQUIRE_FALSE(dnd::matches_number(21.7f, dnd::NumberFilterType::LESS_THAN, 20.3f));
        REQUIRE_FALSE(dnd::matches_number(514.31f, dnd::NumberFilterType::LESS_THAN, -84.9f));
    }

    SECTION("matching LESS_THAN_OR_EQUAL") {
        REQUIRE(dnd::matches_number(1, dnd::NumberFilterType::LESS_THAN_OR_EQUAL, 2));
        REQUIRE(dnd::matches_number(-14, dnd::NumberFilterType::LESS_THAN_OR_EQUAL, 37));
        REQUIRE(dnd::matches_number(1, dnd::NumberFilterType::LESS_THAN_OR_EQUAL, 1));
        REQUIRE(dnd::matches_number(-4, dnd::NumberFilterType::LESS_THAN_OR_EQUAL, -4));
        REQUIRE_FALSE(dnd::matches_number(23, dnd::NumberFilterType::LESS_THAN_OR_EQUAL, 22));
        REQUIRE_FALSE(dnd::matches_number(100, dnd::NumberFilterType::LESS_THAN_OR_EQUAL, -27));

        REQUIRE(dnd::matches_number(7.0f, dnd::NumberFilterType::LESS_THAN_OR_EQUAL, 7.1f));
        REQUIRE(dnd::matches_number(-5432.0f, dnd::NumberFilterType::LESS_THAN_OR_EQUAL, 171.1771f));
        REQUIRE(dnd::matches_number(7.0f, dnd::NumberFilterType::LESS_THAN_OR_EQUAL, 7.0f));
        REQUIRE(dnd::matches_number(-1314.0f, dnd::NumberFilterType::LESS_THAN_OR_EQUAL, -1314.0f));
        REQUIRE_FALSE(dnd::matches_number(21.7f, dnd::NumberFilterType::LESS_THAN_OR_EQUAL, 20.3f));
        REQUIRE_FALSE(dnd::matches_number(514.31f, dnd::NumberFilterType::LESS_THAN_OR_EQUAL, -84.9f));
    }

    SECTION("matching GREATER_THAN") {
        REQUIRE_FALSE(dnd::matches_number(1, dnd::NumberFilterType::GREATER_THAN, 2));
        REQUIRE_FALSE(dnd::matches_number(-14, dnd::NumberFilterType::GREATER_THAN, 37));
        REQUIRE_FALSE(dnd::matches_number(1, dnd::NumberFilterType::GREATER_THAN, 1));
        REQUIRE_FALSE(dnd::matches_number(-4, dnd::NumberFilterType::GREATER_THAN, -4));
        REQUIRE(dnd::matches_number(23, dnd::NumberFilterType::GREATER_THAN, 22));
        REQUIRE(dnd::matches_number(100, dnd::NumberFilterType::GREATER_THAN, -27));

        REQUIRE_FALSE(dnd::matches_number(7.0f, dnd::NumberFilterType::GREATER_THAN, 7.1f));
        REQUIRE_FALSE(dnd::matches_number(-5432.0f, dnd::NumberFilterType::GREATER_THAN, 171.1771f));
        REQUIRE_FALSE(dnd::matches_number(7.0f, dnd::NumberFilterType::GREATER_THAN, 7.0f));
        REQUIRE_FALSE(dnd::matches_number(-1314.0f, dnd::NumberFilterType::GREATER_THAN, -1314.0f));
        REQUIRE(dnd::matches_number(21.7f, dnd::NumberFilterType::GREATER_THAN, 20.3f));
        REQUIRE(dnd::matches_number(514.31f, dnd::NumberFilterType::GREATER_THAN, -84.9f));
    }

    SECTION("matching GREATER_THAN_OR_EQUAL") {
        REQUIRE_FALSE(dnd::matches_number(1, dnd::NumberFilterType::GREATER_THAN_OR_EQUAL, 2));
        REQUIRE_FALSE(dnd::matches_number(-14, dnd::NumberFilterType::GREATER_THAN_OR_EQUAL, 37));
        REQUIRE(dnd::matches_number(1, dnd::NumberFilterType::GREATER_THAN_OR_EQUAL, 1));
        REQUIRE(dnd::matches_number(-4, dnd::NumberFilterType::GREATER_THAN_OR_EQUAL, -4));
        REQUIRE(dnd::matches_number(23, dnd::NumberFilterType::GREATER_THAN_OR_EQUAL, 22));
        REQUIRE(dnd::matches_number(100, dnd::NumberFilterType::GREATER_THAN_OR_EQUAL, -27));

        REQUIRE_FALSE(dnd::matches_number(7.0f, dnd::NumberFilterType::GREATER_THAN_OR_EQUAL, 7.1f));
        REQUIRE_FALSE(dnd::matches_number(-5432.0f, dnd::NumberFilterType::GREATER_THAN_OR_EQUAL, 171.1771f));
        REQUIRE(dnd::matches_number(7.0f, dnd::NumberFilterType::GREATER_THAN_OR_EQUAL, 7.0f));
        REQUIRE(dnd::matches_number(-1314.0f, dnd::NumberFilterType::GREATER_THAN_OR_EQUAL, -1314.0f));
        REQUIRE(dnd::matches_number(21.7f, dnd::NumberFilterType::GREATER_THAN_OR_EQUAL, 20.3f));
        REQUIRE(dnd::matches_number(514.31f, dnd::NumberFilterType::GREATER_THAN_OR_EQUAL, -84.9f));
    }
}
