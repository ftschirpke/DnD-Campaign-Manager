#include <core/errors/validation_error.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/validation/character/character_data.hpp>
#include <core/validation/feature/choosable_feature_data.hpp>

static constexpr const char* tags = "[core][validation][errors]";

TEST_CASE("dnd::ValidationError: basic getters", tags) {
    const dnd::CharacterData validation_data;
    const dnd::ChoosableFeatureData other_validation_data;
    const std::string error_message = "Error message";

    SECTION("Get error code") {
        dnd::ValidationError error1(dnd::ValidationErrorCode::MISSING_ATTRIBUTE, &validation_data, error_message);
        REQUIRE(error1.get_error_code() == dnd::ValidationErrorCode::MISSING_ATTRIBUTE);

        dnd::ValidationError error2(
            dnd::ValidationErrorCode::INVALID_ATTRIBUTE_FORMAT, &validation_data, error_message
        );
        REQUIRE(error2.get_error_code() == dnd::ValidationErrorCode::INVALID_ATTRIBUTE_FORMAT);
    }

    SECTION("Get validation data") {
        dnd::ValidationError error1(dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, &validation_data, error_message);
        REQUIRE(*dynamic_cast<const dnd::CharacterData*>(error1.get_validation_data()) == validation_data);

        dnd::ValidationError error2(dnd::ValidationErrorCode::INVALID_RELATION, &other_validation_data, error_message);
        REQUIRE(*dynamic_cast<const dnd::ChoosableFeatureData*>(error2.get_validation_data()) == other_validation_data);
    }

    SECTION("Get error message") {
        dnd::ValidationError error1(dnd::ValidationErrorCode::INCONSISTENT_ATTRIBUTES, &validation_data, error_message);
        REQUIRE(error1.get_error_message() == error_message);

        std::string other_message = "Another error message";
        dnd::ValidationError error2(dnd::ValidationErrorCode::UNKNOWN_ERROR, &validation_data, other_message);
        REQUIRE(error2.get_error_message() == other_message);
    }
}
