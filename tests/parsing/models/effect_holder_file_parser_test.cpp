#include "parsing/models/effect_holder_file_parser.hpp"

#include <string>
#include <unordered_map>
#include <utility>

#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#include "controllers/groups.hpp"
#include "models/effect_holder/activation.hpp"
#include "models/effect_holder/effect.hpp"
#include "models/effect_holder/effect_holder.hpp"

// class that allows us to test the abstract dnd::EffectHolderFileParser class
class TestEffectHolderFileParser : public dnd::EffectHolderFileParser {
public:
    TestEffectHolderFileParser() noexcept : dnd::EffectHolderFileParser(dnd::Groups()) {}
    void parseAndAddEffectForTesting(const std::string& effect_str, dnd::EffectHolder& effect_holder) const {
        dnd::EffectHolderFileParser::parseAndAddEffect(effect_str, &effect_holder);
    }
    void parseAndAddActivationForTesting(const std::string& activation_str, dnd::EffectHolder& effect_holder) const {
        dnd::EffectHolderFileParser::parseAndAddActivation(activation_str, &effect_holder);
    }
    dnd::EffectHolder createEffectHolderForTesting(const nlohmann::json& effect_holder_json) const {
        return createEffectHolder(effect_holder_json);
    }
    void parse() override {}
    bool validate() const override { return true; }
    void saveResult() override {}
};

TEST_CASE("dnd::EffectHolderFileParser::parseAndAddEffect: parse invalid effects") {
    TestEffectHolderFileParser parser;
    dnd::EffectHolder eh;
    SECTION("wrong format or order") {
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("hello", eh));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("", eh));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CON,normal,add,2", eh));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("WIS.early.mult.-2", eh));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CHA|latest|div|4.5", eh));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("earliest set STRMAX 22", eh));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("addConst MAXHP LEVEL late", eh));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("INT normal add other:PB", eh));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CON addOther INTMOD", eh));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("early mult", eh));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CHAlatestdivLEVEL", eh));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("INT", eh));
    }
    SECTION("numeric operations without number") {
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CON normal add", eh));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("WIS early mult ", eh));
    }
    SECTION("numeric operations with identifier") {
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CON normal add INTMOD", eh));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("WIS early mult MAXHP", eh));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CHA latest div LEVEL", eh));
    }
    SECTION("numeric operations: maximum of 2 decimal places") {
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CON normal add -0.3333333333", eh));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("WIS early mult 1.324150", eh));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("INT late set -3.525", eh));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CHA latest div 1.000", eh));
    }
    SECTION("identifier operations without identifier") {
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CON normal addOther", eh));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("WIS early multConst ", eh));
    }
    SECTION("identifier operations with number") {
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CON normal addOther 2", eh));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("WIS early multConst -2", eh));
        REQUIRE_THROWS(parser.parseAndAddEffectForTesting("CHA latest divOther 4.5", eh));
    }
}

TEST_CASE("dnd::EffectHolderFileParser::parseAndAddEffect: parse valid numeric effects") {
    TestEffectHolderFileParser parser;
    dnd::EffectHolder eh;
    const std::unordered_map<std::string, int> constants;
    std::unordered_map<std::string, int> attributes = {
        {"MAXHP", 1000},
        {"STR", 1000},
        {"CON", 1000},
        {"INT", 1000},
    };
    SECTION("add") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal add 2", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest add 1.25", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest add -0.7", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early add -3", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal mult 2", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest mult 1.25", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest mult -0.7", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early mult -3", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal div 2", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest div 1.25", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest div -0.7", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early div -3", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal set 2", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest set 1.25", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest set -0.7", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early set -3", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal max 20", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest max 1.25", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest max -0.7", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early max -3", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal min 20", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest min 1.25", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest min -0.7", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early min -3", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
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

TEST_CASE("dnd::EffectHolderFileParser::parseAndAddEffect: parse valid 'Other' identifier effects") {
    TestEffectHolderFileParser parser;
    dnd::EffectHolder eh;
    const std::unordered_map<std::string, int> constants;
    std::unordered_map<std::string, int> attributes = {
        {"AC", 200},     {"DEX", 125},  {"WIS", -70},  {"CHA", -300},
        {"MAXHP", 1000}, {"STR", 1000}, {"CON", 1000}, {"INT", 1000},
    };
    SECTION("addOther") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal addOther AC", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest addOther DEX", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest addOther WIS", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early addOther CHA", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"AC", 200},     {"DEX", 125},  {"WIS", -70}, {"CHA", -300},
            {"MAXHP", 1200}, {"STR", 1125}, {"CON", 930}, {"INT", 700},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("multOther") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal multOther AC", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest multOther DEX", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest multOther WIS", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early multOther CHA", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"AC", 200},     {"DEX", 125},  {"WIS", -70},  {"CHA", -300},
            {"MAXHP", 2000}, {"STR", 1250}, {"CON", -700}, {"INT", -3000},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("divOther") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal divOther AC", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest divOther DEX", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest divOther WIS", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early divOther CHA", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"AC", 200},    {"DEX", 125}, {"WIS", -70},   {"CHA", -300},
            {"MAXHP", 500}, {"STR", 800}, {"CON", -1428}, {"INT", -333},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("setOther") {
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal setOther AC", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest setOther DEX", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest setOther WIS", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early setOther CHA", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal maxOther AC", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest maxOther DEX", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest maxOther WIS", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early maxOther CHA", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal minOther AC", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest minOther DEX", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest minOther WIS", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early minOther CHA", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"AC", 2000},    {"DEX", 125}, {"WIS", -70}, {"CHA", -300},
            {"MAXHP", 1000}, {"STR", 125}, {"CON", -70}, {"INT", -300},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
}

