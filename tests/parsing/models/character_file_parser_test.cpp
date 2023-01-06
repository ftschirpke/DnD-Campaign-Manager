#include "parsing/models/character_file_parser.hpp"

#include <array>
#include <memory>
#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#include "controllers/content.hpp"
#include "models/character.hpp"
#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/spell.hpp"
#include "parsing/models/character_file_parser.hpp"

class TestCharacterFileParser : public dnd::CharacterFileParser {
public:
    TestCharacterFileParser(
        std::unordered_map<std::string, dnd::Character>& results,
        const std::unordered_map<std::string, const dnd::CharacterClass>& character_classes,
        const std::unordered_map<std::string, const dnd::CharacterSubclass>& character_subclasses,
        const std::unordered_map<std::string, const dnd::CharacterRace>& character_races,
        const std::unordered_map<std::string, const dnd::CharacterSubrace>& character_subraces,
        const std::unordered_map<std::string, const dnd::Spell>& spells
    )
        : dnd::CharacterFileParser(
            results, character_classes, character_subclasses, character_races, character_subraces, spells
        ) {}
    void setJSON(const nlohmann::json& character_json) {
        filename = "test_file_name.json";
        json_to_parse = character_json;
    }
};

class SetupCharacterParserTest {
public:
    std::unordered_map<std::string, dnd::Character> characters;
    SetupCharacterParserTest();
    TestCharacterFileParser createParser();
private:
    static bool values_set;
    static std::unordered_map<std::string, const dnd::CharacterClass> character_classes;
    static std::unordered_map<std::string, const dnd::CharacterSubclass> character_subclasses;
    static std::unordered_map<std::string, const dnd::CharacterRace> character_races;
    static std::unordered_map<std::string, const dnd::CharacterSubrace> character_subraces;
    static std::unordered_map<std::string, const dnd::Spell> spells;
    static void setClasses();
    static void setSubclasses();
    static void setRaces();
    static void setSubraces();
    static void setSpells();
};

bool SetupCharacterParserTest::values_set = false;
std::unordered_map<std::string, const dnd::CharacterClass> SetupCharacterParserTest::character_classes = {};
std::unordered_map<std::string, const dnd::CharacterSubclass> SetupCharacterParserTest::character_subclasses = {};
std::unordered_map<std::string, const dnd::CharacterRace> SetupCharacterParserTest::character_races = {};
std::unordered_map<std::string, const dnd::CharacterSubrace> SetupCharacterParserTest::character_subraces = {};
std::unordered_map<std::string, const dnd::Spell> SetupCharacterParserTest::spells = {};

inline void SetupCharacterParserTest::setClasses() {
    character_classes.emplace(
        std::piecewise_construct, std::forward_as_tuple("Barbarian"),
        std::forward_as_tuple("Barbarian", "d12", std::vector<int>({4, 8, 12, 16, 19}), 3)
    );
}

inline void SetupCharacterParserTest::setSubclasses() {
    character_subclasses.emplace(
        std::piecewise_construct, std::forward_as_tuple("Path of the Berserker"),
        std::forward_as_tuple("Path of the Berserker", "Barbarian")
    );
}

inline void SetupCharacterParserTest::setRaces() {
    character_races.emplace(
        std::piecewise_construct, std::forward_as_tuple("Dwarf"), std::forward_as_tuple("Dwarf", true)
    );
}

inline void SetupCharacterParserTest::setSubraces() {
    character_subraces.emplace(
        std::piecewise_construct, std::forward_as_tuple("Hill Dwarf"), std::forward_as_tuple("Hill Dwarf", "Dwarf")
    );
}

inline void SetupCharacterParserTest::setSpells() {}


inline SetupCharacterParserTest::SetupCharacterParserTest() {
    if (!values_set) {
        setClasses();
        setSubclasses();
        setRaces();
        setSubraces();
        setSpells();
        values_set = true;
    }
}

