#include "parsing/models/feature_holder_file_parser.hpp"

#include <unordered_map>

#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#include "models/creature_state.hpp"
#include "models/effects_holder/activation.hpp"
#include "models/effects_holder/effect.hpp"
#include "models/effects_holder/feature.hpp"

// class that allows us to test the abstract dnd::FeatureHolderFileParser class
class TestFeatureHolderFileParser : public dnd::FeatureHolderFileParser {
public:
    void parseAndAddEffectForTesting(const std::string& effect_str, dnd::Feature& feature) const {
        dnd::FeatureHolderFileParser::parseAndAddEffect(effect_str, feature);
    }
    void parseAndAddActivationForTesting(const std::string& activation_str, dnd::Feature& feature) const {
        dnd::FeatureHolderFileParser::parseAndAddActivation(activation_str, feature);
    }
    std::unique_ptr<dnd::Feature> createFeatureForTesting(
        const std::string& feature_name, const nlohmann::json& feature_json
    ) const {
        return std::make_unique<dnd::Feature>(dnd::FeatureHolderFileParser::createFeature(feature_name, feature_json));
    }
    void parse() override {}
    bool validate() const override { return true; }
    void saveResult() override {}
};

TEST_CASE("dnd::FeatureHolderFileParser::parseAndAddEffect: parse invalid effects") {
    TestFeatureHolderFileParser parser;
    dnd::Feature feature("test", "feature for testing of effect parsing");
    SECTION("wrong format or order") {
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("hello", feature));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("", feature));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CON,normal,add,2", feature));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("WIS.early.mult.-2", feature));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CHA|latest|div|4.5", feature));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("earliest set STRMAX 22", feature));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("addConst MAXHP LEVEL late", feature));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("INT normal add other:PB", feature));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CON addOther INTMOD", feature));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("early mult", feature));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CHAlatestdivLEVEL", feature));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("INT", feature));
    }
    SECTION("numeric operations without number") {
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CON normal add", feature));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("WIS early mult ", feature));
    }
    SECTION("numeric operations with identifier") {
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CON normal add INTMOD", feature));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("WIS early mult MAXHP", feature));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CHA latest div LEVEL", feature));
    }
    SECTION("numeric operations: maximum of 2 decimal places") {
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CON normal add -0.3333333333", feature));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("WIS early mult 1.324150", feature));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("INT late set -3.525", feature));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CHA latest div 1.000", feature));
    }
    SECTION("identifier operations without identifier") {
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CON normal addOther", feature));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("WIS early multConst ", feature));
    }
    SECTION("identifier operations with number") {
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CON normal addOther 2", feature));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("WIS early multConst -2", feature));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CHA latest divOther 4.5", feature));
    }
}

TEST_CASE("dnd::FeatureHolderFileParser::parseAndAddEffect: parse valid numeric effects") {
    TestFeatureHolderFileParser parser;
    dnd::Feature feature("test", "feature for testing of effect parsing");
    const std::unordered_map<std::string, int> constants;
    std::unordered_map<std::string, int> attributes = {
        {"MAXHP", 1000},
        {"STR", 1000},
        {"CON", 1000},
        {"INT", 1000},
    };
    SECTION("add") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal add 2", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest add 1.25", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest add -0.7", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early add -3", feature));
        REQUIRE(feature.ability_score_effects.size() == 3);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(feature.normal_effects.size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 1200},
            {"STR", 1125},
            {"CON", 930},
            {"INT", 700},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("mult") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal mult 2", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest mult 1.25", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest mult -0.7", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early mult -3", feature));
        REQUIRE(feature.ability_score_effects.size() == 3);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(feature.normal_effects.size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 2000},
            {"STR", 1250},
            {"CON", -700},
            {"INT", -3000},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("div") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal div 2", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest div 1.25", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest div -0.7", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early div -3", feature));
        REQUIRE(feature.ability_score_effects.size() == 3);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(feature.normal_effects.size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 500},
            {"STR", 800},
            {"CON", -1428},
            {"INT", -333},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("set") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal set 2", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest set 1.25", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest set -0.7", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early set -3", feature));
        REQUIRE(feature.ability_score_effects.size() == 3);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(feature.normal_effects.size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 200},
            {"STR", 125},
            {"CON", -70},
            {"INT", -300},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("max") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal max 20", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest max 1.25", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest max -0.7", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early max -3", feature));
        REQUIRE(feature.ability_score_effects.size() == 3);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(feature.normal_effects.size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 2000},
            {"STR", 1000},
            {"CON", 1000},
            {"INT", 1000},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("min") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal min 20", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest min 1.25", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest min -0.7", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early min -3", feature));
        REQUIRE(feature.ability_score_effects.size() == 3);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(feature.normal_effects.size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 1000},
            {"STR", 125},
            {"CON", -70},
            {"INT", -300},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
}

