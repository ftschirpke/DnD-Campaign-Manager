#include <core/parsing/models/effect_holder/effect_holder_parser.hpp>

#include <string>
#include <unordered_map>
#include <utility>

#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#include <core/controllers/groups.hpp>
#include <core/models/effect_holder/activation.hpp>
#include <core/models/effect_holder/effect.hpp>
#include <core/models/effect_holder/effect_holder.hpp>
#include <core/parsing/parsing_types.hpp>

/**
 * @brief A class that allows us to test the dnd::EffectHolderParser class
 */
class TestEffectHolderParser : public dnd::EffectHolderParser {
public:
    TestEffectHolderParser() noexcept
        : dnd::EffectHolderParser(dnd::ParsingType::CHARACTER, "testing", dnd::Groups()) {}
    void parse_and_add_effect_for_testing(const std::string& effect_str, dnd::EffectHolder& effect_holder) const {
        dnd::EffectHolderParser::parse_and_add_effect(effect_str, &effect_holder);
    }
    void parse_and_add_activation_for_testing(const std::string& activation_str, dnd::EffectHolder& effect_holder)
        const {
        dnd::EffectHolderParser::parse_and_add_activation(activation_str, &effect_holder);
    }
    dnd::EffectHolder create_effect_holder_for_testing(const nlohmann::json& effect_holder_json) const {
        return create_effect_holder(effect_holder_json);
    }
};

TEST_CASE("dnd::EffectHolderParser::parseAndAddEffect: parse invalid effects") {
    TestEffectHolderParser parser;
    dnd::EffectHolder eh;
    SECTION("wrong format or order") {
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("hello", eh));
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("", eh));
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("CON,normal,add,2", eh));
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("WIS.early.mult.-2", eh));
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("CHA|latest|div|4.5", eh));
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("earliest set STRMAX 22", eh));
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("addConst MAXHP LEVEL late", eh));
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("INT normal add other:PB", eh));
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("CON addOther INTMOD", eh));
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("early mult", eh));
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("CHAlatestdivLEVEL", eh));
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("INT", eh));
    }
    SECTION("numeric operations without number") {
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("CON normal add", eh));
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("WIS early mult ", eh));
    }
    SECTION("numeric operations with identifier") {
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("CON normal add INTMOD", eh));
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("WIS early mult MAXHP", eh));
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("CHA latest div LEVEL", eh));
    }
    SECTION("numeric operations: maximum of 2 decimal places") {
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("CON normal add -0.3333333333", eh));
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("WIS early mult 1.324150", eh));
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("INT late set -3.525", eh));
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("CHA latest div 1.000", eh));
    }
    SECTION("identifier operations without identifier") {
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("CON normal addOther", eh));
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("WIS early multConst ", eh));
    }
    SECTION("identifier operations with number") {
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("CON normal addOther 2", eh));
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("WIS early multConst -2", eh));
        REQUIRE_THROWS(parser.parse_and_add_effect_for_testing("CHA latest divOther 4.5", eh));
    }
}

