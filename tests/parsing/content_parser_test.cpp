#include "parsing/content_parser.hpp"

#include <catch2/catch_test_macros.hpp>

#include "controllers/content_controller.hpp"

TEST_CASE("dnd::ContentParser::parseAll: providing invalid directory to parser") {
    dnd::ContentController controller;
    dnd::ContentParser parser("/this/directory/doesnt/exist", "example_campaign", controller);
    REQUIRE_THROWS(parser.parseAll());
}