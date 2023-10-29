#include <dnd_config.hpp>

#include <core/searching/content_filters/string_filtering.hpp>

#include <catch2/catch_test_macros.hpp>

static constexpr const char* tags = "[core][searching][content_filters]";

TEST_CASE("dnd::matches_string // matching strings with string filters", tags) {
    SECTION("matching NONE always returns true") {
        REQUIRE(dnd::matches_string("sOmething", dnd::StringFilterType::NONE, "Another"));
        REQUIRE(dnd::matches_string("else", dnd::StringFilterType::NONE, ""));
        REQUIRE(dnd::matches_string("", dnd::StringFilterType::NONE, "somethinG"));
        REQUIRE(dnd::matches_string("", dnd::StringFilterType::NONE, ""));
        REQUIRE(dnd::matches_string("same", dnd::StringFilterType::NONE, "same"));
        REQUIRE(dnd::matches_string("Hello World!", dnd::StringFilterType::NONE, "World!"));
    }

    SECTION("matching EQUAL") {
        REQUIRE(dnd::matches_string("sOmething", dnd::StringFilterType::EQUAL, "sOmething"));
        REQUIRE(dnd::matches_string("else", dnd::StringFilterType::EQUAL, "else"));
        REQUIRE(dnd::matches_string("", dnd::StringFilterType::EQUAL, ""));
        REQUIRE(dnd::matches_string("same", dnd::StringFilterType::EQUAL, "same"));
        REQUIRE_FALSE(dnd::matches_string("same", dnd::StringFilterType::EQUAL, "Same"));
        REQUIRE_FALSE(dnd::matches_string("Hello World!", dnd::StringFilterType::EQUAL, "World!"));
        REQUIRE_FALSE(dnd::matches_string("Hello World!", dnd::StringFilterType::EQUAL, "Hello"));
        REQUIRE_FALSE(dnd::matches_string("wildly DiFFerent!", dnd::StringFilterType::EQUAL, "hey?"));
    }

    SECTION("matching NOT_EQUAL") {
        REQUIRE_FALSE(dnd::matches_string("sOmething", dnd::StringFilterType::NOT_EQUAL, "sOmething"));
        REQUIRE_FALSE(dnd::matches_string("else", dnd::StringFilterType::NOT_EQUAL, "else"));
        REQUIRE_FALSE(dnd::matches_string("", dnd::StringFilterType::NOT_EQUAL, ""));
        REQUIRE_FALSE(dnd::matches_string("same", dnd::StringFilterType::NOT_EQUAL, "same"));
        REQUIRE(dnd::matches_string("same", dnd::StringFilterType::NOT_EQUAL, "Same"));
        REQUIRE(dnd::matches_string("Hello World!", dnd::StringFilterType::NOT_EQUAL, "World!"));
        REQUIRE(dnd::matches_string("Hello World!", dnd::StringFilterType::NOT_EQUAL, "Hello"));
        REQUIRE(dnd::matches_string("wildly DiFFerent!", dnd::StringFilterType::NOT_EQUAL, "hey?"));
    }

    SECTION("matching CONTAINS") {
        REQUIRE(dnd::matches_string("sOmething", dnd::StringFilterType::CONTAINS, "sOmething"));
        REQUIRE(dnd::matches_string("says else and", dnd::StringFilterType::CONTAINS, "else"));
        REQUIRE(dnd::matches_string("same Here", dnd::StringFilterType::CONTAINS, "same"));
        REQUIRE(dnd::matches_string("Hello World!", dnd::StringFilterType::CONTAINS, "World!"));
        REQUIRE_FALSE(dnd::matches_string("Hello World!", dnd::StringFilterType::CONTAINS, "Hi there"));
        REQUIRE_FALSE(dnd::matches_string("wildly DiFFerent!", dnd::StringFilterType::CONTAINS, "hey?"));
    }

    SECTION("matching NOT_CONTAINS") {
        REQUIRE_FALSE(dnd::matches_string("sOmething", dnd::StringFilterType::NOT_CONTAINS, "sOmething"));
        REQUIRE_FALSE(dnd::matches_string("says else and", dnd::StringFilterType::NOT_CONTAINS, "else"));
        REQUIRE_FALSE(dnd::matches_string("same Here", dnd::StringFilterType::NOT_CONTAINS, "same"));
        REQUIRE_FALSE(dnd::matches_string("Hello World!", dnd::StringFilterType::NOT_CONTAINS, "World!"));
        REQUIRE(dnd::matches_string("Hello World!", dnd::StringFilterType::NOT_CONTAINS, "Hi there"));
        REQUIRE(dnd::matches_string("wildly DiFFerent!", dnd::StringFilterType::NOT_CONTAINS, "hey?"));
    }

    SECTION("matching STARTS_WITH") {
        REQUIRE(dnd::matches_string("sOmething", dnd::StringFilterType::STARTS_WITH, "sOmething"));
        REQUIRE_FALSE(dnd::matches_string("says else and", dnd::StringFilterType::STARTS_WITH, "else"));
        REQUIRE(dnd::matches_string("same Here", dnd::StringFilterType::STARTS_WITH, "same"));
        REQUIRE_FALSE(dnd::matches_string("Hello World!", dnd::StringFilterType::STARTS_WITH, "World!"));
        REQUIRE_FALSE(dnd::matches_string("Hello World!", dnd::StringFilterType::STARTS_WITH, "Hi there"));
        REQUIRE_FALSE(dnd::matches_string("wildly DiFFerent!", dnd::StringFilterType::STARTS_WITH, "hey?"));
    }

    SECTION("matching NOT_STARTS_WITH") {
        REQUIRE_FALSE(dnd::matches_string("sOmething", dnd::StringFilterType::NOT_STARTS_WITH, "sOmething"));
        REQUIRE(dnd::matches_string("says else and", dnd::StringFilterType::NOT_STARTS_WITH, "else"));
        REQUIRE_FALSE(dnd::matches_string("same Here", dnd::StringFilterType::NOT_STARTS_WITH, "same"));
        REQUIRE(dnd::matches_string("Hello World!", dnd::StringFilterType::NOT_STARTS_WITH, "World!"));
        REQUIRE(dnd::matches_string("Hello World!", dnd::StringFilterType::NOT_STARTS_WITH, "Hi there"));
        REQUIRE(dnd::matches_string("wildly DiFFerent!", dnd::StringFilterType::NOT_STARTS_WITH, "hey?"));
    }

    SECTION("matching ENDS_WITH") {
        REQUIRE(dnd::matches_string("sOmething", dnd::StringFilterType::ENDS_WITH, "sOmething"));
        REQUIRE_FALSE(dnd::matches_string("says else and", dnd::StringFilterType::ENDS_WITH, "else"));
        REQUIRE_FALSE(dnd::matches_string("same Here", dnd::StringFilterType::ENDS_WITH, "same"));
        REQUIRE(dnd::matches_string("Hello World!", dnd::StringFilterType::ENDS_WITH, "World!"));
        REQUIRE_FALSE(dnd::matches_string("Hello World!", dnd::StringFilterType::ENDS_WITH, "Hi there"));
        REQUIRE_FALSE(dnd::matches_string("wildly DiFFerent!", dnd::StringFilterType::ENDS_WITH, "hey?"));
    }

    SECTION("matching NOT_ENDS_WITH") {
        REQUIRE_FALSE(dnd::matches_string("sOmething", dnd::StringFilterType::NOT_ENDS_WITH, "sOmething"));
        REQUIRE(dnd::matches_string("says else and", dnd::StringFilterType::NOT_ENDS_WITH, "else"));
        REQUIRE(dnd::matches_string("same Here", dnd::StringFilterType::NOT_ENDS_WITH, "same"));
        REQUIRE_FALSE(dnd::matches_string("Hello World!", dnd::StringFilterType::NOT_ENDS_WITH, "World!"));
        REQUIRE(dnd::matches_string("Hello World!", dnd::StringFilterType::NOT_ENDS_WITH, "Hi there"));
        REQUIRE(dnd::matches_string("wildly DiFFerent!", dnd::StringFilterType::NOT_ENDS_WITH, "hey?"));
    }
}
