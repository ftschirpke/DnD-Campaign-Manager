#include <dnd_config.hpp>

#include <core/validation/subclass/subclass_validation.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][subclass]";

TEST_CASE("Validate Subclass // valid subclass", tags) {
    Subclass::Data data;
    set_valid_mock_values(data, "Subclass");
    data.spellcasting_data.is_spellcaster = false;
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("class with one valid feature") {
        data.class_key = "Wizard|dummy";
        ClassFeature::Data& feature_data = data.features_data.emplace_back();
        set_valid_mock_values(feature_data, "Feature");
        REQUIRE_NOTHROW(errors = validate_subclass_nonrecursively_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("class with multiple differently named features") {
        data.class_key = "Wizard|dummy";
        Feature::Data& feature_data1 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data1, "Feature 1");
        Feature::Data& feature_data2 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data2, "Feature 2");
        Feature::Data& feature_data3 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data3, "Feature 3");
        REQUIRE_NOTHROW(errors = validate_subclass_nonrecursively_for_content(data, content));
        REQUIRE(errors.ok());
    }
}

TEST_CASE("Validate Subclass // invalid subclass", tags) {
    Subclass::Data data;
    set_valid_mock_values(data, "Subclass");
    Content content = minimal_testing_content();
    data.spellcasting_data.is_spellcaster = false;
    Errors errors;

    SECTION("subclass without class is invalid") {
        data.class_key = "";
        ClassFeature::Data& feature_data = data.features_data.emplace_back();
        set_valid_mock_values(feature_data, "Feature");
        REQUIRE_NOTHROW(errors = validate_subclass_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("subclass without features is invalid") {
        data.class_key = "Wizard|dummy";
        REQUIRE_NOTHROW(errors = validate_subclass_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("subclass without a class is invalid") {
        data.class_key = "";
        REQUIRE_NOTHROW(errors = validate_subclass_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("Validate Subclass // invalid subclass data relations", tags) {
    Subclass::Data data;
    set_valid_mock_values(data, "Subclass");
    data.spellcasting_data.is_spellcaster = false;
    ClassFeature::Data& valid_feature_data = data.features_data.emplace_back();
    set_valid_mock_values(valid_feature_data, "Valid Feature");
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("subclass with a name that already exists in the content") {
        data.name = "Abjuration Wizard"; // already exists in the example content
        data.class_key = "Wizard|dummy";
        REQUIRE_NOTHROW(errors = validate_subclass_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("a class with the given class name must exist") {
        data.class_key = "Nonexistent Class|dummy";
        REQUIRE_NOTHROW(errors = validate_subclass_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("subclass cannot have spellcasting if class already has") {
        data.class_key = "Wizard|dummy";
        data.spellcasting_data.is_spellcaster = true;
        data.spellcasting_data.ability = "INT";
        data.spellcasting_data.is_spells_known_type = true;
        REQUIRE_NOTHROW(errors = validate_subclass_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }
}

} // namespace dnd::test
