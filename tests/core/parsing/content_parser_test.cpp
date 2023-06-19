#include <core/parsing/content_parser.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>

TEST_CASE("dnd::ContentParser::parse: providing invalid directory to parser") {
    dnd::ContentParser parser;
    dnd::ParsingResult result = parser.parse("/this/directory/doesnt/exist", "example_campaign");
    REQUIRE(!result.errors.ok());
}