TEST_CASE("dnd::EffectHolderFileParser::parseAndAddEffect: parse valid 'Const' identifier effects") {
    TestEffectHolderFileParser parser;
    dnd::EffectHolder eh;
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
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal addConst LEVEL", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest addConst XP", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest addConst CONST1", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early addConst CONST2", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal multConst LEVEL", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest multConst XP", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest multConst CONST1", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early multConst CONST2", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal divConst LEVEL", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest divConst XP", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest divConst CONST1", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early divConst CONST2", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal setConst LEVEL", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest setConst XP", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest setConst CONST1", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early setConst CONST2", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal maxConst LEVEL", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest maxConst XP", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest maxConst CONST1", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early maxConst CONST2", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(
            eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, max_min_constants)
        );
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, max_min_constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, max_min_constants)
        );
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, max_min_constants));
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
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal minConst LEVEL", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest minConst XP", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("CON latest minConst CONST1", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("INT early minConst CONST2", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(
            eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, max_min_constants)
        );
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, max_min_constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, max_min_constants)
        );
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, max_min_constants));
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

TEST_CASE("dnd::EffectHolderFileParser::parseAndAddEffect: parse effect combinations") {
    TestEffectHolderFileParser parser;
    dnd::EffectHolder eh;
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
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP earliest setConst CONST2", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP early div -4", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal add 2", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP late multOther STR", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP late mult -1", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP latest addConst LEVEL", eh));
        REQUIRE(eh.ability_score_effects.empty());
        REQUIRE(eh.normal_effects.size() == 5);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::LATE).size() == 2);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::LATE)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::LATE)[1]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR earliest setConst XP", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("STR early mult -2", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP early div 2", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal add 2", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP normal add 1", eh));
        REQUIRE_NOTHROW(parser.parseAndAddEffectForTesting("MAXHP latest addOther STR", eh));
        REQUIRE(eh.ability_score_effects.size() == 2);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.normal_effects.size() == 3);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 2);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[1]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
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

TEST_CASE("dnd::EffectHolderFileParser::parseAndAddActivation: parse invalid activations") {
    TestEffectHolderFileParser parser;
    dnd::EffectHolder eh;
    SECTION("wrong format or order") {
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("hello", eh));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("", eh));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("CLASS_LEVEL>=2", eh));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("LEVEL== 1", eh));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("ARMOR_ON <0", eh));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("== CLASS_LEVEL LEVEL", eh));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("LEVEL 1 ==", eh));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("CLASS_LEVEL 5", eh));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("LEVEL is CLASS_LEVEL", eh));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("INT 12 greater", eh));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("ARMOR_ON 1 ==", eh));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("ARMOR_ON", eh));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("true == ARMOR_ON", eh));
        REQUIRE_THROWS(parser.parseAndAddActivationForTesting("13 <= INT", eh));
    }
}

