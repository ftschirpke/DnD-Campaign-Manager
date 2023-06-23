#include <dnd_config.hpp>

#include <core/errors/validation_error.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/validation/validation_data_mock.hpp>

static constexpr const char* tags = "[core][validation][errors]";

TEST_CASE("dnd::ValidationError // basic getters", tags) {
    dnd::ValidationDataMock validation_data;
    validation_data.name = "First Mock Validation Data";
    validation_data.description = "some description";
    dnd::ValidationDataMock other_validation_data;
    other_validation_data.name = "Second Mock Validation Data";
    other_validation_data.description = "another description";
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
        REQUIRE(*dynamic_cast<const dnd::ValidationDataMock*>(error1.get_validation_data()) == validation_data);

        dnd::ValidationError error2(dnd::ValidationErrorCode::INVALID_RELATION, &other_validation_data, error_message);
        REQUIRE(*dynamic_cast<const dnd::ValidationDataMock*>(error2.get_validation_data()) == other_validation_data);
    }

    SECTION("Get error message") {
        dnd::ValidationError error1(dnd::ValidationErrorCode::INCONSISTENT_ATTRIBUTES, &validation_data, error_message);
        REQUIRE(error1.get_error_message() == error_message);

        std::string other_message = "Another error message";
        dnd::ValidationError error2(dnd::ValidationErrorCode::UNKNOWN_ERROR, &validation_data, other_message);
        REQUIRE(error2.get_error_message() == other_message);
    }
}
