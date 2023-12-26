#include <dnd_config.hpp>

#include <core/validation/subspecies/subspecies_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <core/validation/effects_provider/feature_data.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

static constexpr const char* tags = "[core][validation][subspecies]";

TEST_CASE("dnd::SubspeciesData::validate and ::validate_relations // valid subspecies data", tags) {
    dnd::SubspeciesData data;
    dndtest::set_valid_mock_values(data, "Subspecies");
    dnd::Content content = dndtest::minimal_testing_content();
    dnd::Errors errors;

    SECTION("species with one valid feature") {
        data.species_name = "Dwarf";
        dnd::FeatureData& feature_data = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data, "Feature");
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("species with multiple differently named features") {
        data.species_name = "Dwarf";
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

TEST_CASE("dnd::SubspeciesData::validate // invalid subspecies data", tags) {
    dnd::SubspeciesData data;
    dndtest::set_valid_mock_values(data, "Subspecies");
    dnd::Errors errors;

    SECTION("subspecies without species is invalid") {
        data.species_name = "";
        dnd::FeatureData& feature_data = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data, "Feature");
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("subspecies without features is invalid") {
        data.species_name = "Dwarf";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("subspecies without a species is invalid") {
        data.species_name = "";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("subspecies with duplicate feature names") {
        data.species_name = "Dwarf";
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

TEST_CASE("dnd::SubspeciesData::validate_relations // invalid subspecies data relations", tags) {
    dnd::SubspeciesData data;
    dndtest::set_valid_mock_values(data, "Subspecies");
    dnd::FeatureData& valid_feature_data = data.features_data.emplace_back(&data);
    dndtest::set_valid_mock_values(valid_feature_data, "Valid Feature");
    dnd::Content content = dndtest::minimal_testing_content();
    dnd::Errors errors;

    SECTION("subspecies with a name that already exists in the content") {
        data.name = "Hill Dwarf"; // already exists in the example content
        data.species_name = "Dwarf";
        REQUIRE_NOTHROW(errors = data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("features with duplicate names aren't allowed") {
        data.name = "New Subspecies";
        data.species_name = "Dwarf";
        dnd::FeatureData& feature_data = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data, "Duplicate Feature");
        feature_data.name = "Example Subspecies Feature"; // feature with that name already exists in the example content
        REQUIRE_NOTHROW(errors = data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("a species with the given species name must exist") {
        data.species_name = "Nonexistent species";
        REQUIRE_NOTHROW(errors = data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("the species with the given species name must allow subspecies") {
        data.species_name = "Human"; // doesn't allow subspecies
        REQUIRE_NOTHROW(errors = data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }
}
