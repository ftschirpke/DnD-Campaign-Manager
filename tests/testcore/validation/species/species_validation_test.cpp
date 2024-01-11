#include <dnd_config.hpp>

#include <core/validation/species/species_validation.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][species]";

TEST_CASE("Validate Species // valid species data", tags) {
    Species::Data data;
    set_valid_mock_values(data, "Species");
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("species with one valid feature") {
        data.subspecies = false;
        Feature::Data& feature_data = data.features_data.emplace_back();
        set_valid_mock_values(feature_data, "Feature");
        REQUIRE_NOTHROW(errors = validate_species_nonrecursively_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("species with multiple differently named features") {
        data.subspecies = true;
        Feature::Data& feature_data1 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data1, "Feature 1");
        Feature::Data& feature_data2 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data2, "Feature 2");
        Feature::Data& feature_data3 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data3, "Feature 3");
        REQUIRE_NOTHROW(errors = validate_species_nonrecursively_for_content(data, content));
        REQUIRE(errors.ok());
    }
}

TEST_CASE("Validate Species // invalid species data", tags) {
    Species::Data data;
    set_valid_mock_values(data, "Species");
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("species without features is invalid") {
        data.subspecies = true;
        REQUIRE_NOTHROW(errors = validate_species_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("species with duplicate feature names") {
        data.subspecies = false;
        Feature::Data& feature_data1 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data1, "Duplicate Feature");
        Feature::Data& feature_data2 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data2, "Duplicate Feature");
        Feature::Data& feature_data3 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data3, "Other Feature");
        REQUIRE_NOTHROW(errors = validate_species_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("Validate Species // invalid species data relations", tags) {
    Species::Data data;
    set_valid_mock_values(data, "Species");
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("species with a name that already exists in the content") {
        data.name = "Dwarf"; // already exists in the example content
        REQUIRE_NOTHROW(errors = validate_species_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("features with duplicate names aren't allowed") {
        data.name = "New Species";
        Feature::Data& feature_data = data.features_data.emplace_back();
        set_valid_mock_values(feature_data, "Duplicate Feature");
        feature_data.name = "Example Species Feature"; // feature with that name already exists in the example content
        REQUIRE_NOTHROW(errors = validate_species_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }
}

} // namespace dnd::test
