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
    // TODO: change, when CreatureState is properly implemented
    dnd::CreatureState teststate;
    teststate.attributes = {
        {"MAXHP", 1000}, {"STR", 1000}, {"CON", 1000}, {"INT", 1000}
    };
    SECTION("add") {
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal add 2", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest add 1.25", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest add -0.7", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early add -3", feature));
        REQUIRE(feature.earliest.size() == 1);
        REQUIRE(feature.early.size() == 1);
        REQUIRE(feature.normal.size() == 1);
        REQUIRE(feature.late.size() == 0);
        REQUIRE(feature.latest.size() == 1);
        // TODO: replace, when Feature class has apply functionality
        REQUIRE_NOTHROW(feature.earliest[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.early[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.normal[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.latest[0]->applyTo(teststate));
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 1200}, {"STR", 1125}, {"CON", 930}, {"INT", 700}
        };
        REQUIRE(teststate.attributes == result);
    }
    SECTION("mult") {
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal mult 2", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest mult 1.25", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest mult -0.7", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early mult -3", feature));
        REQUIRE(feature.earliest.size() == 1);
        REQUIRE(feature.early.size() == 1);
        REQUIRE(feature.normal.size() == 1);
        REQUIRE(feature.late.size() == 0);
        REQUIRE(feature.latest.size() == 1);
        // TODO: replace, when Feature class has apply functionality
        REQUIRE_NOTHROW(feature.earliest[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.early[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.normal[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.latest[0]->applyTo(teststate));
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 2000}, {"STR", 1250}, {"CON", -700}, {"INT", -3000}
        };
        REQUIRE(teststate.attributes == result);
    }
    SECTION("div") {
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal div 2", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest div 1.25", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest div -0.7", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early div -3", feature));
        REQUIRE(feature.earliest.size() == 1);
        REQUIRE(feature.early.size() == 1);
        REQUIRE(feature.normal.size() == 1);
        REQUIRE(feature.late.size() == 0);
        REQUIRE(feature.latest.size() == 1);
        // TODO: replace, when Feature class has apply functionality
        REQUIRE_NOTHROW(feature.earliest[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.early[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.normal[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.latest[0]->applyTo(teststate));
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 500}, {"STR", 800}, {"CON", -1428}, {"INT", -333}
        };
        REQUIRE(teststate.attributes == result);
    }
    SECTION("div") {
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal set 2", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest set 1.25", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest set -0.7", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early set -3", feature));
        REQUIRE(feature.earliest.size() == 1);
        REQUIRE(feature.early.size() == 1);
        REQUIRE(feature.normal.size() == 1);
        REQUIRE(feature.late.size() == 0);
        REQUIRE(feature.latest.size() == 1);
        // TODO: replace, when Feature class has apply functionality
        REQUIRE_NOTHROW(feature.earliest[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.early[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.normal[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.latest[0]->applyTo(teststate));
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 200}, {"STR", 125}, {"CON", -70}, {"INT", -300}
        };
        REQUIRE(teststate.attributes == result);
    }
}

