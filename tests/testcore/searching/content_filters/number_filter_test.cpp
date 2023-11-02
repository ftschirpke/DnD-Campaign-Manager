#include <dnd_config.hpp>

#include <core/searching/content_filters/number_filter.hpp>

#include <catch2/catch_test_macros.hpp>

static constexpr const char* tags = "[core][searching][content_filters]";

TEST_CASE("dnd::matches_number // matching numbers with number filters", tags) {
    dnd::NumberFilter<int> int_filter;
    dnd::NumberFilter<float> float_filter;

    SECTION("matching NONE always returns true") {
        int_filter.set_type(dnd::NumberFilterType::NONE);
        int_filter.set_value(1);
        REQUIRE(int_filter.matches(1));
        int_filter.set_value(2);
        REQUIRE(int_filter.matches(-1));

        float_filter.set_type(dnd::NumberFilterType::NONE);
        float_filter.set_value(42.3f);
        REQUIRE(float_filter.matches(42.3f));
        float_filter.set_value(33.3f);
        REQUIRE(float_filter.matches(-140.13f));
    }

    SECTION("matching EQUAL") {
        int_filter.set_type(dnd::NumberFilterType::EQUAL);
        int_filter.set_value(1);
        REQUIRE(int_filter.matches(1));
        int_filter.set_value(-4);
        REQUIRE(int_filter.matches(-4));
        int_filter.set_value(2);
        REQUIRE_FALSE(int_filter.matches(1));
        int_filter.set_value(37);
        REQUIRE_FALSE(int_filter.matches(-14));

        float_filter.set_type(dnd::NumberFilterType::EQUAL);
        float_filter.set_value(7.0f);
        REQUIRE(float_filter.matches(7.0f));
        float_filter.set_value(-1314.0f);
        REQUIRE(float_filter.matches(-1314.0f));
        float_filter.set_value(7.1f);
        REQUIRE_FALSE(float_filter.matches(7.0f));
        float_filter.set_value(171.1771f);
        REQUIRE_FALSE(float_filter.matches(-5432.0f));
    }

    SECTION("matching NOT_EQUAL") {
        int_filter.set_type(dnd::NumberFilterType::NOT_EQUAL);
        int_filter.set_value(2);
        REQUIRE(int_filter.matches(1));
        int_filter.set_value(37);
        REQUIRE(int_filter.matches(-14));
        int_filter.set_value(1);
        REQUIRE_FALSE(int_filter.matches(1));
        int_filter.set_value(-4);
        REQUIRE_FALSE(int_filter.matches(-4));

        float_filter.set_type(dnd::NumberFilterType::NOT_EQUAL);
        float_filter.set_value(7.1f);
        REQUIRE(float_filter.matches(7.0f));
        float_filter.set_value(171.1771f);
        REQUIRE(float_filter.matches(-5432.0f));
        float_filter.set_value(7.0f);
        REQUIRE_FALSE(float_filter.matches(7.0f));
        float_filter.set_value(-1314.0f);
        REQUIRE_FALSE(float_filter.matches(-1314.0f));
    }

    SECTION("matching LESS_THAN") {
        int_filter.set_type(dnd::NumberFilterType::LESS_THAN);
        int_filter.set_value(2);
        REQUIRE(int_filter.matches(1));
        int_filter.set_value(37);
        REQUIRE(int_filter.matches(-14));
        int_filter.set_value(1);
        REQUIRE_FALSE(int_filter.matches(1));
        int_filter.set_value(-4);
        REQUIRE_FALSE(int_filter.matches(-4));
        int_filter.set_value(22);
        REQUIRE_FALSE(int_filter.matches(23));
        int_filter.set_value(-27);
        REQUIRE_FALSE(int_filter.matches(100));

        float_filter.set_type(dnd::NumberFilterType::LESS_THAN);
        float_filter.set_value(7.1f);
        REQUIRE(float_filter.matches(7.0f));
        float_filter.set_value(171.1771f);
        REQUIRE(float_filter.matches(-5432.0f));
        float_filter.set_value(7.0f);
        REQUIRE_FALSE(float_filter.matches(7.0f));
        float_filter.set_value(-1314.0f);
        REQUIRE_FALSE(float_filter.matches(-1314.0f));
        float_filter.set_value(20.3f);
        REQUIRE_FALSE(float_filter.matches(21.7f));
        float_filter.set_value(-84.9f);
        REQUIRE_FALSE(float_filter.matches(514.31f));
    }

    SECTION("matching LESS_THAN_OR_EQUAL") {
        int_filter.set_type(dnd::NumberFilterType::LESS_THAN_OR_EQUAL);
        int_filter.set_value(2);
        REQUIRE(int_filter.matches(1));
        int_filter.set_value(37);
        REQUIRE(int_filter.matches(-14));
        int_filter.set_value(1);
        REQUIRE(int_filter.matches(1));
        int_filter.set_value(-4);
        REQUIRE(int_filter.matches(-4));
        int_filter.set_value(22);
        REQUIRE_FALSE(int_filter.matches(23));
        int_filter.set_value(-27);
        REQUIRE_FALSE(int_filter.matches(100));

        float_filter.set_type(dnd::NumberFilterType::LESS_THAN_OR_EQUAL);
        float_filter.set_value(7.1f);
        REQUIRE(float_filter.matches(7.0f));
        float_filter.set_value(171.1771f);
        REQUIRE(float_filter.matches(-5432.0f));
        float_filter.set_value(7.0f);
        REQUIRE(float_filter.matches(7.0f));
        float_filter.set_value(-1314.0f);
        REQUIRE(float_filter.matches(-1314.0f));
        float_filter.set_value(20.3f);
        REQUIRE_FALSE(float_filter.matches(21.7f));
        float_filter.set_value(-84.9f);
        REQUIRE_FALSE(float_filter.matches(514.31f));
    }

    SECTION("matching GREATER_THAN") {
        int_filter.set_type(dnd::NumberFilterType::GREATER_THAN);
        int_filter.set_value(2);
        REQUIRE_FALSE(int_filter.matches(1));
        int_filter.set_value(37);
        REQUIRE_FALSE(int_filter.matches(-14));
        int_filter.set_value(1);
        REQUIRE_FALSE(int_filter.matches(1));
        int_filter.set_value(-4);
        REQUIRE_FALSE(int_filter.matches(-4));
        int_filter.set_value(22);
        REQUIRE(int_filter.matches(23));
        int_filter.set_value(-27);
        REQUIRE(int_filter.matches(100));

        float_filter.set_type(dnd::NumberFilterType::GREATER_THAN);
        float_filter.set_value(7.1f);
        REQUIRE_FALSE(float_filter.matches(7.0f));
        float_filter.set_value(171.1771f);
        REQUIRE_FALSE(float_filter.matches(-5432.0f));
        float_filter.set_value(7.0f);
        REQUIRE_FALSE(float_filter.matches(7.0f));
        float_filter.set_value(-1314.0f);
        REQUIRE_FALSE(float_filter.matches(-1314.0f));
        float_filter.set_value(20.3f);
        REQUIRE(float_filter.matches(21.7f));
        float_filter.set_value(-84.9f);
        REQUIRE(float_filter.matches(514.31f));
    }

    SECTION("matching GREATER_THAN_OR_EQUAL") {
        int_filter.set_type(dnd::NumberFilterType::GREATER_THAN_OR_EQUAL);
        int_filter.set_value(2);
        REQUIRE_FALSE(int_filter.matches(1));
        int_filter.set_value(37);
        REQUIRE_FALSE(int_filter.matches(-14));
        int_filter.set_value(1);
        REQUIRE(int_filter.matches(1));
        int_filter.set_value(-4);
        REQUIRE(int_filter.matches(-4));
        int_filter.set_value(22);
        REQUIRE(int_filter.matches(23));
        int_filter.set_value(-27);
        REQUIRE(int_filter.matches(100));

        float_filter.set_type(dnd::NumberFilterType::GREATER_THAN_OR_EQUAL);
        float_filter.set_value(7.1f);
        REQUIRE_FALSE(float_filter.matches(7.0f));
        float_filter.set_value(171.1771f);
        REQUIRE_FALSE(float_filter.matches(-5432.0f));
        float_filter.set_value(7.0f);
        REQUIRE(float_filter.matches(7.0f));
        float_filter.set_value(-1314.0f);
        REQUIRE(float_filter.matches(-1314.0f));
        float_filter.set_value(20.3f);
        REQUIRE(float_filter.matches(21.7f));
        float_filter.set_value(-84.9f);
        REQUIRE(float_filter.matches(514.31f));
    }
}
