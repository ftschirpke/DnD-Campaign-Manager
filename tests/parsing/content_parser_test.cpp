#include "parsing/content_parser.hpp"

#include <catch2/catch_test_macros.hpp>

#include "controllers/content.hpp"

TEST_CASE("dnd::ContentParser::parseAll: providing invalid directory to parser") {
    dnd::Content content;
    dnd::ContentParser parser("/this/directory/doesnt/exist", "example_campaign", content);
    REQUIRE_THROWS(parser.parseAll());
}