inline TestCharacterFileParser SetupCharacterParserTest::createParser() {
    return TestCharacterFileParser(
        characters, character_classes, character_subclasses, character_races, character_subraces, spells
    );
}


TEST_CASE("dnd::CharacterParser::createCharacter: parse characters of invalid format") {
    SetupCharacterParserTest setup;
    auto parser = setup.createParser();
    // TODO: update once more features available
    // TODO: add more test characters?
    nlohmann::json low_level_bob = {
        {"name", "Bob"},
        {"class", "Barbarian"},
        {"race", "Dwarf"},
        {"subrace", "Hill Dwarf"},
        {"base_ability_scores", {15, 12, 14, 10, 8, 13}},
        {"level", 2},
        {"xp", 600},
        {"hit_dice_rolls", {8, 11}},
    };
    nlohmann::json high_level_bob = {
        {"name", "Bob the Great"},
        {"class", "Barbarian"},
        {"subclass", "Path of the Berserker"},
        {"race", "Dwarf"},
        {"subrace", "Hill Dwarf"},
        {"base_ability_scores", {15, 12, 14, 10, 8, 13}},
        {"level", 15},
        {"xp", 170000},
        {"hit_dice_rolls", {8, 11, 7, 6, 12, 4, 5, 3, 3, 7, 9, 7, 4, 2, 10}},
    };
    SECTION("JSON is no object/map") {
        // JSON is literal
        parser.setJSON(true);
        REQUIRE_THROWS(parser.parse());

        parser.setJSON(1);
        REQUIRE_THROWS(parser.parse());

        parser.setJSON(-3.4);
        REQUIRE_THROWS(parser.parse());

        parser.setJSON("string");
        REQUIRE_THROWS(parser.parse());
        // JSON is array
        nlohmann::json low_level_bob_array = nlohmann::json::array();
        for (const auto& [key, value] : low_level_bob.items()) {
            low_level_bob_array.emplace_back(value);
        }
        parser.setJSON(low_level_bob_array);
        REQUIRE_THROWS(parser.parse());

        nlohmann::json high_level_bob_array = nlohmann::json::array();
        for (const auto& [key, value] : high_level_bob.items()) {
            high_level_bob_array.emplace_back(value);
        }
        parser.setJSON(high_level_bob_array);
        REQUIRE_THROWS(parser.parse());
    }
    SECTION("character has no name") {
        low_level_bob.erase("name");
        parser.setJSON(low_level_bob);
        REQUIRE_THROWS(parser.parse());

        high_level_bob.erase("name");
        parser.setJSON(high_level_bob);
        REQUIRE_THROWS(parser.parse());
    }
    SECTION("character has no class") {
        low_level_bob.erase("class");
        parser.setJSON(low_level_bob);
        REQUIRE_THROWS(parser.parse());

        high_level_bob.erase("class");
        parser.setJSON(high_level_bob);
        REQUIRE_THROWS(parser.parse());
    }
    SECTION("character has no race") {
        low_level_bob.erase("race");
        parser.setJSON(low_level_bob);
        REQUIRE_THROWS(parser.parse());

        high_level_bob.erase("race");
        parser.setJSON(high_level_bob);
        REQUIRE_THROWS(parser.parse());
    }
    SECTION("character has neither level nor xp") {
        low_level_bob.erase("level");
        low_level_bob.erase("xp");
        parser.setJSON(low_level_bob);
        REQUIRE_THROWS(parser.parse());

        high_level_bob.erase("level");
        high_level_bob.erase("xp");
        parser.setJSON(high_level_bob);
        REQUIRE_THROWS(parser.parse());
    }
}