TEST_CASE("dnd::EffectHolderFileParser::parseAndAddActivation: parse valid numeric activations") {
    TestEffectHolderFileParser parser;
    dnd::EffectHolder eh;
    REQUIRE(eh.activations.empty());
    std::unordered_map<std::string, int> attributes = {{"MAXHP", 4000}, {"STR", 1600}, {"CON", 1300}, {"INT", 800}};
    const std::unordered_map<std::string, int> constants = {{"LEVEL", 500}, {"CLASS_LEVEL", 500}, {"ARMOR_ON", 1}};
    size_t idx = 0;
    SECTION("==") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL == 5", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CLASS_LEVEL == 3", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("ARMOR_ON == true", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("ARMOR_ON == false", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CON == 13", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("STR == 18", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
    }
    SECTION("!=") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CLASS_LEVEL != 3", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL != 5", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("ARMOR_ON != false", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("ARMOR_ON != true", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("STR != 18", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CON != 13", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
    }
    SECTION(">=") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CLASS_LEVEL >= 3", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL >= 5", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CLASS_LEVEL >= 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("MAXHP >= 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("STR >= 16", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("INT >= 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
    }
    SECTION("<=") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CLASS_LEVEL <= 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL <= 5", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CLASS_LEVEL <= 3", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("STR <= 16", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("INT <= 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("MAXHP <= 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
    }
    SECTION(">") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CLASS_LEVEL > 3", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL > 5", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CLASS_LEVEL > 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("MAXHP > 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("STR > 16", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("INT > 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
    }
    SECTION("<") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CLASS_LEVEL < 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL < 5", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CLASS_LEVEL < 3", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("INT < 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("STR < 16", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("MAXHP < 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
    }
}

TEST_CASE("dnd::EffectHolderFileParser::createActivation: parse valid identifier activations") {
    TestEffectHolderFileParser parser;
    dnd::EffectHolder eh;
    REQUIRE(eh.activations.empty());
    std::unordered_map<std::string, int> attributes1 = {{"MAXHP", 4000}, {"STR", 1600}, {"CON", 1300}, {"INT", 800}};
    const std::unordered_map<std::string, int> constants1 = {{"LEVEL", 500}, {"CLASS_LEVEL", 500}, {"ARMOR_ON", 1}};
    std::unordered_map<std::string, int> attributes2 = {{"MAXHP", 2300}, {"STR", 800}, {"CON", 1000}, {"INT", 1800}};
    const std::unordered_map<std::string, int> constants2 = {{"LEVEL", 1300}, {"CLASS_LEVEL", 1300}, {"ARMOR_ON", 0}};
    size_t idx = 0;
    SECTION("==") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL == CLASS_LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("INT == LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CON == STR", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
    }
    SECTION("!=") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL != CLASS_LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("INT != LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("CON != STR", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
    }
    SECTION(">=") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL >= CLASS_LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("MAXHP >= INT", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL >= CON", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("ARMOR_ON >= LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
    }
    SECTION("<=") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL <= CLASS_LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("MAXHP <= INT", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL <= CON", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("ARMOR_ON <= LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
    }
    SECTION(">") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL > CLASS_LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("MAXHP > INT", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL > CON", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("ARMOR_ON > LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
    }
    SECTION("<") {
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL < CLASS_LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("MAXHP < INT", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("LEVEL < CON", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parseAndAddActivationForTesting("ARMOR_ON < LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
    }
}

TEST_CASE("dnd::EffectHolderFileParser::createFeature: parse valid ehs") {
    TestEffectHolderFileParser parser;
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
    const std::string description = "eh for testing of effect parsing";
    SECTION("eh without effects") {
        // TODO: add more testcases when activation etc. is implemented
        const nlohmann::json eh_json = {
            {"description", description},
        };
        dnd::EffectHolder eh;
        REQUIRE_NOTHROW(eh = std::move(parser.createEffectHolderForTesting(eh_json)));
        REQUIRE(eh.ability_score_effects.empty());
        REQUIRE(eh.normal_effects.empty());
    }
    SECTION("effect combination 1 (order of calculation)") {
        const nlohmann::json effects_json = {
            "MAXHP earliest setConst CONST2", "MAXHP early div -4", "MAXHP normal add 2",
            "MAXHP late multOther STR",       "MAXHP late mult -1", "MAXHP latest addConst LEVEL",
        };
        // TODO: add more testcases when activation etc. is implemented
        const nlohmann::json eh_json = {
            {"description", description},
            {"effects", effects_json},
        };
        dnd::EffectHolder eh;
        REQUIRE_NOTHROW(eh = std::move(parser.createEffectHolderForTesting(eh_json)));
        REQUIRE(eh.ability_score_effects.empty());
        REQUIRE(eh.normal_effects.size() == 5);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::LATE).size() == 2);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::LATE)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::LATE)[1]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
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
        const nlohmann::json eh_json = {
            {"description", description},
            {"effects", effects_json},
        };
        dnd::EffectHolder eh;
        REQUIRE_NOTHROW(eh = std::move(parser.createEffectHolderForTesting(eh_json)));
        REQUIRE(eh.ability_score_effects.size() == 2);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.normal_effects.size() == 3);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 2);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::EARLY)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[1]->applyTo(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::LATEST)[0]->applyTo(attributes, constants));
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
