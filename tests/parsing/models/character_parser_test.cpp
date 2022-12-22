#include "parsing/models/character_parser.hpp"

#include <memory>
#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#include "controllers/content_controller.hpp"
#include "models/character.hpp"
#include "models/character_class.hpp"
#include "models/character_race.hpp"

std::shared_ptr<dnd::ContentController> setupContentControllerForTesting() {
    dnd::ContentController test_controller;
    test_controller.character_classes = {
        {"Barbarian", std::make_shared<dnd::CharacterClass>("Barbarian", "1d12")},
    };
    test_controller.character_subclasses = {
        {"Path of the Berserker", std::make_shared<dnd::CharacterSubclass>("Path of the Berserker", "Barbarian")},
    };
    test_controller.character_races = {
        {"Dwarf", std::make_shared<dnd::CharacterRace>("Dwarf", true)},
    };
    test_controller.character_subraces = {
        {"Hill Dwarf", std::make_shared<dnd::CharacterSubrace>("Hill Dwarf", "Dwarf")},
    };
    return std::make_shared<dnd::ContentController>(std::move(test_controller));
}

TEST_CASE("dnd::CharacterParser::createCharacter: parse characters of invalid format") {
    dnd::ContentController test_controller = *setupContentControllerForTesting();
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
    SECTION("JSON is no object/map") {
        // JSON is literal
        REQUIRE_THROWS(dnd::CharacterParser::createCharacter(true, test_controller));
        REQUIRE_THROWS(dnd::CharacterParser::createCharacter(1, test_controller));
        REQUIRE_THROWS(dnd::CharacterParser::createCharacter(-3.4, test_controller));
        REQUIRE_THROWS(dnd::CharacterParser::createCharacter("string", test_controller));
        // JSON is array
        nlohmann::json valid_low_level_bob_array = nlohmann::json::array();
        for (const auto& [key, value] : valid_low_level_bob.items()) {
            valid_low_level_bob_array.push_back(value);
        }
        REQUIRE_THROWS(dnd::CharacterParser::createCharacter(valid_low_level_bob_array, test_controller));
        nlohmann::json valid_high_level_bob_array = nlohmann::json::array();
        for (const auto& [key, value] : valid_high_level_bob.items()) {
            valid_high_level_bob_array.push_back(value);
        }
        REQUIRE_THROWS(dnd::CharacterParser::createCharacter(valid_high_level_bob_array, test_controller));
    }
    SECTION("character has no name") {
        valid_low_level_bob.erase("name");
        REQUIRE_THROWS(dnd::CharacterParser::createCharacter(valid_low_level_bob, test_controller));
        valid_high_level_bob.erase("name");
        REQUIRE_THROWS(dnd::CharacterParser::createCharacter(valid_high_level_bob, test_controller));
    }
    SECTION("character has no class") {
        valid_low_level_bob.erase("class");
        REQUIRE_THROWS(dnd::CharacterParser::createCharacter(valid_low_level_bob, test_controller));
        valid_high_level_bob.erase("class");
        REQUIRE_THROWS(dnd::CharacterParser::createCharacter(valid_high_level_bob, test_controller));
    }
    SECTION("character has no race") {
        valid_low_level_bob.erase("race");
        REQUIRE_THROWS(dnd::CharacterParser::createCharacter(valid_low_level_bob, test_controller));
        valid_high_level_bob.erase("race");
        REQUIRE_THROWS(dnd::CharacterParser::createCharacter(valid_high_level_bob, test_controller));
    }
    SECTION("character has neither level nor xp") {
        valid_low_level_bob.erase("level");
        valid_low_level_bob.erase("xp");
        REQUIRE_THROWS(dnd::CharacterParser::createCharacter(valid_low_level_bob, test_controller));
        valid_high_level_bob.erase("level");
        valid_high_level_bob.erase("xp");
        REQUIRE_THROWS(dnd::CharacterParser::createCharacter(valid_high_level_bob, test_controller));
    }
}

