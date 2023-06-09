#include <core/parsing/models/character_file_parser.hpp>

#include <array>
#include <filesystem>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#include <core/basic_mechanics/dice.hpp>
#include <core/controllers/content_holder.hpp>
#include <core/controllers/content_library.hpp>
#include <core/controllers/groups.hpp>
#include <core/models/character.hpp>
#include <core/models/character_class.hpp>
#include <core/models/character_race.hpp>
#include <core/models/character_subclass.hpp>
#include <core/models/character_subrace.hpp>
#include <core/models/effect_holder/feature.hpp>
#include <core/models/spell.hpp>

/**
 * @brief A class that allows us to test the dnd::CharacterFileParser class
 */
class TestCharacterFileParser : public dnd::CharacterFileParser {
public:
    TestCharacterFileParser(
        const std::filesystem::path& filepath, dnd::StorageContentLibrary<dnd::Character>& characters,
        const dnd::Groups& groups, const dnd::StorageContentLibrary<const dnd::CharacterClass>& character_classes,
        const dnd::StorageContentLibrary<const dnd::CharacterSubclass>& character_subclasses,
        const dnd::StorageContentLibrary<const dnd::CharacterRace>& character_races,
        const dnd::StorageContentLibrary<const dnd::CharacterSubrace>& character_subraces,
        const dnd::StorageContentLibrary<const dnd::Spell>& spells
    )
        : dnd::CharacterFileParser(
            filepath, characters, groups, character_classes, character_subclasses, character_races, character_subraces,
            spells
        ) {}
    void setJSON(const nlohmann::json& character_json) { json_to_parse = character_json; }
};

class SetupCharacterParserTest {
public:
    dnd::StorageContentLibrary<dnd::Character> characters;
    SetupCharacterParserTest();
    TestCharacterFileParser createParser();
private:
    dnd::Groups groups;
    const std::filesystem::path filepath;
    dnd::StorageContentLibrary<const dnd::CharacterClass> character_classes;
    dnd::StorageContentLibrary<const dnd::CharacterSubclass> character_subclasses;
    dnd::StorageContentLibrary<const dnd::CharacterRace> character_races;
    dnd::StorageContentLibrary<const dnd::CharacterSubrace> character_subraces;
    dnd::StorageContentLibrary<const dnd::Spell> spells;
    void set_classes();
    void set_subclasses();
    void set_races();
    void set_subraces();
    void set_spells();
};

void SetupCharacterParserTest::set_classes() {
    character_classes.create(
        "Barbarian", "dummy_path", std::vector<dnd::Feature>(), dnd::int_to_dice(12),
        std::vector<int>({4, 8, 12, 16, 19}), 3
    );
}

void SetupCharacterParserTest::set_subclasses() {
    character_subclasses.create("Path of the Berserker", "dummy_path", std::vector<dnd::Feature>(), "Barbarian");
}

void SetupCharacterParserTest::set_races() {
    character_races.create("Dwarf", "dummy_path", std::vector<dnd::Feature>(), true);
}

void SetupCharacterParserTest::set_subraces() {
    character_subraces.create("Hill Dwarf", "dummy_path", std::vector<dnd::Feature>(), "Dwarf");
}

void SetupCharacterParserTest::set_spells() {}


SetupCharacterParserTest::SetupCharacterParserTest() : groups(), filepath("testing") {
    set_classes();
    set_subclasses();
    set_races();
    set_subraces();
    set_spells();
}

