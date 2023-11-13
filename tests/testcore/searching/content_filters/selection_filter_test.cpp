#include <dnd_config.hpp>

#include <core/searching/content_filters/selection_filter.hpp>

#include <string>
#include <utility>
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
    dnd::SelectionFilter<int> int_filter;
    int_filter.set_values(std::move(int_selection));

    std::vector<float> float_selection = {-18.13f, 0.0f, 1.13f, 2008.71f};
    dnd::SelectionFilter<float> float_filter;
    float_filter.set_values(std::move(float_selection));

    std::vector<std::string> string_selection1 = {"Hello", "World", "!"};
    dnd::SelectionFilter<std::string> string_filter1;
    string_filter1.set_values(std::move(string_selection1));

    std::vector<std::string> string_selection2 = {"How", "are", "you", "?"};
    dnd::SelectionFilter<std::string> string_filter2;
    string_filter2.set_values(std::move(string_selection2));

    std::vector<CustomType> custom_selection1 = {CustomType::A, CustomType::B, CustomType::C};
    dnd::SelectionFilter<CustomType> custom_filter1;
    custom_filter1.set_values(std::move(custom_selection1));

    std::vector<CustomType> custom_selection2 = {CustomType::D, CustomType::E, CustomType::F, CustomType::G};
    dnd::SelectionFilter<CustomType> custom_filter2;
    custom_filter2.set_values(std::move(custom_selection2));

    SECTION("matching NONE always returns true") {
        int_filter.set_type(dnd::SelectionFilterType::NONE);
        REQUIRE(int_filter.matches(4));
        REQUIRE(int_filter.matches(-7));

        float_filter.set_type(dnd::SelectionFilterType::NONE);
        REQUIRE(float_filter.matches(-18.13f));
        REQUIRE(float_filter.matches(42.13f));

        string_filter1.set_type(dnd::SelectionFilterType::NONE);
        REQUIRE(string_filter1.matches("Hello"));
        REQUIRE(string_filter1.matches("are"));

        string_filter2.set_type(dnd::SelectionFilterType::NONE);
        REQUIRE(string_filter2.matches("How"));
        REQUIRE(string_filter2.matches("!"));

        custom_filter1.set_type(dnd::SelectionFilterType::NONE);
        REQUIRE(custom_filter1.matches(CustomType::A));
        REQUIRE(custom_filter1.matches(CustomType::D));
        REQUIRE(custom_filter1.matches(CustomType::H));

        custom_filter2.set_type(dnd::SelectionFilterType::NONE);
        REQUIRE(custom_filter2.matches(CustomType::C));
        REQUIRE(custom_filter2.matches(CustomType::F));
        REQUIRE(custom_filter2.matches(CustomType::H));
    }

    SECTION("matching IS_IN") {
        int_filter.set_type(dnd::SelectionFilterType::IS_IN);
        REQUIRE(int_filter.matches(4));
        REQUIRE_FALSE(int_filter.matches(-7));

        float_filter.set_type(dnd::SelectionFilterType::IS_IN);
        REQUIRE(float_filter.matches(-18.13f));
        REQUIRE_FALSE(float_filter.matches(42.13f));

        string_filter1.set_type(dnd::SelectionFilterType::IS_IN);
        REQUIRE(string_filter1.matches("Hello"));
        REQUIRE_FALSE(string_filter1.matches("are"));

        string_filter2.set_type(dnd::SelectionFilterType::IS_IN);
        REQUIRE(string_filter2.matches("How"));
        REQUIRE_FALSE(string_filter2.matches("!"));

        custom_filter1.set_type(dnd::SelectionFilterType::IS_IN);
        REQUIRE(custom_filter1.matches(CustomType::A));
        REQUIRE_FALSE(custom_filter1.matches(CustomType::D));
        REQUIRE_FALSE(custom_filter1.matches(CustomType::H));

        custom_filter2.set_type(dnd::SelectionFilterType::IS_IN);
        REQUIRE_FALSE(custom_filter2.matches(CustomType::C));
        REQUIRE(custom_filter2.matches(CustomType::F));
        REQUIRE_FALSE(custom_filter2.matches(CustomType::H));
    }

    SECTION("matching IS_NOT_IN") {
        int_filter.set_type(dnd::SelectionFilterType::IS_NOT_IN);
        REQUIRE_FALSE(int_filter.matches(4));
        REQUIRE(int_filter.matches(-7));

        float_filter.set_type(dnd::SelectionFilterType::IS_NOT_IN);
        REQUIRE_FALSE(float_filter.matches(-18.13f));
        REQUIRE(float_filter.matches(42.13f));

        string_filter1.set_type(dnd::SelectionFilterType::IS_NOT_IN);
        REQUIRE_FALSE(string_filter1.matches("Hello"));
        REQUIRE(string_filter1.matches("are"));

        string_filter2.set_type(dnd::SelectionFilterType::IS_NOT_IN);
        REQUIRE_FALSE(string_filter2.matches("How"));
        REQUIRE(string_filter2.matches("!"));

        custom_filter1.set_type(dnd::SelectionFilterType::IS_NOT_IN);
        REQUIRE_FALSE(custom_filter1.matches(CustomType::A));
        REQUIRE(custom_filter1.matches(CustomType::D));
        REQUIRE(custom_filter1.matches(CustomType::H));

        custom_filter2.set_type(dnd::SelectionFilterType::IS_NOT_IN);
        REQUIRE(custom_filter2.matches(CustomType::C));
        REQUIRE_FALSE(custom_filter2.matches(CustomType::F));
        REQUIRE(custom_filter2.matches(CustomType::H));
    }
}
