#include <dnd_config.hpp>

#include <core/validation/character_subrace/character_subrace_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

static constexpr const char* tags = "[core][validation][character_subrace]";

TEST_CASE("dnd::CharacterSubraceData::validate and ::validate_relations // valid subrace data", tags) {
    dndtest::ValidationDataMock parent;
    dnd::CharacterSubraceData data;
    dndtest::set_valid_mock_values(data, "Subrace");
    dnd::Content content = dndtest::minimal_testing_content();
    dnd::Errors errors;

    SECTION("race with one valid feature") {
        data.race_name = "Dwarf";
        auto& feature_data = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data, "Feature");
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("race with multiple differently named features") {
        data.race_name = "Dwarf";
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

TEST_CASE("dnd::CharacterSubraceData::validate // invalid subrace data", tags) {
    dnd::CharacterSubraceData data;
    dndtest::set_valid_mock_values(data, "Subrace");
    dnd::Errors errors;

    SECTION("subrace without features is invalid") {
        data.race_name = "Dwarf";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("subrace without a race is invalid") {
        data.race_name = "";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("subrace with duplicate feature names") {
        data.race_name = "Dwarf";
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

TEST_CASE("dnd::CharacterSubraceData::validate_relations // invalid subrace data relations", tags) {
    dnd::CharacterSubraceData data;
    dndtest::set_valid_mock_values(data, "Subrace");
    auto& valid_feature_data = data.features_data.emplace_back(&data);
    dndtest::set_valid_mock_values(valid_feature_data, "Valid Feature");
    dnd::Content content = dndtest::minimal_testing_content();
    dnd::Errors errors;

    SECTION("subrace with a name that already exists in the content") {
        data.name = "Hill Dwarf"; // already exists in the example content
        data.race_name = "Dwarf";
        REQUIRE_NOTHROW(errors = data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("features with duplicate names aren't allowed") {
        data.name = "New Subrace";
        data.race_name = "Dwarf";
        auto& feature_data = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data, "Duplicate Feature");
        feature_data.name = "Example Subrace Feature"; // feature with that name already exists in the example content
        REQUIRE_NOTHROW(errors = data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("a race with the given race name must exist") {
        data.race_name = "Non-existent race";
        REQUIRE_NOTHROW(errors = data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("the race with the given race name must allow subraces") {
        data.race_name = "Human"; // doesn't allow subraces
        REQUIRE_NOTHROW(errors = data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }
}
