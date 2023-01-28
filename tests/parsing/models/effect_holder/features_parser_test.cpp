#include "parsing/models/effect_holder/features_parser.hpp"

#include <string>
#include <unordered_map>

#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#include "controllers/groups.hpp"
#include "models/effect_holder/feature.hpp"

// class that allows us to test the abstract dnd::FeaturesParser class
class TestFeaturesParser : public dnd::FeaturesParser {
public:
    TestFeaturesParser() noexcept : dnd::FeaturesParser(dnd::Groups()) {}
    dnd::Feature createFeatureForTesting(const std::string& feature_name, const nlohmann::json& feature_json) const {
        return createFeature(feature_name, feature_json);
    }
    virtual void requiresConfiguration() const override {} // for testing, no configuration is required
};

TEST_CASE("dnd::FeaturesParser::createFeature: invalid JSON format") {
    TestFeaturesParser parser;
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
