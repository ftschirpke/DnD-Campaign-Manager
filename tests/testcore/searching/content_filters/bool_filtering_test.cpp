#include <dnd_config.hpp>

#include <core/searching/content_filters/bool_filtering.hpp>

#include <catch2/catch_test_macros.hpp>

static constexpr const char* tags = "[core][validation][effects]";

TEST_CASE("dnd::matches_bool // matching bools with bool filters", tags) {
    SECTION("matching NONE always returns true") {
        REQUIRE(dnd::matches_bool(true, dnd::BoolFilterType::NONE));
        REQUIRE(dnd::matches_bool(false, dnd::BoolFilterType::NONE));
    }

    SECTION("matching IS_TRUE") {
        REQUIRE(dnd::matches_bool(true, dnd::BoolFilterType::IS_TRUE));
        REQUIRE_FALSE(dnd::matches_bool(false, dnd::BoolFilterType::IS_TRUE));
    }

    SECTION("matching IS_FALSE") {
        REQUIRE(dnd::matches_bool(false, dnd::BoolFilterType::IS_FALSE));
        REQUIRE_FALSE(dnd::matches_bool(true, dnd::BoolFilterType::IS_FALSE));
    }
}
