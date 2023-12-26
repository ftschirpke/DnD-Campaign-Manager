#include <dnd_config.hpp>

#include <core/errors/validation_error.hpp>

#include <catch2/catch_test_macros.hpp>

#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][errors]";

TEST_CASE("ValidationError // basic getters", tags) {
    ValidationDataMock validation_data;
    validation_data.name = "First Mock Validation Data";
    validation_data.description = "some description";
    ValidationDataMock other_validation_data;
    other_validation_data.name = "Second Mock Validation Data";
    other_validation_data.description = "another description";
    const std::string error_message = "Error message";

    SECTION("Get error code") {
        ValidationError error1(ValidationErrorCode::MISSING_ATTRIBUTE, &validation_data, error_message);
        REQUIRE(error1.get_error_code() == ValidationErrorCode::MISSING_ATTRIBUTE);

        ValidationError error2(
            ValidationErrorCode::INVALID_ATTRIBUTE_FORMAT, &validation_data, error_message
        );
        REQUIRE(error2.get_error_code() == ValidationErrorCode::INVALID_ATTRIBUTE_FORMAT);
    }

    SECTION("Get validation data") {
        ValidationError error1(ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, &validation_data, error_message);
        REQUIRE(*dynamic_cast<const ValidationDataMock*>(error1.get_validation_data()) == validation_data);

        ValidationError error2(ValidationErrorCode::INVALID_RELATION, &other_validation_data, error_message);
        REQUIRE(
            *dynamic_cast<const ValidationDataMock*>(error2.get_validation_data()) == other_validation_data
        );
    }

    SECTION("Get error message") {
        ValidationError error1(ValidationErrorCode::INCONSISTENT_ATTRIBUTES, &validation_data, error_message);
        REQUIRE(error1.get_error_message() == error_message);

        std::string other_message = "Another error message";
        ValidationError error2(ValidationErrorCode::UNKNOWN_ERROR, &validation_data, other_message);
        REQUIRE(error2.get_error_message() == other_message);
    }
}

} // namespace dnd::test