TEST_CASE("dnd::CharacterParser::createCharacter: parse logically wrong character") {
    SetupCharacterParserTest setup;
    auto parser = setup.createParser();
    // TODO: update once more features available
    // TODO: add more test characters?
    nlohmann::json valid_low_level_bob = {
        {"name", "Bob"},
        {"class", "Barbarian"},
        {"race", "Dwarf"},
        {"subrace", "Hill Dwarf"},
        {"base_ability_scores", {15, 12, 14, 10, 8, 13}},
        {"level", 2},
        {"xp", 600},
        {"hit_dice_rolls", {8, 11}},
    };
    nlohmann::json valid_high_level_bob = {
        {"name", "Bob the Great"},
        {"class", "Barbarian"},
        {"subclass", "Path of the Berserker"},
        {"race", "Dwarf"},
        {"subrace", "Hill Dwarf"},
        {"base_ability_scores", {15, 12, 14, 10, 8, 13}},
        {"level", 15},
        {"xp", 170000},
        {"hit_dice_rolls", {8, 11, 7, 6, 12, 4, 5, 3, 3, 7, 9, 7, 4, 2, 10}},
    };
    SECTION("character has no subrace although race requires subrace") {
        // TODO: add tests where subrace is not required
        valid_low_level_bob.erase("subrace");
        parser.setJSON(valid_low_level_bob);
        REQUIRE_THROWS(parser.parse());

        valid_high_level_bob.erase("subrace");
        parser.setJSON(valid_high_level_bob);
        REQUIRE_THROWS(parser.parse());
    }
    SECTION("character's xp value corresponds to another level than the level provided") {
        valid_low_level_bob.at("level") = 1;
        parser.setJSON(valid_low_level_bob);
        REQUIRE_THROWS(parser.parse());

        valid_high_level_bob.at("xp") = 200000; // corresponds to level 16
        valid_high_level_bob.erase("subrace");
        parser.setJSON(valid_high_level_bob);
        REQUIRE_THROWS(parser.parse());
    }
}

void testBasicValuesFromJSON(const nlohmann::json& character_json, const dnd::Character* const character_ptr) {
    REQUIRE(character_ptr->name == character_json.at("name").get<std::string>());
    REQUIRE(character_ptr->class_ptr->name == character_json.at("class"));
    if (character_json.contains("subclass")) {
        REQUIRE(character_ptr->subclass_ptr->name == character_json.at("subclass").get<std::string>());
    }
    REQUIRE(character_ptr->race_ptr->name == character_json.at("race").get<std::string>());
    if (character_json.contains("subrace")) {
        REQUIRE(character_ptr->subrace_ptr->name == character_json.at("subrace").get<std::string>());
    }
    REQUIRE(character_ptr->base_ability_scores == character_json.at("base_ability_scores").get<std::array<int, 6>>());
    if (character_json.contains("level")) {
        REQUIRE(character_ptr->getLevel() == character_json.at("level").get<int>());
    }
    if (character_json.contains("xp")) {
        REQUIRE(character_ptr->getXP() == character_json.at("xp").get<int>());
    }
    REQUIRE(character_ptr->getHitDiceRolls() == character_json.at("hit_dice_rolls").get<std::vector<int>>());
}

