#include <dnd_config.hpp>

#include <core/searching/content_filters/bool_filter.hpp>

#include <catch2/catch_test_macros.hpp>

static constexpr const char* tags = "[core][searching][content_filters]";

TEST_CASE("dnd::matches_bool // matching bools with bool filters", tags) {
    dnd::BoolFilter filter;

    SECTION("matching NONE always returns true") {
        filter.set_type(dnd::BoolFilterType::NONE);
        REQUIRE(filter.matches(true));
        REQUIRE(filter.matches(false));
    }

    SECTION("matching IS_TRUE") {
        filter.set_type(dnd::BoolFilterType::IS_TRUE);
        REQUIRE(filter.matches(true));
        REQUIRE_FALSE(filter.matches(false));
    }

    SECTION("matching IS_FALSE") {
        filter.set_type(dnd::BoolFilterType::IS_FALSE);
        REQUIRE(filter.matches(false));
        REQUIRE_FALSE(filter.matches(true));
    }
}
