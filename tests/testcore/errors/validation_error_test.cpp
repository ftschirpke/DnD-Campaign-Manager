#include <dnd_config.hpp>

#include <core/errors/validation_error.hpp>

#include <catch2/catch_test_macros.hpp>

#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][errors]";

TEST_CASE("ValidationError // basic getters", tags) {
    const std::string error_message = "Error message";

    SECTION("Get error code") {
        ValidationError error1(ValidationError::Code::MISSING_ATTRIBUTE, error_message);
        REQUIRE(error1.get_error_code() == ValidationError::Code::MISSING_ATTRIBUTE);

        ValidationError error2(ValidationError::Code::INVALID_ATTRIBUTE_FORMAT, error_message);
        REQUIRE(error2.get_error_code() == ValidationError::Code::INVALID_ATTRIBUTE_FORMAT);
    }

    SECTION("Get error message") {
        ValidationError error1(ValidationError::Code::INCONSISTENT_ATTRIBUTES, error_message);
        REQUIRE(error1.get_error_message() == error_message);

        std::string other_message = "Another error message";
        ValidationError error2(ValidationError::Code::UNKNOWN_ERROR, other_message);
        REQUIRE(error2.get_error_message() == other_message);
    }
}

} // namespace dnd::test