TEST_CASE("dnd::FeatureHolderFileParser::parseAndAddEffect: parse valid 'Other' identifier effects") {
    TestFeatureHolderFileParser parser;
    dnd::Feature feature("test", "feature for testing of effect parsing");
    const std::unordered_map<std::string, int> constants;
    std::unordered_map<std::string, int> attributes = {
        {"AC", 200},     {"DEX", 125},  {"WIS", -70},  {"CHA", -300},
        {"MAXHP", 1000}, {"STR", 1000}, {"CON", 1000}, {"INT", 1000},
    };
    SECTION("addOther") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal addOther AC", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest addOther DEX", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest addOther WIS", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early addOther CHA", feature));
        REQUIRE(feature.ability_score_effects.size() == 3);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(feature.normal_effects.size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"AC", 200},     {"DEX", 125},  {"WIS", -70}, {"CHA", -300},
            {"MAXHP", 1200}, {"STR", 1125}, {"CON", 930}, {"INT", 700},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("multOther") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal multOther AC", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest multOther DEX", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest multOther WIS", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early multOther CHA", feature));
        REQUIRE(feature.ability_score_effects.size() == 3);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(feature.normal_effects.size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"AC", 200},     {"DEX", 125},  {"WIS", -70},  {"CHA", -300},
            {"MAXHP", 2000}, {"STR", 1250}, {"CON", -700}, {"INT", -3000},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("divOther") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal divOther AC", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest divOther DEX", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest divOther WIS", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early divOther CHA", feature));
        REQUIRE(feature.ability_score_effects.size() == 3);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(feature.normal_effects.size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"AC", 200},    {"DEX", 125}, {"WIS", -70},   {"CHA", -300},
            {"MAXHP", 500}, {"STR", 800}, {"CON", -1428}, {"INT", -333},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("setOther") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal setOther AC", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest setOther DEX", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest setOther WIS", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early setOther CHA", feature));
        REQUIRE(feature.ability_score_effects.size() == 3);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(feature.normal_effects.size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"AC", 200},    {"DEX", 125}, {"WIS", -70}, {"CHA", -300},
            {"MAXHP", 200}, {"STR", 125}, {"CON", -70}, {"INT", -300},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("maxOther") {
        attributes["AC"] = 2000;
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal maxOther AC", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest maxOther DEX", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest maxOther WIS", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early maxOther CHA", feature));
        REQUIRE(feature.ability_score_effects.size() == 3);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(feature.normal_effects.size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"AC", 2000},    {"DEX", 125},  {"WIS", -70},  {"CHA", -300},
            {"MAXHP", 2000}, {"STR", 1000}, {"CON", 1000}, {"INT", 1000},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("minOther") {
        attributes["AC"] = 2000;
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal minOther AC", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest minOther DEX", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest minOther WIS", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early minOther CHA", feature));
        REQUIRE(feature.ability_score_effects.size() == 3);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(feature.normal_effects.size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"AC", 2000},    {"DEX", 125}, {"WIS", -70}, {"CHA", -300},
            {"MAXHP", 1000}, {"STR", 125}, {"CON", -70}, {"INT", -300},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
}

