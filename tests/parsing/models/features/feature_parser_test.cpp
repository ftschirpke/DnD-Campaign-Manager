#include "parsing/models/features/feature_parser.hpp"

#include <unordered_map>

#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#include "models/creature_state.hpp"
#include "models/features/effect.hpp"
#include "models/features/feature.hpp"

TEST_CASE("dnd::FeatureParser::parseAndAddEffect: parse invalid effects") {
    dnd::Feature feature("test", "feature for testing of effect parsing");
    SECTION("wrong format or order") {
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("hello", feature));
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("", feature));
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("CON,normal,add,2", feature));
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("WIS.early.mult.-2", feature));
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("CHA|latest|div|4.5", feature));
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("earliest set STRMAX 22", feature));
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("addConst MAXHP LEVEL late", feature));
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("INT normal add other:PB", feature));
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("CON addOther INTMOD", feature));
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("early mult", feature));
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("CHAlatestdivLEVEL", feature));
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("INT", feature));
    }
    SECTION("numeric operations without number") {
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("CON normal add", feature));
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("WIS early mult ", feature));
    }
    SECTION("numeric operations with identifier") {
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("CON normal add INTMOD", feature));
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("WIS early mult MAXHP", feature));
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("CHA latest div LEVEL", feature));
    }
    SECTION("numeric operations: maximum of 2 decimal places") {
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("CON normal add -0.3333333333", feature));
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("WIS early mult 1.324150", feature));
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("INT late set -3.525", feature));
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("CHA latest div 1.000", feature));
    }
    SECTION("identifier operations without identifier") {
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("CON normal addOther", feature));
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("WIS early multConst ", feature));
    }
    SECTION("identifier operations with number") {
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("CON normal addOther 2", feature));
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("WIS early multConst -2", feature));
        REQUIRE_THROWS(dnd::FeatureParser::parseAndAddEffect("CHA latest divOther 4.5", feature));
    }
}

TEST_CASE("dnd::FeatureParser::parseAndAddEffect: parse valid numeric effects") {
    dnd::Feature feature("test", "feature for testing of effect parsing");
    const std::unordered_map<std::string, int> constants;
    std::unordered_map<std::string, int> attributes = {
        {"MAXHP", 1000},
        {"STR", 1000},
        {"CON", 1000},
        {"INT", 1000},
    };
    SECTION("add") {
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal add 2", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest add 1.25", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest add -0.7", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early add -3", feature));
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
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal mult 2", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest mult 1.25", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest mult -0.7", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early mult -3", feature));
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
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal div 2", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest div 1.25", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest div -0.7", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early div -3", feature));
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
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal set 2", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest set 1.25", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest set -0.7", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early set -3", feature));
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
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal max 20", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest max 1.25", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest max -0.7", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early max -3", feature));
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
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal min 20", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest min 1.25", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest min -0.7", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early min -3", feature));
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

TEST_CASE("dnd::FeatureParser::parseAndAddEffect: parse valid 'Other' identifier effects") {
    dnd::Feature feature("test", "feature for testing of effect parsing");
    const std::unordered_map<std::string, int> constants;
    std::unordered_map<std::string, int> attributes = {
        {"AC", 200},     {"DEX", 125},  {"WIS", -70},  {"CHA", -300},
        {"MAXHP", 1000}, {"STR", 1000}, {"CON", 1000}, {"INT", 1000},
    };
    SECTION("addOther") {
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal addOther AC", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest addOther DEX", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest addOther WIS", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early addOther CHA", feature));
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
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal multOther AC", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest multOther DEX", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest multOther WIS", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early multOther CHA", feature));
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
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal divOther AC", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest divOther DEX", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest divOther WIS", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early divOther CHA", feature));
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
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal setOther AC", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest setOther DEX", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest setOther WIS", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early setOther CHA", feature));
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
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal maxOther AC", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest maxOther DEX", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest maxOther WIS", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early maxOther CHA", feature));
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
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal minOther AC", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest minOther DEX", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest minOther WIS", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early minOther CHA", feature));
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

