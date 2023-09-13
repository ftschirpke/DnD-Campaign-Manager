#include <dnd_config.hpp>

#include <core/validation/character_race/character_race_data.hpp>

#include <utility>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <core/validation/feature/feature_data.hpp>
#include <testcore/validation/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

static constexpr const char* tags = "[core][validation][character_race]";

TEST_CASE("dnd::CharacterRaceData::validate and ::validate_relations // invalid race data", tags) {
    dnd::CharacterRaceData data;
    dndtest::set_valid_mock_values(data, "Race");
    dnd::Content content = dndtest::minimal_testing_content();
    dnd::Errors errors;

    SECTION("race with one valid feature") {
        data.subraces = false;
        dnd::FeatureData& feature_data = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data, "Feature");
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE(errors.ok());
    }

    SECTION("race with multiple differently named features") {
        data.subraces = true;
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

TEST_CASE("dnd::CharacterRaceData::validate", tags) {
    dnd::CharacterRaceData data;
    dndtest::set_valid_mock_values(data, "Race");
    dnd::Errors errors;

    SECTION("race without features is invalid") {
        data.subraces = true;
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("race with duplicate feature names") {
        data.subraces = false;
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

TEST_CASE("dnd::CharacterRaceData::validate_relations", tags) {
    dnd::CharacterRaceData data;
    dndtest::set_valid_mock_values(data, "Race");
    dnd::Content content = dndtest::minimal_testing_content();
    dnd::Errors errors;

    SECTION("race with a name that already exists in the content") {
        data.name = "Dwarf"; // already exists in the example content
        REQUIRE_NOTHROW(errors = data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("features with duplicate names aren't allowed") {
        data.name = "New Race";
        auto& feature_data = data.features_data.emplace_back(&data);
        dndtest::set_valid_mock_values(feature_data, "Duplicate Feature");
        feature_data.name = "Example Race Feature"; // feature with that name already exists in the example content
        REQUIRE_NOTHROW(errors = data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());
    }
}