TEST_CASE("dnd::FeatureHolderFileParser::parseAndAddEffect: parse valid 'Const' identifier effects") {
    TestFeatureHolderFileParser parser;
    dnd::Feature feature("test", "feature for testing of effect parsing");
    const std::unordered_map<std::string, int> constants = {
        {"LEVEL", 200},
        {"XP", 125},
        {"CONST1", -70},
        {"CONST2", -300},
    };
    std::unordered_map<std::string, int> attributes = {
        {"MAXHP", 1000},
        {"STR", 1000},
        {"CON", 1000},
        {"INT", 1000},
    };
    SECTION("addConst") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal addConst LEVEL", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest addConst XP", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest addConst CONST1", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early addConst CONST2", feature));
        REQUIRE(feature.ability_score_effects.size() == 3);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(feature.normal_effects.size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 1200},
            {"STR", 1125},
            {"CON", 930},
            {"INT", 700},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("multConst") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal multConst LEVEL", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest multConst XP", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest multConst CONST1", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early multConst CONST2", feature));
        REQUIRE(feature.ability_score_effects.size() == 3);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(feature.normal_effects.size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 2000},
            {"STR", 1250},
            {"CON", -700},
            {"INT", -3000},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("divConst") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal divConst LEVEL", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest divConst XP", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest divConst CONST1", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early divConst CONST2", feature));
        REQUIRE(feature.ability_score_effects.size() == 3);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(feature.normal_effects.size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 500},
            {"STR", 800},
            {"CON", -1428},
            {"INT", -333},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("setConst") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal setConst LEVEL", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest setConst XP", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest setConst CONST1", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early setConst CONST2", feature));
        REQUIRE(feature.ability_score_effects.size() == 3);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(feature.normal_effects.size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 200},
            {"STR", 125},
            {"CON", -70},
            {"INT", -300},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }

    const std::unordered_map<std::string, int> max_min_constants = {
        {"LEVEL", 2000},
        {"XP", 125},
        {"CONST1", -70},
        {"CONST2", -300},
    };

    SECTION("maxConst") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal maxConst LEVEL", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest maxConst XP", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest maxConst CONST1", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early maxConst CONST2", feature));
        REQUIRE(feature.ability_score_effects.size() == 3);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(feature.normal_effects.size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(
            feature.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, max_min_constants)
        );
        REQUIRE_NOTHROW(
            feature.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, max_min_constants)
        );
        REQUIRE_NOTHROW(
            feature.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, max_min_constants)
        );
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, max_min_constants));
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 2000},
            {"STR", 1000},
            {"CON", 1000},
            {"INT", 1000},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("minConst") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal minConst LEVEL", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest minConst XP", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest minConst CONST1", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early minConst CONST2", feature));
        REQUIRE(feature.ability_score_effects.size() == 3);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(feature.normal_effects.size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(
            feature.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, max_min_constants)
        );
        REQUIRE_NOTHROW(
            feature.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, max_min_constants)
        );
        REQUIRE_NOTHROW(
            feature.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, max_min_constants)
        );
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, max_min_constants));
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 1000},
            {"STR", 125},
            {"CON", -70},
            {"INT", -300},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
}

TEST_CASE("dnd::FeatureHolderFileParser::parseAndAddEffect: parse effect combinations") {
    TestFeatureHolderFileParser parser;
    dnd::Feature feature("test", "feature for testing of effect parsing");
    const std::unordered_map<std::string, int> constants = {
        {"LEVEL", 200},
        {"XP", 125},
        {"CONST1", -70},
        {"CONST2", -300},
    };
    std::unordered_map<std::string, int> attributes = {
        {"MAXHP", 1000},
        {"STR", 1000},
        {"CON", 1000},
        {"INT", 1000},
    };
    SECTION("combination 1 (order of calculation)") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP earliest setConst CONST2", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP early div -4", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal add 2", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP late multOther STR", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP late mult -1", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP latest addConst LEVEL", feature));
        REQUIRE(feature.ability_score_effects.size() == 0);
        REQUIRE(feature.normal_effects.size() == 5);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::LATE).size() == 2);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::LATE)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::LATE)[1]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", -2550},
            {"STR", 1000},
            {"CON", 1000},
            {"INT", 1000},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("combination 2") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest setConst XP", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR early mult -2", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP early div 2", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal add 2", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal add 1", feature));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP latest addOther STR", feature));
        REQUIRE(feature.ability_score_effects.size() == 2);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature.normal_effects.size() == 3);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::NORMAL).size() == 2);
        REQUIRE(feature.normal_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::NORMAL)[1]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature.normal_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 550},
            {"STR", -250},
            {"CON", 1000},
            {"INT", 1000},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
}