TestCharacterFileParser SetupCharacterParserTest::createParser() {
    return TestCharacterFileParser(
        filepath, characters, groups, character_classes, character_subclasses, character_races, character_subraces,
        spells
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

void test_basic_values_from_json(const nlohmann::json& character_json, const dnd::Character* const character_ptr) {
    REQUIRE(character_ptr->get_name() == character_json.at("name").get<std::string>());
    REQUIRE(character_ptr->class_ptr->get_name() == character_json.at("class").get<std::string>());
    if (character_json.contains("subclass")) {
        REQUIRE(character_ptr->subclass_ptr->get_name() == character_json.at("subclass").get<std::string>());
    }
    REQUIRE(character_ptr->race_ptr->get_name() == character_json.at("race").get<std::string>());
    if (character_json.contains("subrace")) {
        REQUIRE(character_ptr->subrace_ptr->get_name() == character_json.at("subrace").get<std::string>());
    }
    REQUIRE(character_ptr->base_ability_scores == character_json.at("base_ability_scores").get<std::array<int, 6>>());
    if (character_json.contains("level")) {
        REQUIRE(character_ptr->get_level() == character_json.at("level").get<int>());
    }
    if (character_json.contains("xp")) {
        REQUIRE(character_ptr->get_xp() == character_json.at("xp").get<int>());
    }
    REQUIRE(character_ptr->get_hit_dice_rolls() == character_json.at("hit_dice_rolls").get<std::vector<int>>());
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
        REQUIRE_NOTHROW(parser.save_result());
        REQUIRE(setup.characters.size() == 1);
        character_ptr = &setup.characters.get(valid_low_level_bob.at("name"));
        test_basic_values_from_json(valid_low_level_bob, character_ptr);
        REQUIRE(
            character_ptr->get_xp()
            == dnd::Character::minxp_for_level.at(valid_low_level_bob.at("level").get<size_t>() - 1)
        );

        valid_high_level_bob.erase("xp");
        parser.setJSON(valid_high_level_bob);
        REQUIRE_NOTHROW(parser.parse());
        REQUIRE(parser.validate());
        REQUIRE_NOTHROW(parser.save_result());
        REQUIRE(setup.characters.size() == 2);
        character_ptr = &setup.characters.get(valid_high_level_bob.at("name"));
        test_basic_values_from_json(valid_high_level_bob, character_ptr);
        REQUIRE(
            character_ptr->get_xp()
            == dnd::Character::minxp_for_level.at(valid_high_level_bob.at("level").get<size_t>() - 1)
        );
    }
    SECTION("characters with xp only") {
        valid_low_level_bob.erase("level");
        parser.setJSON(valid_low_level_bob);
        REQUIRE_NOTHROW(parser.parse());
        REQUIRE(parser.validate());
        REQUIRE_NOTHROW(parser.save_result());
        REQUIRE(setup.characters.size() == 1);
        character_ptr = &setup.characters.get(valid_low_level_bob.at("name"));
        test_basic_values_from_json(valid_low_level_bob, character_ptr);
        REQUIRE(character_ptr->get_level() == dnd::Character::level_for_xp(valid_low_level_bob.at("xp").get<int>()));

        valid_high_level_bob.erase("level");
        parser.setJSON(valid_high_level_bob);
        REQUIRE_NOTHROW(parser.parse());
        REQUIRE(parser.validate());
        REQUIRE_NOTHROW(parser.save_result());
        REQUIRE(setup.characters.size() == 2);
        character_ptr = &setup.characters.get(valid_high_level_bob.at("name"));
        test_basic_values_from_json(valid_high_level_bob, character_ptr);
        REQUIRE(character_ptr->get_level() == dnd::Character::level_for_xp(valid_high_level_bob.at("xp").get<int>()));
    }
    SECTION("characters with level and xp") {
        parser.setJSON(valid_low_level_bob);
        REQUIRE_NOTHROW(parser.parse());
        REQUIRE(parser.validate());
        REQUIRE_NOTHROW(parser.save_result());
        REQUIRE(setup.characters.size() == 1);
        character_ptr = &setup.characters.get(valid_low_level_bob.at("name"));
        test_basic_values_from_json(valid_low_level_bob, character_ptr);

        parser.setJSON(valid_high_level_bob);
        REQUIRE_NOTHROW(parser.parse());
        REQUIRE(parser.validate());
        REQUIRE_NOTHROW(parser.save_result());
        REQUIRE(setup.characters.size() == 2);
        character_ptr = &setup.characters.get(valid_high_level_bob.at("name"));
        test_basic_values_from_json(valid_high_level_bob, character_ptr);
    }
}

// TODO: add test case: parse characters with spellcasting
// TODO: add test case: parse characters with custom background
// TODO: add test case: parse characters with their own custom spells
// TODO: add test case: parse characters with their own custom features
// TODO: add test case: parse characters with equipment
// TODO: should the result of CharacterState calculation be tested here?
