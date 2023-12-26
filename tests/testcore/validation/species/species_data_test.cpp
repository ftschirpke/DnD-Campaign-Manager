#include <dnd_config.hpp>

#include <core/validation/character_species/character_species_data.hpp>

#include <utility>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <core/validation/effects_provider/feature_data.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

static constexpr const char* tags = "[core][validation][character_species]";

TEST_CASE("dnd::CharacterSpeciesData::validate and ::validate_relations // valid species data", tags) {
    dnd::CharacterSpeciesData data;
    dndtest::set_valid_mock_values(data, "Species");
    dnd::Content content = dndtest::minimal_testing_content();
    dnd::Errors errors;

    SECTION("species with one valid feature") {
        data.subspecies = false;
        dnd::FeatureData& feature_data = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data, "Feature");
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("species with multiple differently named features") {
        data.subspecies = true;
        dnd::FeatureData& feature_data1 = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data1, "Feature 1");
        dnd::FeatureData& feature_data2 = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data2, "Feature 2");
        dnd::FeatureData& feature_data3 = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data3, "Feature 3");
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE(errors.ok());
    }
}

TEST_CASE("dnd::CharacterSpeciesData::validate // invalid species data", tags) {
    dnd::CharacterSpeciesData data;
    dndtest::set_valid_mock_values(data, "Species");
    dnd::Errors errors;

    SECTION("species without features is invalid") {
        data.subspecies = true;
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("species with duplicate feature names") {
        data.subspecies = false;
        dnd::FeatureData& feature_data1 = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data1, "Duplicate Feature");
        dnd::FeatureData& feature_data2 = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data2, "Duplicate Feature");
        dnd::FeatureData& feature_data3 = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data3, "Other Feature");
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("dnd::CharacterSpeciesData::validate_relations // invalid species data relations", tags) {
    dnd::CharacterSpeciesData data;
    dndtest::set_valid_mock_values(data, "Species");
    dnd::Content content = dndtest::minimal_testing_content();
    dnd::Errors errors;

    SECTION("species with a name that already exists in the content") {
        data.name = "Dwarf"; // already exists in the example content
        REQUIRE_NOTHROW(errors = data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("features with duplicate names aren't allowed") {
        data.name = "New Species";
        dnd::FeatureData& feature_data = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data, "Duplicate Feature");
        feature_data.name = "Example Species Feature"; // feature with that name already exists in the example content
        REQUIRE_NOTHROW(errors = data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }
}
