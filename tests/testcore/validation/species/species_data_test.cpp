#include <dnd_config.hpp>

#include <core/validation/species/species_data.hpp>

#include <utility>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <core/validation/effects_provider/feature_data.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][species]";

TEST_CASE("SpeciesData::validate and ::validate_relations // valid species data", tags) {
    SpeciesData data;
    set_valid_mock_values(data, "Species");
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("species with one valid feature") {
        data.subspecies = false;
        FeatureData& feature_data = data.features_data.emplace_back(&data);
        set_valid_mock_values(feature_data, "Feature");
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("species with multiple differently named features") {
        data.subspecies = true;
        FeatureData& feature_data1 = data.features_data.emplace_back(&data);
        set_valid_mock_values(feature_data1, "Feature 1");
        FeatureData& feature_data2 = data.features_data.emplace_back(&data);
        set_valid_mock_values(feature_data2, "Feature 2");
        FeatureData& feature_data3 = data.features_data.emplace_back(&data);
        set_valid_mock_values(feature_data3, "Feature 3");
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE(errors.ok());
    }
}

TEST_CASE("SpeciesData::validate // invalid species data", tags) {
    SpeciesData data;
    set_valid_mock_values(data, "Species");
    Errors errors;

    SECTION("species without features is invalid") {
        data.subspecies = true;
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("species with duplicate feature names") {
        data.subspecies = false;
        FeatureData& feature_data1 = data.features_data.emplace_back(&data);
        set_valid_mock_values(feature_data1, "Duplicate Feature");
        FeatureData& feature_data2 = data.features_data.emplace_back(&data);
        set_valid_mock_values(feature_data2, "Duplicate Feature");
        FeatureData& feature_data3 = data.features_data.emplace_back(&data);
        set_valid_mock_values(feature_data3, "Other Feature");
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("SpeciesData::validate_relations // invalid species data relations", tags) {
    SpeciesData data;
    set_valid_mock_values(data, "Species");
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("species with a name that already exists in the content") {
        data.name = "Dwarf"; // already exists in the example content
        REQUIRE_NOTHROW(errors = data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("features with duplicate names aren't allowed") {
        data.name = "New Species";
        FeatureData& feature_data = data.features_data.emplace_back(&data);
        set_valid_mock_values(feature_data, "Duplicate Feature");
        feature_data.name = "Example Species Feature"; // feature with that name already exists in the example content
        REQUIRE_NOTHROW(errors = data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }
}

} // namespace dnd::test