TEST_CASE("dnd::FeatureParser::parseAndAddEffect: parse valid 'Other' identifier effects") {
    dnd::Feature feature("test", "feature for testing of effect parsing");
    // TODO: change, when CreatureState is properly implemented
    dnd::CreatureState teststate;
    teststate.attributes = {
        {"AC", 200}, {"DEX", 125}, {"WIS", -70}, {"CHA", -300},
        {"MAXHP", 1000}, {"STR", 1000}, {"CON", 1000}, {"INT", 1000}
    };
    SECTION("addOther") {
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal addOther AC", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest addOther DEX", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest addOther WIS", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early addOther CHA", feature));
        REQUIRE(feature.earliest.size() == 1);
        REQUIRE(feature.early.size() == 1);
        REQUIRE(feature.normal.size() == 1);
        REQUIRE(feature.late.size() == 0);
        REQUIRE(feature.latest.size() == 1);
        // TODO: replace, when Feature class has apply functionality
        REQUIRE_NOTHROW(feature.earliest[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.early[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.normal[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.latest[0]->applyTo(teststate));
        const std::unordered_map<std::string, int> result = {
            {"AC", 200}, {"DEX", 125}, {"WIS", -70}, {"CHA", -300},
            {"MAXHP", 1200}, {"STR", 1125}, {"CON", 930}, {"INT", 700}
        };
        REQUIRE(teststate.attributes == result);
    }
    SECTION("multOther") {
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal multOther AC", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest multOther DEX", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest multOther WIS", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early multOther CHA", feature));
        REQUIRE(feature.earliest.size() == 1);
        REQUIRE(feature.early.size() == 1);
        REQUIRE(feature.normal.size() == 1);
        REQUIRE(feature.late.size() == 0);
        REQUIRE(feature.latest.size() == 1);
        // TODO: replace, when Feature class has apply functionality
        REQUIRE_NOTHROW(feature.earliest[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.early[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.normal[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.latest[0]->applyTo(teststate));
        const std::unordered_map<std::string, int> result = {
            {"AC", 200}, {"DEX", 125}, {"WIS", -70}, {"CHA", -300},
            {"MAXHP", 2000}, {"STR", 1250}, {"CON", -700}, {"INT", -3000}
        };
        REQUIRE(teststate.attributes == result);
    }
    SECTION("divOther") {
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal divOther AC", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest divOther DEX", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest divOther WIS", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early divOther CHA", feature));
        REQUIRE(feature.earliest.size() == 1);
        REQUIRE(feature.early.size() == 1);
        REQUIRE(feature.normal.size() == 1);
        REQUIRE(feature.late.size() == 0);
        REQUIRE(feature.latest.size() == 1);
        // TODO: replace, when Feature class has apply functionality
        REQUIRE_NOTHROW(feature.earliest[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.early[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.normal[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.latest[0]->applyTo(teststate));
        const std::unordered_map<std::string, int> result = {
            {"AC", 200}, {"DEX", 125}, {"WIS", -70}, {"CHA", -300},
            {"MAXHP", 500}, {"STR", 800}, {"CON", -1428}, {"INT", -333}
        };
        REQUIRE(teststate.attributes == result);
    }
    SECTION("setOther") {
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal setOther AC", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest setOther DEX", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest setOther WIS", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early setOther CHA", feature));
        REQUIRE(feature.earliest.size() == 1);
        REQUIRE(feature.early.size() == 1);
        REQUIRE(feature.normal.size() == 1);
        REQUIRE(feature.late.size() == 0);
        REQUIRE(feature.latest.size() == 1);
        // TODO: replace, when Feature class has apply functionality
        REQUIRE_NOTHROW(feature.earliest[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.early[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.normal[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.latest[0]->applyTo(teststate));
        const std::unordered_map<std::string, int> result = {
            {"AC", 200}, {"DEX", 125}, {"WIS", -70}, {"CHA", -300},
            {"MAXHP", 200}, {"STR", 125}, {"CON", -70}, {"INT", -300}
        };
        REQUIRE(teststate.attributes == result);
    }
}

TEST_CASE("dnd::FeatureParser::parseAndAddEffect: parse valid 'Const' identifier effects") {
    dnd::Feature feature("test", "feature for testing of effect parsing");
    const std::unordered_map<std::string, int> constants = {
        {"LEVEL", 200}, {"ARMOR_ON", 125}, {"CONST1", -70}, {"CONST2", -300}
    };
    // TODO: change, when CreatureState is properly implemented
    dnd::CreatureState teststate(constants);
    REQUIRE(teststate.constants == constants);
    teststate.attributes = {
        {"MAXHP", 1000}, {"STR", 1000}, {"CON", 1000}, {"INT", 1000}
    };
    SECTION("addConst") {
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal addConst LEVEL", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest addConst ARMOR_ON", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest addConst CONST1", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early addConst CONST2", feature));
        REQUIRE(feature.earliest.size() == 1);
        REQUIRE(feature.early.size() == 1);
        REQUIRE(feature.normal.size() == 1);
        REQUIRE(feature.late.size() == 0);
        REQUIRE(feature.latest.size() == 1);
        // TODO: replace, when Feature class has apply functionality
        REQUIRE_NOTHROW(feature.earliest[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.early[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.normal[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.latest[0]->applyTo(teststate));
        REQUIRE(teststate.constants == constants);
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 1200}, {"STR", 1125}, {"CON", 930}, {"INT", 700}
        };
        REQUIRE(teststate.attributes == result);
    }
    SECTION("multConst") {
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal multConst LEVEL", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest multConst ARMOR_ON", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest multConst CONST1", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early multConst CONST2", feature));
        REQUIRE(feature.earliest.size() == 1);
        REQUIRE(feature.early.size() == 1);
        REQUIRE(feature.normal.size() == 1);
        REQUIRE(feature.late.size() == 0);
        REQUIRE(feature.latest.size() == 1);
        // TODO: replace, when Feature class has apply functionality
        REQUIRE_NOTHROW(feature.earliest[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.early[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.normal[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.latest[0]->applyTo(teststate));
        REQUIRE(teststate.constants == constants);
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 2000}, {"STR", 1250}, {"CON", -700}, {"INT", -3000}
        };
        REQUIRE(teststate.attributes == result);
    }
    SECTION("divConst") {
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal divConst LEVEL", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest divConst ARMOR_ON", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest divConst CONST1", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early divConst CONST2", feature));
        REQUIRE(feature.earliest.size() == 1);
        REQUIRE(feature.early.size() == 1);
        REQUIRE(feature.normal.size() == 1);
        REQUIRE(feature.late.size() == 0);
        REQUIRE(feature.latest.size() == 1);
        // TODO: replace, when Feature class has apply functionality
        REQUIRE_NOTHROW(feature.earliest[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.early[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.normal[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.latest[0]->applyTo(teststate));
        REQUIRE(teststate.constants == constants);
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 500}, {"STR", 800}, {"CON", -1428}, {"INT", -333}
        };
        REQUIRE(teststate.attributes == result);
    }
    SECTION("setConst") {
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal setConst LEVEL", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest setConst ARMOR_ON", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("CON latest setConst CONST1", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("INT early setConst CONST2", feature));
        REQUIRE(feature.earliest.size() == 1);
        REQUIRE(feature.early.size() == 1);
        REQUIRE(feature.normal.size() == 1);
        REQUIRE(feature.late.size() == 0);
        REQUIRE(feature.latest.size() == 1);
        // TODO: replace, when Feature class has apply functionality
        REQUIRE_NOTHROW(feature.earliest[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.early[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.normal[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.latest[0]->applyTo(teststate));
        REQUIRE(teststate.constants == constants);
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 200}, {"STR", 125}, {"CON", -70}, {"INT", -300}
        };
        REQUIRE(teststate.attributes == result);
    }
}

TEST_CASE("dnd::FeatureParser::parseAndAddEffect: parse effect combinations") {
    dnd::Feature feature("test", "feature for testing of effect parsing");
    const std::unordered_map<std::string, int> constants = {
        {"LEVEL", 200}, {"ARMOR_ON", 125}, {"CONST1", -70}, {"CONST2", -300}
    };
    // TODO: change, when CreatureState is properly implemented
    dnd::CreatureState teststate(constants);
    REQUIRE(teststate.constants == constants);
    teststate.attributes = {
        {"MAXHP", 1000}, {"STR", 1000}, {"CON", 1000}, {"INT", 1000}
    };
    SECTION("combination 1 (order of calculation)") {
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP earliest setConst CONST2", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP early div -4", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal add 2", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP late multOther STR", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP late mult -1", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP latest addConst LEVEL", feature));
        REQUIRE(feature.earliest.size() == 1);
        REQUIRE(feature.early.size() == 1);
        REQUIRE(feature.normal.size() == 1);
        REQUIRE(feature.late.size() == 2);
        REQUIRE(feature.latest.size() == 1);
        // TODO: replace, when Feature class has apply functionality
        REQUIRE_NOTHROW(feature.earliest[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.early[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.normal[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.late[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.late[1]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.latest[0]->applyTo(teststate));
        REQUIRE(teststate.constants == constants);
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", -2550}, {"STR", 1000}, {"CON", 1000}, {"INT", 1000}
        };
        REQUIRE(teststate.attributes == result);
    }
    SECTION("combination 2") {
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR earliest setConst ARMOR_ON", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("STR early mult -2", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP early div 2", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal add 2", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP normal add 1", feature));
        REQUIRE_NOTHROW(dnd::FeatureParser::parseAndAddEffect("MAXHP latest addOther STR", feature));
        REQUIRE(feature.earliest.size() == 1);
        REQUIRE(feature.early.size() == 2);
        REQUIRE(feature.normal.size() == 2);
        REQUIRE(feature.late.size() == 0);
        REQUIRE(feature.latest.size() == 1);
        // TODO: replace, when Feature class has apply functionality
        REQUIRE_NOTHROW(feature.earliest[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.early[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.early[1]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.normal[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.normal[1]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.latest[0]->applyTo(teststate));
        REQUIRE(teststate.constants == constants);
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 550}, {"STR", -250}, {"CON", 1000}, {"INT", 1000}
        };
        REQUIRE(teststate.attributes == result);
    }
}

TEST_CASE("dnd::FeatureParser::addEffects: invalid JSON format") {
    dnd::Feature feature("test", "feature for testing of effect parsing");
    SECTION("JSON is object/map") {
        const nlohmann::json effects_json = {
            {"1st effect", "CON normal add 2"},
            {"2nd effect", "INT normal mult 3"}
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
        {"LEVEL", 200}, {"ARMOR_ON", 125}, {"CONST1", -70}, {"CONST2", -300}
    };
    // TODO: change, when CreatureState is properly implemented
    dnd::CreatureState teststate(constants);
    REQUIRE(teststate.constants == constants);
    teststate.attributes = {
        {"MAXHP", 1000}, {"STR", 1000}, {"CON", 1000}, {"INT", 1000}
    };
    SECTION("combination 1 (order of calculation)") {
        const nlohmann::json effects_json = {
            "MAXHP earliest setConst CONST2",
            "MAXHP early div -4",
            "MAXHP normal add 2",
            "MAXHP late multOther STR",
            "MAXHP late mult -1",
            "MAXHP latest addConst LEVEL"
        };
        REQUIRE_NOTHROW(dnd::FeatureParser::addEffects(effects_json, feature));
        REQUIRE(feature.earliest.size() == 1);
        REQUIRE(feature.early.size() == 1);
        REQUIRE(feature.normal.size() == 1);
        REQUIRE(feature.late.size() == 2);
        REQUIRE(feature.latest.size() == 1);
        // TODO: replace, when Feature class has apply functionality
        REQUIRE_NOTHROW(feature.earliest[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.early[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.normal[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.late[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.late[1]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.latest[0]->applyTo(teststate));
        REQUIRE(teststate.constants == constants);
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", -2550}, {"STR", 1000}, {"CON", 1000}, {"INT", 1000}
        };
        REQUIRE(teststate.attributes == result);
    }
    SECTION("combination 2") {
        const nlohmann::json effects_json = {
            "STR earliest setConst ARMOR_ON",
            "STR early mult -2",
            "MAXHP early div 2",
            "MAXHP normal add 2",
            "MAXHP normal add 1",
            "MAXHP latest addOther STR"
        };
        REQUIRE_NOTHROW(dnd::FeatureParser::addEffects(effects_json, feature));
        REQUIRE(feature.earliest.size() == 1);
        REQUIRE(feature.early.size() == 2);
        REQUIRE(feature.normal.size() == 2);
        REQUIRE(feature.late.size() == 0);
        REQUIRE(feature.latest.size() == 1);
        // TODO: replace, when Feature class has apply functionality
        REQUIRE_NOTHROW(feature.earliest[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.early[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.early[1]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.normal[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.normal[1]->applyTo(teststate));
        REQUIRE_NOTHROW(feature.latest[0]->applyTo(teststate));
        REQUIRE(teststate.constants == constants);
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 550}, {"STR", -250}, {"CON", 1000}, {"INT", 1000}
        };
        REQUIRE(teststate.attributes == result);
    }
}

TEST_CASE("dnd::FeatureParser::createFeature: invalid JSON format") {
    dnd::Feature feature("test", "feature for testing of effect parsing");
    SECTION("JSON is array") {
        const nlohmann::json feature_json = {
            "feature for testing of effect parsing",
            {"CON normal add 2", "INT normal mult 3"}
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
        {"LEVEL", 200}, {"ARMOR_ON", 125}, {"CONST1", -70}, {"CONST2", -300}
    };
    // TODO: change, when CreatureState is properly implemented
    dnd::CreatureState teststate(constants);
    REQUIRE(teststate.constants == constants);
    teststate.attributes = {
        {"MAXHP", 1000}, {"STR", 1000}, {"CON", 1000}, {"INT", 1000}
    };
    const std::string name = "test";
    const std::string description = "feature for testing of effect parsing";
    SECTION("feature without effects") {
        // TODO: add more testcases when activation etc. is implemented
        const nlohmann::json feature_json = {
            {"description", description}
        };
        std::unique_ptr<dnd::Feature> feature;
        REQUIRE_NOTHROW(feature = std::move(dnd::FeatureParser::createFeature(name, feature_json)));
        REQUIRE(feature->name == name);
        REQUIRE(feature->description == description);
        REQUIRE(feature->earliest.size() == 0);
        REQUIRE(feature->early.size() == 0);
        REQUIRE(feature->normal.size() == 0);
        REQUIRE(feature->late.size() == 0);
        REQUIRE(feature->latest.size() == 0);
    }
    SECTION("effect combination 1 (order of calculation)") {
        const nlohmann::json effects_json = {
            "MAXHP earliest setConst CONST2",
            "MAXHP early div -4",
            "MAXHP normal add 2",
            "MAXHP late multOther STR",
            "MAXHP late mult -1",
            "MAXHP latest addConst LEVEL"
        };
        // TODO: add more testcases when activation etc. is implemented
        const nlohmann::json feature_json = {
            {"description", description},
            {"effects", effects_json}
        };
        std::unique_ptr<dnd::Feature> feature;
        REQUIRE_NOTHROW(feature = std::move(dnd::FeatureParser::createFeature(name, feature_json)));
        REQUIRE(feature->name == name);
        REQUIRE(feature->description == description);
        REQUIRE(feature->earliest.size() == 1);
        REQUIRE(feature->early.size() == 1);
        REQUIRE(feature->normal.size() == 1);
        REQUIRE(feature->late.size() == 2);
        REQUIRE(feature->latest.size() == 1);
        // TODO: replace, when Feature class has apply functionality
        REQUIRE_NOTHROW(feature->earliest[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature->early[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature->normal[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature->late[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature->late[1]->applyTo(teststate));
        REQUIRE_NOTHROW(feature->latest[0]->applyTo(teststate));
        REQUIRE(teststate.constants == constants);
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", -2550}, {"STR", 1000}, {"CON", 1000}, {"INT", 1000}
        };
        REQUIRE(teststate.attributes == result);
    }
    SECTION("effect combination 2") {
        const nlohmann::json effects_json = {
            "STR earliest setConst ARMOR_ON",
            "STR early mult -2",
            "MAXHP early div 2",
            "MAXHP normal add 2",
            "MAXHP normal add 1",
            "MAXHP latest addOther STR"
        };
        // TODO: add more testcases when activation etc. is implemented
        const nlohmann::json feature_json = {
            {"description", description},
            {"effects", effects_json}
        };
        std::unique_ptr<dnd::Feature> feature;
        REQUIRE_NOTHROW(feature = std::move(dnd::FeatureParser::createFeature(name, feature_json)));
        REQUIRE(feature->name == name);
        REQUIRE(feature->description == description);
        REQUIRE(feature->earliest.size() == 1);
        REQUIRE(feature->early.size() == 2);
        REQUIRE(feature->normal.size() == 2);
        REQUIRE(feature->late.size() == 0);
        REQUIRE(feature->latest.size() == 1);
        // TODO: replace, when Feature class has apply functionality
        REQUIRE_NOTHROW(feature->earliest[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature->early[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature->early[1]->applyTo(teststate));
        REQUIRE_NOTHROW(feature->normal[0]->applyTo(teststate));
        REQUIRE_NOTHROW(feature->normal[1]->applyTo(teststate));
        REQUIRE_NOTHROW(feature->latest[0]->applyTo(teststate));
        REQUIRE(teststate.constants == constants);
        const std::unordered_map<std::string, int> result = {
            {"MAXHP", 550}, {"STR", -250}, {"CON", 1000}, {"INT", 1000}
        };
        REQUIRE(teststate.attributes == result);
    }
}