TEST_CASE("dnd::EffectHolderParser::parseAndAddEffect: parse valid numeric effects") {
    TestEffectHolderParser parser;
    dnd::EffectHolder eh;
    const std::unordered_map<std::string, int> constants;
    std::unordered_map<std::string, int> attributes = {
        {"MAXHP", 1000},
        {"STR", 1000},
        {"CON", 1000},
        {"INT", 1000},
    };
    SECTION("add") {
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP normal add 2", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("STR earliest add 1.25", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("CON latest add -0.7", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("INT early add -3", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP normal mult 2", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("STR earliest mult 1.25", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("CON latest mult -0.7", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("INT early mult -3", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP normal div 2", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("STR earliest div 1.25", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("CON latest div -0.7", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("INT early div -3", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP normal set 2", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("STR earliest set 1.25", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("CON latest set -0.7", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("INT early set -3", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP normal max 20", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("STR earliest max 1.25", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("CON latest max -0.7", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("INT early max -3", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP normal min 20", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("STR earliest min 1.25", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("CON latest min -0.7", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("INT early min -3", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, constants));
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

TEST_CASE("dnd::EffectHolderParser::parseAndAddEffect: parse valid 'Other' identifier effects") {
    TestEffectHolderParser parser;
    dnd::EffectHolder eh;
    const std::unordered_map<std::string, int> constants;
    std::unordered_map<std::string, int> attributes = {
        {"AC", 200},     {"DEX", 125},  {"WIS", -70},  {"CHA", -300},
        {"MAXHP", 1000}, {"STR", 1000}, {"CON", 1000}, {"INT", 1000},
    };
    SECTION("addOther") {
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP normal addOther AC", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("STR earliest addOther DEX", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("CON latest addOther WIS", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("INT early addOther CHA", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"AC", 200},     {"DEX", 125},  {"WIS", -70}, {"CHA", -300},
            {"MAXHP", 1200}, {"STR", 1125}, {"CON", 930}, {"INT", 700},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("multOther") {
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP normal multOther AC", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("STR earliest multOther DEX", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("CON latest multOther WIS", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("INT early multOther CHA", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"AC", 200},     {"DEX", 125},  {"WIS", -70},  {"CHA", -300},
            {"MAXHP", 2000}, {"STR", 1250}, {"CON", -700}, {"INT", -3000},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("divOther") {
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP normal divOther AC", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("STR earliest divOther DEX", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("CON latest divOther WIS", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("INT early divOther CHA", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"AC", 200},    {"DEX", 125}, {"WIS", -70},   {"CHA", -300},
            {"MAXHP", 500}, {"STR", 800}, {"CON", -1428}, {"INT", -333},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
    SECTION("setOther") {
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP normal setOther AC", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("STR earliest setOther DEX", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("CON latest setOther WIS", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("INT early setOther CHA", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP normal maxOther AC", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("STR earliest maxOther DEX", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("CON latest maxOther WIS", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("INT early maxOther CHA", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP normal minOther AC", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("STR earliest minOther DEX", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("CON latest minOther WIS", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("INT early minOther CHA", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, constants));
        const std::unordered_map<std::string, int> result = {
            {"AC", 2000},    {"DEX", 125}, {"WIS", -70}, {"CHA", -300},
            {"MAXHP", 1000}, {"STR", 125}, {"CON", -70}, {"INT", -300},
        };
        for (const auto& [attr_name, attr_val] : attributes) {
            REQUIRE(attr_val == result.at(attr_name));
        }
    }
}

TEST_CASE("dnd::EffectHolderParser::parseAndAddEffect: parse valid 'Const' identifier effects") {
    TestEffectHolderParser parser;
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
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP normal addConst LEVEL", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("STR earliest addConst XP", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("CON latest addConst CONST1", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("INT early addConst CONST2", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP normal multConst LEVEL", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("STR earliest multConst XP", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("CON latest multConst CONST1", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("INT early multConst CONST2", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP normal divConst LEVEL", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("STR earliest divConst XP", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("CON latest divConst CONST1", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("INT early divConst CONST2", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP normal setConst LEVEL", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("STR earliest setConst XP", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("CON latest setConst CONST1", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("INT early setConst CONST2", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP normal maxConst LEVEL", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("STR earliest maxConst XP", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("CON latest maxConst CONST1", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("INT early maxConst CONST2", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(
            eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, max_min_constants)
        );
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, max_min_constants)
        );
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, max_min_constants)
        );
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, max_min_constants));
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
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP normal minConst LEVEL", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("STR earliest minConst XP", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("CON latest minConst CONST1", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("INT early minConst CONST2", eh));
        REQUIRE(eh.ability_score_effects.size() == 3);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE(eh.normal_effects.size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE_NOTHROW(
            eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, max_min_constants)
        );
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, max_min_constants)
        );
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, max_min_constants)
        );
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, max_min_constants));
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

TEST_CASE("dnd::EffectHolderParser::parseAndAddEffect: parse effect combinations") {
    TestEffectHolderParser parser;
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
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP earliest setConst CONST2", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP early div -4", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP normal add 2", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP late multOther STR", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP late mult -1", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP latest addConst LEVEL", eh));
        REQUIRE(eh.ability_score_effects.empty());
        REQUIRE(eh.normal_effects.size() == 5);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::LATE).size() == 2);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::LATE)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::LATE)[1]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, constants));
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
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("STR earliest setConst XP", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("STR early mult -2", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP early div 2", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP normal add 2", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP normal add 1", eh));
        REQUIRE_NOTHROW(parser.parse_and_add_effect_for_testing("MAXHP latest addOther STR", eh));
        REQUIRE(eh.ability_score_effects.size() == 2);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.normal_effects.size() == 3);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 2);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[1]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, constants));
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

