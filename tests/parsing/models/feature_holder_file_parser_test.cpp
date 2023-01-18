#include "parsing/models/feature_holder_file_parser.hpp"

#include <unordered_map>

#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#include "models/creature_state.hpp"
#include "models/effect_holder/activation.hpp"
#include "models/effect_holder/effect.hpp"
#include "models/effect_holder/feature.hpp"

// class that allows us to test the abstract dnd::FeatureHolderFileParser class
class TestFeatureHolderFileParser : public dnd::FeatureHolderFileParser {
public:
    dnd::Feature createFeatureForTesting(const std::string& feature_name, const nlohmann::json& feature_json) const {
        return createFeature(feature_name, feature_json);
    }
    void parse() override {}
    bool validate() const override { return true; }
    void saveResult() override {}
};

TEST_CASE("dnd::FeatureHolderFileParser::createFeature: invalid JSON format") {
    TestFeatureHolderFileParser parser;
    SECTION("JSON is array") {
        const nlohmann::json feature_json = {
            "feature for testing of effect parsing",
            {"CON normal add 2", "INT normal mult 3"},
        };
        REQUIRE_THROWS(parser.createFeatureForTesting("test", feature_json));
    }
    SECTION("JSON is literal") {
        REQUIRE_THROWS(parser.createFeatureForTesting("test", true));
        REQUIRE_THROWS(parser.createFeatureForTesting("test", 1));
        REQUIRE_THROWS(parser.createFeatureForTesting("test", -3.4));
        REQUIRE_THROWS(parser.createFeatureForTesting("test", "string"));
    }
}
