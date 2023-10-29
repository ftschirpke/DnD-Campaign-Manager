#include <dnd_config.hpp>

#include <core/searching/content_filters/selection_filtering.hpp>

#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>

static constexpr const char* tags = "[core][searching][content_filters]";

enum class CustomType {
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
};

TEST_CASE("dnd::matches_selection // matching selections with selection filters", tags) {
    std::vector<int> int_selection = {-14, 4, 713};
    std::vector<float> float_selection = {-18.13f, 0.0f, 1.13f, 2008.71f};
    std::vector<std::string> string_selection1 = {"Hello", "World", "!"};
    std::vector<std::string> string_selection2 = {"How", "are", "you", "?"};
    std::vector<CustomType> custom_selection1 = {CustomType::A, CustomType::B, CustomType::C};
    std::vector<CustomType> custom_selection2 = {CustomType::D, CustomType::E, CustomType::F, CustomType::G};

    SECTION("matching NONE always returns true") {
        REQUIRE(dnd::matches_selection(4, dnd::SelectionFilterType::NONE, int_selection));
        REQUIRE(dnd::matches_selection(-7, dnd::SelectionFilterType::NONE, int_selection));
        REQUIRE(dnd::matches_selection(-18.13f, dnd::SelectionFilterType::NONE, float_selection));
        REQUIRE(dnd::matches_selection(42.13f, dnd::SelectionFilterType::NONE, float_selection));
        REQUIRE(dnd::matches_selection<std::string>("Hello", dnd::SelectionFilterType::NONE, string_selection1));
        REQUIRE(dnd::matches_selection<std::string>("are", dnd::SelectionFilterType::NONE, string_selection1));
        REQUIRE(dnd::matches_selection<std::string>("How", dnd::SelectionFilterType::NONE, string_selection2));
        REQUIRE(dnd::matches_selection<std::string>("!", dnd::SelectionFilterType::NONE, string_selection2));
        REQUIRE(dnd::matches_selection(CustomType::A, dnd::SelectionFilterType::NONE, custom_selection1));
        REQUIRE(dnd::matches_selection(CustomType::D, dnd::SelectionFilterType::NONE, custom_selection1));
        REQUIRE(dnd::matches_selection(CustomType::C, dnd::SelectionFilterType::NONE, custom_selection2));
        REQUIRE(dnd::matches_selection(CustomType::F, dnd::SelectionFilterType::NONE, custom_selection2));
        REQUIRE(dnd::matches_selection(CustomType::H, dnd::SelectionFilterType::NONE, custom_selection1));
        REQUIRE(dnd::matches_selection(CustomType::H, dnd::SelectionFilterType::NONE, custom_selection2));
    }

    SECTION("matching IS_IN") {
        REQUIRE(dnd::matches_selection(4, dnd::SelectionFilterType::IS_IN, int_selection));
        REQUIRE_FALSE(dnd::matches_selection(-7, dnd::SelectionFilterType::IS_IN, int_selection));
        REQUIRE(dnd::matches_selection(-18.13f, dnd::SelectionFilterType::IS_IN, float_selection));
        REQUIRE_FALSE(dnd::matches_selection(42.13f, dnd::SelectionFilterType::IS_IN, float_selection));
        REQUIRE(dnd::matches_selection<std::string>("Hello", dnd::SelectionFilterType::IS_IN, string_selection1));
        REQUIRE_FALSE(dnd::matches_selection<std::string>("are", dnd::SelectionFilterType::IS_IN, string_selection1));
        REQUIRE(dnd::matches_selection<std::string>("How", dnd::SelectionFilterType::IS_IN, string_selection2));
        REQUIRE_FALSE(dnd::matches_selection<std::string>("!", dnd::SelectionFilterType::IS_IN, string_selection2));
        REQUIRE(dnd::matches_selection(CustomType::A, dnd::SelectionFilterType::IS_IN, custom_selection1));
        REQUIRE_FALSE(dnd::matches_selection(CustomType::D, dnd::SelectionFilterType::IS_IN, custom_selection1));
        REQUIRE_FALSE(dnd::matches_selection(CustomType::C, dnd::SelectionFilterType::IS_IN, custom_selection2));
        REQUIRE(dnd::matches_selection(CustomType::F, dnd::SelectionFilterType::IS_IN, custom_selection2));
        REQUIRE_FALSE(dnd::matches_selection(CustomType::H, dnd::SelectionFilterType::IS_IN, custom_selection1));
        REQUIRE_FALSE(dnd::matches_selection(CustomType::H, dnd::SelectionFilterType::IS_IN, custom_selection2));
    }

    SECTION("matching IS_NOT_IN") {
        REQUIRE_FALSE(dnd::matches_selection(4, dnd::SelectionFilterType::IS_NOT_IN, int_selection));
        REQUIRE(dnd::matches_selection(-7, dnd::SelectionFilterType::IS_NOT_IN, int_selection));
        REQUIRE_FALSE(dnd::matches_selection(-18.13f, dnd::SelectionFilterType::IS_NOT_IN, float_selection));
        REQUIRE(dnd::matches_selection(42.13f, dnd::SelectionFilterType::IS_NOT_IN, float_selection));
        REQUIRE_FALSE(
            dnd::matches_selection<std::string>("Hello", dnd::SelectionFilterType::IS_NOT_IN, string_selection1)
        );
        REQUIRE(dnd::matches_selection<std::string>("are", dnd::SelectionFilterType::IS_NOT_IN, string_selection1));
        REQUIRE_FALSE(dnd::matches_selection<std::string>("How", dnd::SelectionFilterType::IS_NOT_IN, string_selection2)
        );
        REQUIRE(dnd::matches_selection<std::string>("!", dnd::SelectionFilterType::IS_NOT_IN, string_selection2));
        REQUIRE_FALSE(dnd::matches_selection(CustomType::A, dnd::SelectionFilterType::IS_NOT_IN, custom_selection1));
        REQUIRE(dnd::matches_selection(CustomType::D, dnd::SelectionFilterType::IS_NOT_IN, custom_selection1));
        REQUIRE(dnd::matches_selection(CustomType::C, dnd::SelectionFilterType::IS_NOT_IN, custom_selection2));
        REQUIRE_FALSE(dnd::matches_selection(CustomType::F, dnd::SelectionFilterType::IS_NOT_IN, custom_selection2));
        REQUIRE(dnd::matches_selection(CustomType::H, dnd::SelectionFilterType::IS_NOT_IN, custom_selection1));
        REQUIRE(dnd::matches_selection(CustomType::H, dnd::SelectionFilterType::IS_NOT_IN, custom_selection2));
    }
}
