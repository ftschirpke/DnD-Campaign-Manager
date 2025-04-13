#include <dnd_config.hpp>

#include <core/utils/rich_text.hpp>

#include <catch2/catch_test_macros.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][utils]";

TEST_CASE("parse_rich_text", tags) {
    {
        std::string s = "{@i}";
        std::optional<RichText> parse_result = parse_rich_text(s);
        REQUIRE(!parse_result.has_value());
    }
    {
        std::string s = "{@spell Disguise Self}";
        RichText expected = {
            .rich_type = "spell",
            .text = "Disguise Self",
            .attributes = {},
            .length = s.length(),
        };
        std::optional<RichText> parse_result = parse_rich_text(s);
        REQUIRE(parse_result.has_value());
        REQUIRE(parse_result->rich_type == expected.rich_type);
        REQUIRE(parse_result->text == expected.text);
        REQUIRE(parse_result->attributes == expected.attributes);
        REQUIRE(parse_result->length == expected.length);
    }
    {
        std::string s = "{@i Astral Adventurer's Guide} some suffix 123";
        RichText expected = {
            .rich_type = "i",
            .text = "Astral Adventurer's Guide",
            .attributes = {},
            .length = s.length() - 16,
        };
        std::optional<RichText> parse_result = parse_rich_text(s);
        REQUIRE(parse_result.has_value());
        REQUIRE(parse_result->rich_type == expected.rich_type);
        REQUIRE(parse_result->text == expected.text);
        REQUIRE(parse_result->attributes == expected.attributes);
        REQUIRE(parse_result->length == expected.length);
    }
    {
        std::string s = "{@filter New Chwinga Charms|rewards|source=IDRotF} 23456";
        RichText expected = {
            .rich_type = "filter",
            .text = "New Chwinga Charms",
            .attributes = {{.key = std::nullopt, .value = "rewards"}, {.key = "source", .value = "IDRotF"}},
            .length = s.length() - 6,
        };
        std::optional<RichText> parse_result = parse_rich_text(s);
        REQUIRE(parse_result.has_value());
        REQUIRE(parse_result->rich_type == expected.rich_type);
        REQUIRE(parse_result->text == expected.text);
        REQUIRE(parse_result->attributes == expected.attributes);
        REQUIRE(parse_result->length == expected.length);
    }
}

} // namespace dnd::test