TEST_CASE("dnd::EffectHolderParser::parseAndAddActivation: parse invalid activations") {
    TestEffectHolderParser parser;
    dnd::EffectHolder eh;
    SECTION("wrong format or order") {
        REQUIRE_THROWS(parser.parse_and_add_activation_for_testing("hello", eh));
        REQUIRE_THROWS(parser.parse_and_add_activation_for_testing("", eh));
        REQUIRE_THROWS(parser.parse_and_add_activation_for_testing("CLASS_LEVEL>=2", eh));
        REQUIRE_THROWS(parser.parse_and_add_activation_for_testing("LEVEL== 1", eh));
        REQUIRE_THROWS(parser.parse_and_add_activation_for_testing("ARMOR_ON <0", eh));
        REQUIRE_THROWS(parser.parse_and_add_activation_for_testing("== CLASS_LEVEL LEVEL", eh));
        REQUIRE_THROWS(parser.parse_and_add_activation_for_testing("LEVEL 1 ==", eh));
        REQUIRE_THROWS(parser.parse_and_add_activation_for_testing("CLASS_LEVEL 5", eh));
        REQUIRE_THROWS(parser.parse_and_add_activation_for_testing("LEVEL is CLASS_LEVEL", eh));
        REQUIRE_THROWS(parser.parse_and_add_activation_for_testing("INT 12 greater", eh));
        REQUIRE_THROWS(parser.parse_and_add_activation_for_testing("ARMOR_ON 1 ==", eh));
        REQUIRE_THROWS(parser.parse_and_add_activation_for_testing("ARMOR_ON", eh));
        REQUIRE_THROWS(parser.parse_and_add_activation_for_testing("true == ARMOR_ON", eh));
        REQUIRE_THROWS(parser.parse_and_add_activation_for_testing("13 <= INT", eh));
    }
}

