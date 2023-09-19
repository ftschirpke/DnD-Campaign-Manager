#include <dnd_config.hpp>

#include <core/validation/character_class/character_class_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <core/validation/feature/feature_data.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

static constexpr const char* tags = "[core][validation][character_class]";

TEST_CASE("dnd::CharacterClassData::validate and ::validate_relations // valid class data", tags) {
    dnd::CharacterClassData data;
    dndtest::set_valid_mock_values(data, "Class");
    data.hit_dice_data.str = "d6";
    data.important_levels_data.feat_levels = {4, 8, 12, 16, 19};
    dnd::Content content = dndtest::minimal_testing_content();
    dnd::Errors errors;

    SECTION("class with one valid feature") {
        dnd::FeatureData& feature_data = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data, "Only Feature");
        data.subclass_feature_name = "Only Feature";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("class with multiple differently named features") {
        dnd::FeatureData& feature_data1 = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data1, "Feature 1");
        dnd::FeatureData& feature_data2 = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data2, "Feature 2");
        dnd::FeatureData& feature_data3 = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data3, "Feature 3");
        data.subclass_feature_name = "Feature 1";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE(errors.ok());
    }
}

TEST_CASE("dnd::CharacterClassData::validate // invalid class data", tags) {
    dnd::CharacterClassData data;
    dndtest::set_valid_mock_values(data, "Class");
    data.hit_dice_data.str = "d6";
    data.important_levels_data.feat_levels = {4, 8, 12, 16, 19};
    dnd::Errors errors;

    SECTION("class without features is invalid") {
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("class without its subclass feature is invalid") {
        dnd::FeatureData& feature_data1 = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data1, "Feature 1");
        dnd::FeatureData& feature_data2 = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data2, "Feature 2");
        data.subclass_feature_name = "Feature 3";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("class with duplicate features is invalid") {
        dnd::FeatureData& feature_data1 = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data1, "Duplicate Feature");
        dnd::FeatureData& feature_data2 = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data2, "Duplicate Feature");
        dnd::FeatureData& feature_data3 = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data3, "Other Feature");
        data.subclass_feature_name = "Duplicate Feature";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("dnd::CharacterClassData::validate_relations // invalid class data relations", tags) {
    dnd::CharacterClassData data;
    dndtest::set_valid_mock_values(data, "Class");
    data.hit_dice_data.str = "d6";
    data.important_levels_data.feat_levels = {4, 8, 12, 16, 19};
    dnd::Content content = dndtest::minimal_testing_content();
    dnd::Errors errors;

    SECTION("class with a name that already exists in the content") {
        data.name = "Wizard"; // already exists in the example content
        REQUIRE_NOTHROW(errors = data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("features with duplicate names aren't allowed") {
        data.name = "New Class";
        auto& feature_data = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data, "Duplicate Feature");
        feature_data.name = "Example Class Feature"; // feature with that name already exists in the example content
        REQUIRE_NOTHROW(errors = data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }
}
