#include <dnd_config.hpp>

#include <core/validation/class/class_validation.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][class]";

TEST_CASE("Validate Class // valid class data", tags) {
    Class::Data data;
    set_valid_mock_values(data, "Class");
    data.hit_dice_str = "d6";
    data.important_levels_data.feat_levels = {4, 8, 12, 16, 19};
    data.spellcasting_data.is_spellcaster = false;
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("class with one valid feature") {
        Feature::Data& feature_data = data.features_data.emplace_back();
        set_valid_mock_values(feature_data, "Only Feature");
        data.subclass_feature_name = "Only Feature";
        REQUIRE_NOTHROW(errors = validate_class_nonrecursively_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("class with multiple differently named features") {
        Feature::Data& feature_data1 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data1, "Feature 1");
        Feature::Data& feature_data2 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data2, "Feature 2");
        Feature::Data& feature_data3 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data3, "Feature 3");
        data.subclass_feature_name = "Feature 1";
        REQUIRE_NOTHROW(errors = validate_class_nonrecursively_for_content(data, content));
        REQUIRE(errors.ok());
    }
}

TEST_CASE("Validate Class // invalid class data", tags) {
    Class::Data data;
    set_valid_mock_values(data, "Class");
    data.hit_dice_str = "d6";
    data.important_levels_data.feat_levels = {4, 8, 12, 16, 19};
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("class without features is invalid") {
        REQUIRE_NOTHROW(errors = validate_class_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("class without its subclass feature is invalid") {
        Feature::Data& feature_data1 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data1, "Feature 1");
        Feature::Data& feature_data2 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data2, "Feature 2");
        data.subclass_feature_name = "Feature 3";
        REQUIRE_NOTHROW(errors = validate_class_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("class with duplicate features is invalid") {
        Feature::Data& feature_data1 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data1, "Duplicate Feature");
        Feature::Data& feature_data2 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data2, "Duplicate Feature");
        Feature::Data& feature_data3 = data.features_data.emplace_back();
        set_valid_mock_values(feature_data3, "Other Feature");
        data.subclass_feature_name = "Duplicate Feature";
        REQUIRE_NOTHROW(errors = validate_class_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("Validate Class // invalid class data relations", tags) {
    Class::Data data;
    set_valid_mock_values(data, "Class");
    data.hit_dice_str = "d6";
    data.important_levels_data.feat_levels = {4, 8, 12, 16, 19};
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("class with a name that already exists in the content") {
        data.name = "Wizard"; // already exists in the example content
        REQUIRE_NOTHROW(errors = validate_class_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("features with duplicate names aren't allowed") {
        data.name = "New Class";
        ClassFeature::Data& feature_data = data.features_data.emplace_back();
        set_valid_mock_values(feature_data, "Duplicate Feature");
        feature_data.name = "Example Class Feature"; // feature with that name already exists in the example content
        REQUIRE_NOTHROW(errors = validate_class_nonrecursively_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }
}

} // namespace dnd::test
