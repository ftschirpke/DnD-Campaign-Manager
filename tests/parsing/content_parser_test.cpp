#include <catch2/catch_test_macros.hpp>

#include "parsing/content_parser.hpp"
#include "models/content_controller.hpp"

TEST_CASE("ContentParser: providing invalid directory to parser") {
    dnd::ContentController controller;
    dnd::ContentParser parser("/this/directory/doesnt/exist", controller);
    REQUIRE_THROWS(parser.parseAll());
}