TEST_CASE("dnd::FeatureParser::parseAndAddEffect: parse valid 'Const' identifier effects") {
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
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal addConst LEVEL", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest addConst XP", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest addConst CONST1", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early addConst CONST2", feature));
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
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal multConst LEVEL", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest multConst XP", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest multConst CONST1", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early multConst CONST2", feature));
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
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal divConst LEVEL", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest divConst XP", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest divConst CONST1", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early divConst CONST2", feature));
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
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal setConst LEVEL", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest setConst XP", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest setConst CONST1", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early setConst CONST2", feature));
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
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal maxConst LEVEL", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest maxConst XP", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest maxConst CONST1", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early maxConst CONST2", feature));
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
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal minConst LEVEL", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest minConst XP", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest minConst CONST1", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early minConst CONST2", feature));
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

TEST_CASE("dnd::FeatureParser::parseAndAddEffect: parse effect combinations") {
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
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP earliest setConst CONST2", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP early div -4", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal add 2", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP late multOther STR", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP late mult -1", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP latest addConst LEVEL", feature));
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
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest setConst XP", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR early mult -2", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP early div 2", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal add 2", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal add 1", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP latest addOther STR", feature));
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

TEST_CASE("dnd::FeatureParser::addEffects: invalid JSON format") {
    dnd::Feature feature("test", "feature for testing of effect parsing");
    SECTION("JSON is object/map") {
        const nlohmann::json effects_json = {
            {"1st effect", "CON normal add 2"},
            {"2nd effect", "INT normal mult 3"},
        };
        REQUIRE_THROWS(dnd::FeatureParser::addEffects(effects_json, feature));
    }
    SECTION("JSON is literal") {
        REQUIRE_THROWS(dnd::FeatureParser::addEffects(true, feature));
        REQUIRE_THROWS(dnd::FeatureParser::addEffects(1, feature));
        REQUIRE_THROWS(dnd::FeatureParser::addEffects(-3.4, feature));
        REQUIRE_THROWS(dnd::FeatureParser::addEffects("string", feature));
    }
}

TEST_CASE("dnd::FeatureParser::addEffects: parse valid effect combinations") {
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
        const nlohmann::json effects_json = {
            "MAXHP earliest setConst CONST2", "MAXHP early div -4", "MAXHP normal add 2",
            "MAXHP late multOther STR",       "MAXHP late mult -1", "MAXHP latest addConst LEVEL",
        };
        REQUIRE_NOTHROW(dnd::FeatureParser::addEffects(effects_json, feature));
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
        const nlohmann::json effects_json = {
            "STR earliest setConst XP", "STR early mult -2",  "MAXHP early div 2",
            "MAXHP normal add 2",       "MAXHP normal add 1", "MAXHP latest addOther STR",
        };
        REQUIRE_NOTHROW(dnd::FeatureParser::addEffects(effects_json, feature));
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

TEST_CASE("dnd::FeatureParser::createFeature: invalid JSON format") {
    dnd::Feature feature("test", "feature for testing of effect parsing");
    SECTION("JSON is array") {
        const nlohmann::json feature_json = {
            "feature for testing of effect parsing",
            {"CON normal add 2", "INT normal mult 3"},
        };
        REQUIRE_THROWS(dnd::FeatureParser::createFeature("test", feature_json));
    }
    SECTION("JSON is literal") {
        REQUIRE_THROWS(dnd::FeatureParser::createFeature("test", true));
        REQUIRE_THROWS(dnd::FeatureParser::createFeature("test", 1));
        REQUIRE_THROWS(dnd::FeatureParser::createFeature("test", -3.4));
        REQUIRE_THROWS(dnd::FeatureParser::createFeature("test", "string"));
    }
}

TEST_CASE("dnd::FeatureParser::createFeature: parse valid features") {
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
        std::shared_ptr<dnd::Feature> feature;
        REQUIRE_NOTHROW(feature = std::move(dnd::FeatureParser::createFeature(name, feature_json)));
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
        std::shared_ptr<dnd::Feature> feature;
        REQUIRE_NOTHROW(feature = std::move(dnd::FeatureParser::createFeature(name, feature_json)));
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
        std::shared_ptr<dnd::Feature> feature;
        REQUIRE_NOTHROW(feature = std::move(dnd::FeatureParser::createFeature(name, feature_json)));
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
