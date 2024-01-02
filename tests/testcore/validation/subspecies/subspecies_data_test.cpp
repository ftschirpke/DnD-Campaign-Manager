#include <dnd_config.hpp>

#include <core/validation/subspecies/subspecies_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <core/validation/effects_provider/feature_data.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][subspecies]";

TEST_CASE("Validate Subspecies // valid subspecies data", tags) {
    Subspecies::Data data;
    set_valid_mock_values(data, "Subspecies");
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("species with one valid feature") {
        data.species_name = "Dwarf";
        Feature::Data& feature_data = data.features_data.emplace_back();
        set_valid_mock_values(feature_data, "Feature");
        REQUIRE_NOTHROW(errors = validate_subspecies_nonrecursively_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("species with multiple differently named features") {
        data.species_name = "Dwarf";
        Feature::Data& feature_data1 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data1, "Feature 1");
        Feature::Data& feature_data2 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data2, "Feature 2");
        Feature::Data& feature_data3 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data3, "Feature 3");
        REQUIRE_NOTHROW(errors = validate_subspecies_nonrecursively_for_content(data, content));
        REQUIRE(errors.ok());
    }
}

TEST_CASE("Validate Subspecies // invalid subspecies data", tags) {
    Subspecies::Data data;
    set_valid_mock_values(data, "Subspecies");
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("subspecies without species is invalid") {
        data.species_name = "";
        Feature::Data& feature_data = data.features_data.emplace_back();
        set_valid_mock_values(feature_data, "Feature");
        REQUIRE_NOTHROW(errors = validate_subspecies_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("subspecies without features is invalid") {
        data.species_name = "Dwarf";
        REQUIRE_NOTHROW(errors = validate_subspecies_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("subspecies without a species is invalid") {
        data.species_name = "";
        REQUIRE_NOTHROW(errors = validate_subspecies_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("subspecies with duplicate feature names") {
        data.species_name = "Dwarf";
        Feature::Data& feature_data1 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data1, "Duplicate Feature");
        Feature::Data& feature_data2 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data2, "Duplicate Feature");
        Feature::Data& feature_data3 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data3, "Other Feature");
        REQUIRE_NOTHROW(errors = validate_subspecies_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("Validate Subspecies // invalid subspecies data relations", tags) {
    Subspecies::Data data;
    set_valid_mock_values(data, "Subspecies");
    Feature::Data& valid_feature_data = data.features_data.emplace_back();
    set_valid_mock_values(valid_feature_data, "Valid Feature");
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("subspecies with a name that already exists in the content") {
        data.name = "Hill Dwarf"; // already exists in the example content
        data.species_name = "Dwarf";
        REQUIRE_NOTHROW(errors = validate_subspecies_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("features with duplicate names aren't allowed") {
        data.name = "New Subspecies";
        data.species_name = "Dwarf";
        Feature::Data& feature_data = data.features_data.emplace_back();
        set_valid_mock_values(feature_data, "Duplicate Feature");
        feature_data
            .name = "Example Subspecies Feature"; // feature with that name already exists in the example content
        REQUIRE_NOTHROW(errors = validate_subspecies_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("a species with the given species name must exist") {
        data.species_name = "Nonexistent species";
        REQUIRE_NOTHROW(errors = validate_subspecies_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("the species with the given species name must allow subspecies") {
        data.species_name = "Human"; // doesn't allow subspecies
        REQUIRE_NOTHROW(errors = validate_subspecies_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }
}

} // namespace dnd::test
