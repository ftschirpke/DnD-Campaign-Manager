#include <core/parsing/models/effect_holder/features_parser.hpp>

#include <string>
#include <unordered_map>

#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#include <core/controllers/groups.hpp>
#include <core/models/effect_holder/feature.hpp>
#include <core/parsing/parsing_types.hpp>

/**
 * @brief A class that allows us to test the dnd::FeaturesParser class
 */
class TestFeaturesParser : public dnd::FeaturesParser {
public:
    TestFeaturesParser() noexcept : dnd::FeaturesParser(dnd::ParsingType::CHARACTER, "testing", dnd::Groups()) {}
    dnd::Feature create_feature_for_testing(const std::string& feature_name, const nlohmann::json& feature_json) const {
        return create_feature(feature_name, feature_json);
    }
};

TEST_CASE("dnd::FeaturesParser::create_feature: invalid JSON format") {
    TestFeaturesParser parser;
    SECTION("JSON is array") {
        const nlohmann::json feature_json = {
            "feature for testing of effect parsing",
            {"CON normal add 2", "INT normal mult 3"},
        };
        REQUIRE_THROWS(parser.create_feature_for_testing("test", feature_json));
    }
    SECTION("JSON is literal") {
        REQUIRE_THROWS(parser.create_feature_for_testing("test", true));
        REQUIRE_THROWS(parser.create_feature_for_testing("test", 1));
        REQUIRE_THROWS(parser.create_feature_for_testing("test", -3.4));
        REQUIRE_THROWS(parser.create_feature_for_testing("test", "string"));
    }
}
