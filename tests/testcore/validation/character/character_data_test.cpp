#include <dnd_config.hpp>

#include <core/validation/character/character_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][character]";

static CharacterData create_valid_character_data() {
    CharacterData character_data;
    set_valid_mock_values(character_data, "Valid Character");
    FeatureData& feature_data = character_data.features_data.emplace_back();
    set_valid_mock_values(feature_data, "Valid Character Feature");
    character_data.base_ability_scores_data.ability_scores = {10, 8, 12, 15, 13, 14};
    character_data.feature_providers_data.species_name = "Human";
    character_data.feature_providers_data.subspecies_name = "";
    character_data.feature_providers_data.class_name = "Rogue";
    character_data.feature_providers_data.subclass_name = "Assassin";
    character_data.progression_data.level = 5;
    character_data.progression_data.xp = 7100;
    character_data.progression_data.hit_dice_rolls = {8, 2, 5, 7, 3};
    return character_data;
}

TEST_CASE("Validate Character", tags) {
    CharacterData data = create_valid_character_data();
    Content content = minimal_testing_content();
    Errors errors;

    REQUIRE_NOTHROW(errors = validate_character_nonrecursively_for_content(data, content));
    REQUIRE(errors.ok());

    SECTION("character without features is valid") {
        data.features_data.clear();
        REQUIRE_NOTHROW(errors = validate_character_nonrecursively_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("character with duplicate features is invalid") {
        FeatureData& feature_data1 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data1, "Duplicate Feature");
        FeatureData& feature_data2 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data2, "Duplicate Feature");
        REQUIRE_NOTHROW(errors = validate_character_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character ability scores must be between 1 and 30 (inclusive)") {
        data.base_ability_scores_data.ability_scores = {-1, 0, 1, 30, 31, 32};
        REQUIRE_NOTHROW(errors = validate_ability_scores(data.base_ability_scores_data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character species must exist") {
        data.feature_providers_data.species_name = "Nonexistent";
        REQUIRE_NOTHROW(errors = validate_feature_providers_for_content(data.feature_providers_data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character cannot have subspecies if species does not have subspecies") {
        data.feature_providers_data.species_name = "Human";
        data.feature_providers_data.subspecies_name = "Hill Dwarf";
        REQUIRE_NOTHROW(errors = validate_feature_providers_for_content(data.feature_providers_data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character subspecies must exist") {
        data.feature_providers_data.subspecies_name = "Nonexistent";
        REQUIRE_NOTHROW(errors = validate_feature_providers_for_content(data.feature_providers_data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character subspecies must be a subspecies of the character's species") {
        data.feature_providers_data.species_name = "Dwarf";
        data.feature_providers_data.subspecies_name = "High Elf";
        REQUIRE_NOTHROW(errors = validate_feature_providers_for_content(data.feature_providers_data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character class must exist") {
        data.feature_providers_data.class_name = "Nonexistent";
        REQUIRE_NOTHROW(errors = validate_feature_providers_for_content(data.feature_providers_data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character subclass must exist") {
        data.feature_providers_data.subclass_name = "Nonexistent";
        REQUIRE_NOTHROW(errors = validate_feature_providers_for_content(data.feature_providers_data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character subclass must be a subclass of the character's class") {
        data.feature_providers_data.class_name = "Wizard";
        data.feature_providers_data.subclass_name = "Assassin"; // subclass of Rogue
        REQUIRE_NOTHROW(errors = validate_feature_providers_for_content(data.feature_providers_data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character level must be between 1 and 20 (inclusive)") {
        data.progression_data.level = 0;
        REQUIRE_NOTHROW(errors = validate_progression(data.progression_data));
        REQUIRE_FALSE(errors.ok());

        data.progression_data.level = 21;
        REQUIRE_NOTHROW(errors = validate_progression(data.progression_data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character xp must be positive") {
        data.progression_data.xp = -1;
        REQUIRE_NOTHROW(errors = validate_progression(data.progression_data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("character level and xp must correspond to each other") {
        data.progression_data.level = 10;
        data.progression_data.xp = 1000;
        REQUIRE_NOTHROW(errors = validate_progression(data.progression_data));
        REQUIRE_FALSE(errors.ok());
    }

    // TODO: test decisions
}

} // namespace dnd::test
