#include <dnd_config.hpp>

#include <core/validation/character/character_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

static constexpr const char* tags = "[core][validation][character]";

static dnd::CharacterData create_valid_character_data() {
    dnd::CharacterData character_data;
    dndtest::set_valid_mock_values(character_data, "Valid Character");
    auto& feature_data = character_data.features_data.emplace_back(&character_data);
    dndtest::set_valid_mock_values(feature_data, "Valid Character Feature");
    character_data.base_ability_scores_data.ability_scores = {10, 8, 12, 15, 13, 14};
    character_data.character_basis_data.race_name = "Human";
    character_data.character_basis_data.subrace_name = "";
    character_data.character_basis_data.class_name = "Rogue";
    character_data.character_basis_data.subclass_name = "Assassin";
    character_data.progression_data.level = 5;
    character_data.progression_data.xp = 7100;
    character_data.progression_data.hit_dice_rolls = {8, 2, 5, 7, 3};
    return character_data;
}

TEST_CASE("dnd::CharacterData::validate and ::validate_relations", tags) {
    dnd::CharacterData data = create_valid_character_data();
    dnd::Content content = dndtest::minimal_testing_content();
    dnd::Errors errors;

    REQUIRE_NOTHROW(errors = data.validate());
    REQUIRE_NOTHROW(errors += data.validate_relations(content));
    REQUIRE(errors.ok());

    SECTION("character without features is valid") {
        data.features_data.clear();
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("character with duplicate features is invalid") {
        dnd::FeatureData& feature_data1 = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data1, "Duplicate Feature");
        dnd::FeatureData& feature_data2 = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data2, "Duplicate Feature");
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character ability scores must be between 1 and 30 (inclusive)") {
        data.base_ability_scores_data.ability_scores = {-1, 0, 1, 30, 31, 32};
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character race must exist") {
        data.character_basis_data.race_name = "Nonexistent";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character cannot have subrace if race does not have subraces") {
        data.character_basis_data.race_name = "Human";
        data.character_basis_data.subrace_name = "Hill Dwarf";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character subrace must exist") {
        data.character_basis_data.subrace_name = "Nonexistent";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character subrace must be a subrace of the character's race") {
        data.character_basis_data.race_name = "Dwarf";
        data.character_basis_data.subrace_name = "High Elf";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character class must exist") {
        data.character_basis_data.class_name = "Nonexistent";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character subclass must exist") {
        data.character_basis_data.subclass_name = "Nonexistent";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character subclass must be a subclass of the character's class") {
        data.character_basis_data.class_name = "Wizard";
        data.character_basis_data.subclass_name = "Assassin"; // subclass of Rogue
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character level must be between 1 and 20 (inclusive)") {
        data.progression_data.level = 0;
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());

        data.progression_data.level = 21;
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character xp must be positive") {
        data.progression_data.xp = -1;
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character level and xp must correspond to each other") {
        data.progression_data.level = 10;
        data.progression_data.xp = 1000;
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    // TODO: test decisions
}
