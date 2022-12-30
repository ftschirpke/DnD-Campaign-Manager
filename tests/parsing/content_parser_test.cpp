#include "parsing/content_parser.hpp"

#include <catch2/catch_test_macros.hpp>

#include "controllers/content.hpp"

TEST_CASE("dnd::ContentParser::parse: providing invalid directory to parser") {
    dnd::ContentParser parser;
    REQUIRE_THROWS(parser.parse("/this/directory/doesnt/exist", "example_campaign"));
}