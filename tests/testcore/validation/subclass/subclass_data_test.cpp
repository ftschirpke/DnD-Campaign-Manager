#include <dnd_config.hpp>

#include <core/validation/subclass/subclass_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <core/validation/effects_provider/class_feature_data.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][subclass]";

TEST_CASE("Validate Subclass // valid subclass", tags) {
    SubclassData data;
    set_valid_mock_values(data, "Subclass");
    data.spellcasting_data.is_spellcaster = false;
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("class with one valid feature") {
        data.class_name = "Wizard";
        ClassFeatureData& feature_data = data.features_data.emplace_back();
        set_valid_mock_values(feature_data, "Feature");
        REQUIRE_NOTHROW(errors = validate_subclass_nonrecursively_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("class with multiple differently named features") {
        data.class_name = "Wizard";
        FeatureData& feature_data1 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data1, "Feature 1");
        FeatureData& feature_data2 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data2, "Feature 2");
        FeatureData& feature_data3 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data3, "Feature 3");
        REQUIRE_NOTHROW(errors = validate_subclass_nonrecursively_for_content(data, content));
        REQUIRE(errors.ok());
    }
}

TEST_CASE("Validate Subclass // invalid subclass", tags) {
    SubclassData data;
    set_valid_mock_values(data, "Subclass");
    Content content = minimal_testing_content();
    data.spellcasting_data.is_spellcaster = false;
    Errors errors;

    SECTION("subclass without class is invalid") {
        data.class_name = "";
        ClassFeatureData& feature_data = data.features_data.emplace_back();
        set_valid_mock_values(feature_data, "Feature");
        REQUIRE_NOTHROW(errors = validate_subclass_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("subclass without features is invalid") {
        data.class_name = "Wizard";
        REQUIRE_NOTHROW(errors = validate_subclass_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("subclass without a class is invalid") {
        data.class_name = "";
        REQUIRE_NOTHROW(errors = validate_subclass_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("subclass with duplicate feature names") {
        data.class_name = "Wizard";
        FeatureData& feature_data1 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data1, "Duplicate Feature");
        FeatureData& feature_data2 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data2, "Duplicate Feature");
        FeatureData& feature_data3 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data3, "Other Feature");
        REQUIRE_NOTHROW(errors = validate_subclass_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("Validate Subclass // invalid subclass data relations", tags) {
    SubclassData data;
    set_valid_mock_values(data, "Subclass");
    data.spellcasting_data.is_spellcaster = false;
    ClassFeatureData& valid_feature_data = data.features_data.emplace_back();
    set_valid_mock_values(valid_feature_data, "Valid Feature");
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("subclass with a name that already exists in the content") {
        data.name = "Abjuration Wizard"; // already exists in the example content
        data.class_name = "Wizard";
        REQUIRE_NOTHROW(errors = validate_subclass_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("features with duplicate names aren't allowed") {
        data.name = "New Subclass";
        data.class_name = "Wizard";
        ClassFeatureData& feature_data = data.features_data.emplace_back();
        set_valid_mock_values(feature_data, "Duplicate Feature");
        feature_data.name = "Example Subclass Feature"; // feature with that name already exists in the example content
        REQUIRE_NOTHROW(errors = validate_subclass_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("a class with the given class name must exist") {
        data.class_name = "Nonexistent Class";
        REQUIRE_NOTHROW(errors = validate_subclass_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("subclass cannot have spellcasting if class already has") {
        data.class_name = "Wizard";
        data.spellcasting_data.is_spellcaster = true;
        data.spellcasting_data.ability = "INT";
        data.spellcasting_data.is_spells_known_type = true;
        REQUIRE_NOTHROW(errors = validate_subclass_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }
}

} // namespace dnd::test