TEST_CASE("dnd::EffectHolderParser::parseAndAddActivation: parse valid numeric activations") {
    TestEffectHolderParser parser;
    dnd::EffectHolder eh;
    REQUIRE(eh.activations.empty());
    std::unordered_map<std::string, int> attributes = {{"MAXHP", 4000}, {"STR", 1600}, {"CON", 1300}, {"INT", 800}};
    const std::unordered_map<std::string, int> constants = {{"LEVEL", 500}, {"CLASS_LEVEL", 500}, {"ARMOR_ON", 1}};
    size_t idx = 0;
    SECTION("==") {
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("LEVEL == 5", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("CLASS_LEVEL == 3", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("ARMOR_ON == true", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("ARMOR_ON == false", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("CON == 13", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("STR == 18", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
    }
    SECTION("!=") {
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("CLASS_LEVEL != 3", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("LEVEL != 5", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("ARMOR_ON != false", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("ARMOR_ON != true", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("STR != 18", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("CON != 13", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
    }
    SECTION(">=") {
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("CLASS_LEVEL >= 3", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("LEVEL >= 5", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("CLASS_LEVEL >= 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("MAXHP >= 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("STR >= 16", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("INT >= 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
    }
    SECTION("<=") {
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("CLASS_LEVEL <= 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("LEVEL <= 5", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("CLASS_LEVEL <= 3", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("STR <= 16", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("INT <= 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("MAXHP <= 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
    }
    SECTION(">") {
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("CLASS_LEVEL > 3", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("LEVEL > 5", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("CLASS_LEVEL > 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("MAXHP > 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("STR > 16", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("INT > 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
    }
    SECTION("<") {
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("CLASS_LEVEL < 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("LEVEL < 5", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("CLASS_LEVEL < 3", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("INT < 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("STR < 16", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("MAXHP < 10", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx++)->check(attributes, constants));
    }
}

TEST_CASE("dnd::EffectHolderParser::createActivation: parse valid identifier activations") {
    TestEffectHolderParser parser;
    dnd::EffectHolder eh;
    REQUIRE(eh.activations.empty());
    std::unordered_map<std::string, int> attributes1 = {{"MAXHP", 4000}, {"STR", 1600}, {"CON", 1300}, {"INT", 800}};
    const std::unordered_map<std::string, int> constants1 = {{"LEVEL", 500}, {"CLASS_LEVEL", 500}, {"ARMOR_ON", 1}};
    std::unordered_map<std::string, int> attributes2 = {{"MAXHP", 2300}, {"STR", 800}, {"CON", 1000}, {"INT", 1800}};
    const std::unordered_map<std::string, int> constants2 = {{"LEVEL", 1300}, {"CLASS_LEVEL", 1300}, {"ARMOR_ON", 0}};
    size_t idx = 0;
    SECTION("==") {
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("LEVEL == CLASS_LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("INT == LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("CON == STR", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
    }
    SECTION("!=") {
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("LEVEL != CLASS_LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("INT != LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("CON != STR", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
    }
    SECTION(">=") {
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("LEVEL >= CLASS_LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("MAXHP >= INT", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("LEVEL >= CON", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("ARMOR_ON >= LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
    }
    SECTION("<=") {
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("LEVEL <= CLASS_LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("MAXHP <= INT", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("LEVEL <= CON", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("ARMOR_ON <= LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
    }
    SECTION(">") {
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("LEVEL > CLASS_LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("MAXHP > INT", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("LEVEL > CON", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("ARMOR_ON > LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
    }
    SECTION("<") {
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("LEVEL < CLASS_LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("MAXHP < INT", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(!eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("LEVEL < CON", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(!eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(!eh.activations.at(idx++)->check(attributes2, constants2));
        REQUIRE_NOTHROW(parser.parse_and_add_activation_for_testing("ARMOR_ON < LEVEL", eh));
        REQUIRE(eh.activations.size() == idx + 1);
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants1));
        REQUIRE(eh.activations.at(idx)->check(attributes1, constants2));
        REQUIRE(eh.activations.at(idx)->check(attributes2, constants1));
        REQUIRE(eh.activations.at(idx++)->check(attributes2, constants2));
    }
}

TEST_CASE("dnd::EffectHolderParser::createFeature: parse valid effect holders") {
    TestEffectHolderParser parser;
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
        REQUIRE_NOTHROW(eh = parser.create_effect_holder_for_testing(eh_json));
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
        REQUIRE_NOTHROW(eh = parser.create_effect_holder_for_testing(eh_json));
        REQUIRE(eh.ability_score_effects.empty());
        REQUIRE(eh.normal_effects.size() == 5);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::LATE).size() == 2);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::LATE)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::LATE)[1]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, constants));
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
        REQUIRE_NOTHROW(eh = parser.create_effect_holder_for_testing(eh_json));
        REQUIRE(eh.ability_score_effects.size() == 2);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST).size() == 1);
        REQUIRE(eh.ability_score_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.normal_effects.size() == 3);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::EARLY).size() == 1);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::NORMAL).size() == 2);
        REQUIRE(eh.normal_effects.at(dnd::EffectTime::LATEST).size() == 1);
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLIEST)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.ability_score_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::EARLY)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[0]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::NORMAL)[1]->apply_to(attributes, constants));
        REQUIRE_NOTHROW(eh.normal_effects.at(dnd::EffectTime::LATEST)[0]->apply_to(attributes, constants));
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