TEST_CASE("dnd::CharacterParser::createCharacter: parse minimum characters") {
    SetupCharacterParserTest setup;
    auto parser = setup.createParser();
    // TODO: update once more features available
    // TODO: add more test characters?
    nlohmann::json valid_low_level_bob = {
        {"name", "Bob"},
        {"class", "Barbarian"},
        {"race", "Dwarf"},
        {"subrace", "Hill Dwarf"},
        {"base_ability_scores", {15, 12, 14, 10, 8, 13}},
        {"level", 2},
        {"xp", 600},
        {"hit_dice_rolls", {8, 11}},
    };
    nlohmann::json valid_high_level_bob = {
        {"name", "Bob the Great"},
        {"class", "Barbarian"},
        {"subclass", "Path of the Berserker"},
        {"race", "Dwarf"},
        {"subrace", "Hill Dwarf"},
        {"base_ability_scores", {15, 12, 14, 10, 8, 13}},
        {"level", 15},
        {"xp", 170000},
        {"hit_dice_rolls", {8, 11, 7, 6, 12, 4, 5, 3, 3, 7, 9, 7, 4, 2, 10}},
    };
    const dnd::Character* character_ptr;
    SECTION("characters with level only") {
        valid_low_level_bob.erase("xp");
        parser.setJSON(valid_low_level_bob);
        REQUIRE_NOTHROW(parser.parse());
        REQUIRE(parser.validate());
        REQUIRE_NOTHROW(parser.saveResult());
        REQUIRE(setup.characters.size() == 1);
        character_ptr = &setup.characters.at(valid_low_level_bob.at("name"));
        testBasicValuesFromJSON(valid_low_level_bob, character_ptr);
        REQUIRE(character_ptr->getXP() == dnd::xp_for_level.at(valid_low_level_bob.at("level").get<int>()));

        valid_high_level_bob.erase("xp");
        parser.setJSON(valid_high_level_bob);
        REQUIRE_NOTHROW(parser.parse());
        REQUIRE(parser.validate());
        REQUIRE_NOTHROW(parser.saveResult());
        REQUIRE(setup.characters.size() == 2);
        character_ptr = &setup.characters.at(valid_high_level_bob.at("name"));
        testBasicValuesFromJSON(valid_high_level_bob, character_ptr);
        REQUIRE(character_ptr->getXP() == dnd::xp_for_level.at(valid_high_level_bob.at("level").get<int>()));
    }
    SECTION("characters with xp only") {
        valid_low_level_bob.erase("level");
        parser.setJSON(valid_low_level_bob);
        REQUIRE_NOTHROW(parser.parse());
        REQUIRE(parser.validate());
        REQUIRE_NOTHROW(parser.saveResult());
        REQUIRE(setup.characters.size() == 1);
        character_ptr = &setup.characters.at(valid_low_level_bob.at("name"));
        testBasicValuesFromJSON(valid_low_level_bob, character_ptr);
        REQUIRE(character_ptr->getLevel() == dnd::Character::levelForXP(valid_low_level_bob.at("xp").get<int>()));

        valid_high_level_bob.erase("level");
        parser.setJSON(valid_high_level_bob);
        REQUIRE_NOTHROW(parser.parse());
        REQUIRE(parser.validate());
        REQUIRE_NOTHROW(parser.saveResult());
        REQUIRE(setup.characters.size() == 2);
        character_ptr = &setup.characters.at(valid_high_level_bob.at("name"));
        testBasicValuesFromJSON(valid_high_level_bob, character_ptr);
        REQUIRE(character_ptr->getLevel() == dnd::Character::levelForXP(valid_high_level_bob.at("xp").get<int>()));
    }
    SECTION("characters with level and xp") {
        parser.setJSON(valid_low_level_bob);
        REQUIRE_NOTHROW(parser.parse());
        REQUIRE(parser.validate());
        REQUIRE_NOTHROW(parser.saveResult());
        REQUIRE(setup.characters.size() == 1);
        character_ptr = &setup.characters.at(valid_low_level_bob.at("name"));
        testBasicValuesFromJSON(valid_low_level_bob, character_ptr);

        parser.setJSON(valid_high_level_bob);
        REQUIRE_NOTHROW(parser.parse());
        REQUIRE(parser.validate());
        REQUIRE_NOTHROW(parser.saveResult());
        REQUIRE(setup.characters.size() == 2);
        character_ptr = &setup.characters.at(valid_high_level_bob.at("name"));
        testBasicValuesFromJSON(valid_high_level_bob, character_ptr);
    }
}

// TODO: add test case: parse characters with spellcasting
// TODO: add test case: parse characters with custom background
// TODO: add test case: parse characters with their own custom spells
// TODO: add test case: parse characters with their own custom features
// TODO: add test case: parse characters with equipment
// TODO: should the result of CharacterState calculation be tested here?
