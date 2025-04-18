#include <dnd_config.hpp>

#include <core/parsing/content_parsing.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][parsing]";

TEST_CASE("parse_content // providing invalid directory to parsing function", tags) {
    std::set<std::filesystem::path> content_paths = {"/this/directory/doesnt/exist"};
    ParsingResult result = parse_content(content_paths);
    REQUIRE_FALSE(result.errors.ok());
}

} // namespace dnd::test
