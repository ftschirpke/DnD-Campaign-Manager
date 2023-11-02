#include <dnd_config.hpp>

#include <core/searching/content_filters/string_filter.hpp>

#include <catch2/catch_test_macros.hpp>

static constexpr const char* tags = "[core][searching][content_filters]";

TEST_CASE("dnd::matches_string // matching strings with string filters", tags) {
    dnd::StringFilter filter;

    SECTION("matching NONE always returns true") {
        filter.set_type(dnd::StringFilterType::NONE);

        filter.set_value("Another");
        REQUIRE(filter.matches("sOmething"));
        filter.set_value("");
        REQUIRE(filter.matches("else"));
        filter.set_value("somethinG");
        REQUIRE(filter.matches(""));
        filter.set_value("");
        REQUIRE(filter.matches(""));
        filter.set_value("same");
        REQUIRE(filter.matches("same"));
        filter.set_value("World!");
        REQUIRE(filter.matches("Hello World!"));
    }

    SECTION("matching EQUAL") {
        filter.set_type(dnd::StringFilterType::EQUAL);

        filter.set_value("sOmething");
        REQUIRE(filter.matches("sOmething"));
        filter.set_value("else");
        REQUIRE(filter.matches("else"));
        filter.set_value("");
        REQUIRE(filter.matches(""));
        filter.set_value("same");
        REQUIRE(filter.matches("same"));

        filter.set_value("Same");
        REQUIRE_FALSE(filter.matches("same"));
        filter.set_value("World!");
        REQUIRE_FALSE(filter.matches("Hello World!"));
        filter.set_value("Hello");
        REQUIRE_FALSE(filter.matches("Hello World!"));
        filter.set_value("hey?");
        REQUIRE_FALSE(filter.matches("wildly DiFFerent!"));
    }

    SECTION("matching NOT_EQUAL") {
        filter.set_type(dnd::StringFilterType::NOT_EQUAL);

        filter.set_value("Same");
        REQUIRE(filter.matches("same"));
        filter.set_value("World!");
        REQUIRE(filter.matches("Hello World!"));
        filter.set_value("Hello");
        REQUIRE(filter.matches("Hello World!"));
        filter.set_value("hey?");
        REQUIRE(filter.matches("wildly DiFFerent!"));

        filter.set_value("sOmething");
        REQUIRE_FALSE(filter.matches("sOmething"));
        filter.set_value("else");
        REQUIRE_FALSE(filter.matches("else"));
        filter.set_value("");
        REQUIRE_FALSE(filter.matches(""));
        filter.set_value("same");
        REQUIRE_FALSE(filter.matches("same"));
    }

    SECTION("matching CONTAINS") {
        filter.set_type(dnd::StringFilterType::CONTAINS);

        filter.set_value("sOmething");
        REQUIRE(filter.matches("sOmething"));
        filter.set_value("else");
        REQUIRE(filter.matches("says else and"));
        filter.set_value("same");
        REQUIRE(filter.matches("same Here"));
        filter.set_value("World!");
        REQUIRE(filter.matches("Hello World!"));

        filter.set_value("Hi there");
        REQUIRE_FALSE(filter.matches("Hello World!"));
        filter.set_value("hey?");
        REQUIRE_FALSE(filter.matches("wildly DiFFerent!"));
    }

    SECTION("matching NOT_CONTAINS") {
        filter.set_type(dnd::StringFilterType::NOT_CONTAINS);

        filter.set_value("Hi there");
        REQUIRE(filter.matches("Hello World!"));
        filter.set_value("hey?");
        REQUIRE(filter.matches("wildly DiFFerent!"));

        filter.set_value("sOmething");
        REQUIRE_FALSE(filter.matches("sOmething"));
        filter.set_value("else");
        REQUIRE_FALSE(filter.matches("says else and"));
        filter.set_value("same");
        REQUIRE_FALSE(filter.matches("same Here"));
        filter.set_value("World!");
        REQUIRE_FALSE(filter.matches("Hello World!"));
    }

    SECTION("matching STARTS_WITH") {
        filter.set_type(dnd::StringFilterType::STARTS_WITH);

        filter.set_value("sOmething");
        REQUIRE(filter.matches("sOmething"));
        filter.set_value("same");
        REQUIRE(filter.matches("same Here"));

        filter.set_value("else");
        REQUIRE_FALSE(filter.matches("says else and"));
        filter.set_value("World!");
        REQUIRE_FALSE(filter.matches("Hello World!"));
        filter.set_value("Hi there");
        REQUIRE_FALSE(filter.matches("Hello World!"));
        filter.set_value("hey?");
        REQUIRE_FALSE(filter.matches("wildly DiFFerent!"));
    }

    SECTION("matching NOT_STARTS_WITH") {
        filter.set_type(dnd::StringFilterType::NOT_STARTS_WITH);

        filter.set_value("else");
        REQUIRE(filter.matches("says else and"));
        filter.set_value("World!");
        REQUIRE(filter.matches("Hello World!"));
        filter.set_value("Hi there");
        REQUIRE(filter.matches("Hello World!"));
        filter.set_value("hey?");
        REQUIRE(filter.matches("wildly DiFFerent!"));

        filter.set_value("sOmething");
        REQUIRE_FALSE(filter.matches("sOmething"));
        filter.set_value("same");
        REQUIRE_FALSE(filter.matches("same Here"));
    }

    SECTION("matching ENDS_WITH") {
        filter.set_type(dnd::StringFilterType::ENDS_WITH);

        filter.set_value("sOmething");
        REQUIRE(filter.matches("sOmething"));
        filter.set_value("World!");
        REQUIRE(filter.matches("Hello World!"));

        filter.set_value("else");
        REQUIRE_FALSE(filter.matches("says else and"));
        filter.set_value("same");
        REQUIRE_FALSE(filter.matches("same Here"));
        filter.set_value("Hi there");
        REQUIRE_FALSE(filter.matches("Hello World!"));
        filter.set_value("hey?");
        REQUIRE_FALSE(filter.matches("wildly DiFFerent!"));
    }

    SECTION("matching NOT_ENDS_WITH") {
        filter.set_type(dnd::StringFilterType::NOT_ENDS_WITH);

        filter.set_value("else");
        REQUIRE(filter.matches("says else and"));
        filter.set_value("same");
        REQUIRE(filter.matches("same Here"));
        filter.set_value("Hi there");
        REQUIRE(filter.matches("Hello World!"));
        filter.set_value("hey?");
        REQUIRE(filter.matches("wildly DiFFerent!"));

        filter.set_value("sOmething");
        REQUIRE_FALSE(filter.matches("sOmething"));
        filter.set_value("World!");
        REQUIRE_FALSE(filter.matches("Hello World!"));
    }
}