TEST_CASE("dnd::FeatureHolderFileParser::parseAndAddActivation: parse invalid activations") {
    TestFeatureHolderFileParser parser;
    dnd::Feature feature("test", "feature for testing of effect parsing");
    SECTION("wrong format or order") {
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("hello", feature));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("", feature));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("CLASS_LEVEL>=2", feature));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("LEVEL== 1", feature));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("ARMOR_ON <0", feature));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("== CLASS_LEVEL LEVEL", feature));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("LEVEL 1 ==", feature));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("CLASS_LEVEL 5", feature));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("LEVEL is CLASS_LEVEL", feature));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("INT 12 greater", feature));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("ARMOR_ON 1 ==", feature));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("ARMOR_ON", feature));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("true == ARMOR_ON", feature));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("13 <= INT", feature));
    }
}

TEST_CASE("dnd::FeatureHolderFileParser::parseAndAddActivation: parse valid numeric activations") {
    TestFeatureHolderFileParser parser;
    dnd::Feature feature("test", "feature for testing of effect parsing");
    REQUIRE(feature.activations.size() == 0);
    std::unordered_map<std::string, int> attributes = {{"MAXHP", 4000}, {"STR", 1600}, {"CON", 1300}, {"INT", 800}};
    const std::unordered_map<std::string, int> constants = {{"LEVEL", 500}, {"CLASS_LEVEL", 500}, {"ARMOR_ON", 1}};
    int idx = 0;
    SECTION("==") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL == 5", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CLASS_LEVEL == 3", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("ARMOR_ON == true", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("ARMOR_ON == false", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CON == 13", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("STR == 18", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx++)->check(attributes, constants));
    }
    SECTION("!=") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CLASS_LEVEL != 3", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL != 5", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("ARMOR_ON != false", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("ARMOR_ON != true", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("STR != 18", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CON != 13", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx++)->check(attributes, constants));
    }
    SECTION(">=") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CLASS_LEVEL >= 3", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL >= 5", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CLASS_LEVEL >= 10", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("MAXHP >= 10", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("STR >= 16", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("INT >= 10", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx++)->check(attributes, constants));
    }
    SECTION("<=") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CLASS_LEVEL <= 10", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL <= 5", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CLASS_LEVEL <= 3", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("STR <= 16", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("INT <= 10", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("MAXHP <= 10", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx++)->check(attributes, constants));
    }
    SECTION(">") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CLASS_LEVEL > 3", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL > 5", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CLASS_LEVEL > 10", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("MAXHP > 10", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("STR > 16", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("INT > 10", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx++)->check(attributes, constants));
    }
    SECTION("<") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CLASS_LEVEL < 10", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL < 5", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CLASS_LEVEL < 3", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("INT < 10", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("STR < 16", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("MAXHP < 10", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx++)->check(attributes, constants));
    }
}

TEST_CASE("dnd::FeatureHolderFileParser::createActivation: parse valid identifier activations") {
    TestFeatureHolderFileParser parser;
    dnd::Feature feature("test", "feature for testing of effect parsing");
    REQUIRE(feature.activations.size() == 0);
    std::unordered_map<std::string, int> attributes1 = {{"MAXHP", 4000}, {"STR", 1600}, {"CON", 1300}, {"INT", 800}};
    const std::unordered_map<std::string, int> constants1 = {{"LEVEL", 500}, {"CLASS_LEVEL", 500}, {"ARMOR_ON", 1}};
    std::unordered_map<std::string, int> attributes2 = {{"MAXHP", 2300}, {"STR", 800}, {"CON", 1000}, {"INT", 1800}};
    const std::unordered_map<std::string, int> constants2 = {{"LEVEL", 1300}, {"CLASS_LEVEL", 1300}, {"ARMOR_ON", 0}};
    int idx = 0;
    SECTION("==") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL == CLASS_LEVEL", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(feature.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("INT == LEVEL", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!feature.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CON == STR", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!feature.activations.at(idx++)->check(attributes2, constants2));
    }
    SECTION("!=") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL != CLASS_LEVEL", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!feature.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("INT != LEVEL", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(feature.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CON != STR", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(feature.activations.at(idx++)->check(attributes2, constants2));
    }
    SECTION(">=") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL >= CLASS_LEVEL", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(feature.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("MAXHP >= INT", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(feature.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL >= CON", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(feature.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("ARMOR_ON >= LEVEL", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!feature.activations.at(idx++)->check(attributes2, constants2));
    }
    SECTION("<=") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL <= CLASS_LEVEL", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(feature.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("MAXHP <= INT", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!feature.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL <= CON", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!feature.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("ARMOR_ON <= LEVEL", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(feature.activations.at(idx++)->check(attributes2, constants2));
    }
    SECTION(">") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL > CLASS_LEVEL", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!feature.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("MAXHP > INT", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(feature.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL > CON", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(feature.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("ARMOR_ON > LEVEL", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!feature.activations.at(idx++)->check(attributes2, constants2));
    }
    SECTION("<") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL < CLASS_LEVEL", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!feature.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("MAXHP < INT", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!feature.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL < CON", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!feature.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("ARMOR_ON < LEVEL", feature));
        REQUIRE(feature.activations.size() == idx + 1);
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(feature.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(feature.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(feature.activations.at(idx++)->check(attributes2, constants2));
    }
}

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