TEST_CASE("dnd::CharacterParser::createCharacter: parse logically wrong character") {
    dnd::ContentController test_controller = *setupContentControllerForTesting();
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
        REQUIRE_THROWS(dnd::CharacterParser::createCharacter(valid_low_level_bob, test_controller));
        valid_high_level_bob.erase("subrace");
        REQUIRE_THROWS(dnd::CharacterParser::createCharacter(valid_high_level_bob, test_controller));
    }
    SECTION("character's xp value corresponds to another level than the level provided") {
        valid_low_level_bob.at("level") = 1;
        REQUIRE_THROWS(dnd::CharacterParser::createCharacter(valid_low_level_bob, test_controller));
        valid_high_level_bob.at("xp") = 200000; // corresponds to level 16
        REQUIRE_THROWS(dnd::CharacterParser::createCharacter(valid_high_level_bob, test_controller));
    }
}

void testBasicValuesFromJSON(
    const nlohmann::json& character_json, std::shared_ptr<const dnd::Character> character_ptr
) {
    REQUIRE(character_ptr->name == character_json.at("name").get<std::string>());
    REQUIRE(character_ptr->class_ptr->name == character_json.at("class"));
    if (character_json.contains("subclass")) {
        REQUIRE(character_ptr->subclass_ptr->name == character_json.at("subclass").get<std::string>());
    }
    REQUIRE(character_ptr->race_ptr->name == character_json.at("race").get<std::string>());
    if (character_json.contains("subrace")) {
        REQUIRE(character_ptr->subrace_ptr->name == character_json.at("subrace").get<std::string>());
    }
    REQUIRE(character_ptr->base_ability_scores == character_json.at("base_ability_scores").get<std::vector<int>>());
    if (character_json.contains("level")) {
        REQUIRE(character_ptr->getLevel() == character_json.at("level").get<int>());
    }
    if (character_json.contains("xp")) {
        REQUIRE(character_ptr->getXP() == character_json.at("xp").get<int>());
    }
    REQUIRE(character_ptr->getHitDiceRolls() == character_json.at("hit_dice_rolls").get<std::vector<int>>());
}

TEST_CASE("dnd::CharacterParser::createCharacter: parse minimum characters") {
    dnd::ContentController test_controller = *setupContentControllerForTesting();
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
    std::shared_ptr<const dnd::Character> character_ptr;
    SECTION("characters with level only") {
        valid_low_level_bob.erase("xp");
        REQUIRE_NOTHROW(character_ptr = dnd::CharacterParser::createCharacter(valid_low_level_bob, test_controller));
        testBasicValuesFromJSON(valid_low_level_bob, character_ptr);
        REQUIRE(character_ptr->getXP() == dnd::xp_for_level.at(valid_low_level_bob.at("level").get<int>()));
        valid_high_level_bob.erase("xp");
        REQUIRE_NOTHROW(character_ptr = dnd::CharacterParser::createCharacter(valid_high_level_bob, test_controller));
        testBasicValuesFromJSON(valid_high_level_bob, character_ptr);
        REQUIRE(character_ptr->getXP() == dnd::xp_for_level.at(valid_high_level_bob.at("level").get<int>()));
    }
    SECTION("characters with xp only") {
        valid_low_level_bob.erase("level");
        REQUIRE_NOTHROW(character_ptr = dnd::CharacterParser::createCharacter(valid_low_level_bob, test_controller));
        testBasicValuesFromJSON(valid_low_level_bob, character_ptr);
        REQUIRE(character_ptr->getLevel() == dnd::Character::levelForXP(valid_low_level_bob.at("xp").get<int>()));
        valid_high_level_bob.erase("level");
        REQUIRE_NOTHROW(character_ptr = dnd::CharacterParser::createCharacter(valid_high_level_bob, test_controller));
        testBasicValuesFromJSON(valid_high_level_bob, character_ptr);
        REQUIRE(character_ptr->getLevel() == dnd::Character::levelForXP(valid_high_level_bob.at("xp").get<int>()));
    }
    SECTION("characters with level and xp") {
        REQUIRE_NOTHROW(character_ptr = dnd::CharacterParser::createCharacter(valid_low_level_bob, test_controller));
        testBasicValuesFromJSON(valid_low_level_bob, character_ptr);
        REQUIRE_NOTHROW(character_ptr = dnd::CharacterParser::createCharacter(valid_high_level_bob, test_controller));
        testBasicValuesFromJSON(valid_high_level_bob, character_ptr);
    }
}

// TODO: add test case: parse characters with spellcasting
// TODO: add test case: parse characters with custom background
// TODO: add test case: parse characters with their own custom spells
// TODO: add test case: parse characters with their own custom features
// TODO: add test case: parse characters with equipment
// TODO: should the result of CharacterState calculation be tested here?