TEST_CASE("dnd::FeatureHolderFileParser::createFeature: parse valid features") {
    TestFeatureHolderFileParser parser;
    const std::unordered_map<std::string, int> constants = {
        {"LEVEL", 200},
        {"XP", 125},
        {"CONST1", -70},
        {"CONST2", -300},
    };
    std::unordered_map<std::string, int> attributes = {
        {"MAXHP", 1000},
        {"STR", 1000},
        {"CON", 1000},
        {"INT", 1000},
    };
    const std::string name = "test";
    const std::string description = "feature for testing of effect parsing";
    SECTION("feature without effects") {
        // TODO: add more testcases when activation etc. is implemented
        const nlohmann::json feature_json = {
            {"description", description},
        };
        std::unique_ptr<dnd::Feature> feature;
        REQUIRE_NOTHROW(feature = std::move(parser.createFeatureForTesting(name, feature_json)));
        REQUIRE(feature->name == name);
        REQUIRE(feature->description == description);
        REQUIRE(feature->ability_score_effects.size() == 0);
        REQUIRE(feature->normal_effects.size() == 0);
    }
    SECTION("effect combination 1 (order of calculation)") {
        const nlohmann::json effects_json = {
            "MAXHP earliest setConst CONST2", "MAXHP early div -4", "MAXHP normal add 2",
            "MAXHP late multOther STR",       "MAXHP late mult -1", "MAXHP latest addConst LEVEL",
        };
        // TODO: add more testcases when activation etc. is implemented
        const nlohmann::json feature_json = {
            {"description", description},
            {"effects", effects_json},
        };
        std::unique_ptr<dnd::Feature> feature;
        REQUIRE_NOTHROW(feature = std::move(parser.createFeatureForTesting(name, feature_json)));
        REQUIRE(feature->name == name);
        REQUIRE(feature->description == description);
        REQUIRE(feature->ability_score_effects.size() == 0);
        REQUIRE(feature->normal_effects.size() == 5);
        REQUIRE(feature->normal_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature->normal_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature->normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE(feature->normal_effects.at(dnd::EffectTime::LATE).size() == 2);
        REQUIRE(feature->normal_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE_NOTHROW(feature->normal_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature->normal_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature->normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature->normal_effects.at(dnd::EffectTime::LATE)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature->normal_effects.at(dnd::EffectTime::LATE)[1]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature->normal_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", -2550},
            {"STR", 1000},
            {"CON", 1000},
            {"INT", 1000},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("effect combination 2") {
        const nlohmann::json effects_json = {
            "STR earliest setConst XP", "STR early mult -2",  "MAXHP early div 2",
            "MAXHP normal add 2",       "MAXHP normal add 1", "MAXHP latest addOther STR",
        };
        // TODO: add more testcases when activation etc. is implemented
        const nlohmann::json feature_json = {
            {"description", description},
            {"effects", effects_json},
        };
        std::unique_ptr<dnd::Feature> feature;
        REQUIRE_NOTHROW(feature = std::move(parser.createFeatureForTesting(name, feature_json)));
        REQUIRE(feature->name == name);
        REQUIRE(feature->description == description);
        REQUIRE(feature->ability_score_effects.size() == 2);
        REQUIRE(feature->ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(feature->ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature->normal_effects.size() == 3);
        REQUIRE(feature->normal_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(feature->normal_effects.at(dnd::EffectTime::NORMAL).size() == 2);
        REQUIRE(feature->normal_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE_NOTHROW(feature->ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants)
        );
        REQUIRE_NOTHROW(feature->ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature->normal_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature->normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature->normal_effects.at(dnd::EffectTime::NORMAL)[1]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(feature->normal_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 550},
            {"STR", -250},
            {"CON", 1000},
            {"INT", 1000},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
}
