#include <dnd_config.hpp>

#include <core/errors/errors.hpp>

#include <filesystem>
#include <utility>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/parsing_error.hpp>
#include <core/errors/validation_error.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][errors]";

template <typename T>
static void check_error_code(const Error& error, typename T::Code expected_code) {
    REQUIRE(std::holds_alternative<T>(error));
    REQUIRE(std::get<T>(error).get_error_code() == expected_code);
}

TEST_CASE("Errors // Construction and Getter Methods", tags) {
    Errors errors;
    const ValidationDataMock validation_data;

    SECTION("Initial state") {
        REQUIRE(errors.ok());
        REQUIRE(errors.get_errors().empty());
    }

    SECTION("Add parsing error") {
        ParsingError parsing_error1(ParsingError::Code::FILE_NOT_FOUND, "/path/to/file.txt", "File not found");
        errors.add_parsing_error(std::move(parsing_error1));

        errors.add_parsing_error(ParsingError::Code::INVALID_FILE_FORMAT, "/path/to/file.txt", "Invalid file format");

        const std::vector<Error>& error_vec = errors.get_errors();
        REQUIRE(error_vec.size() == 2);
        check_error_code<ParsingError>(error_vec[0], ParsingError::Code::FILE_NOT_FOUND);
        check_error_code<ParsingError>(error_vec[1], ParsingError::Code::INVALID_FILE_FORMAT);
    }

    SECTION("Add validation error") {
        ValidationError validation_error1(ValidationError::Code::MISSING_ATTRIBUTE, "Missing attribute");
        errors.add_validation_error(std::move(validation_error1));

        errors.add_validation_error(ValidationError::Code::INVALID_ATTRIBUTE_FORMAT, "Invalid attribute format");

        const std::vector<Error>& error_vec = errors.get_errors();
        REQUIRE(error_vec.size() == 2);
        check_error_code<ValidationError>(error_vec[0], ValidationError::Code::MISSING_ATTRIBUTE);
        check_error_code<ValidationError>(error_vec[1], ValidationError::Code::INVALID_ATTRIBUTE_FORMAT);
    }

    SECTION("Merge errors") {
        Errors other_errors;

        ParsingError parsing_error(ParsingError::Code::FILE_NOT_FOUND, "/path/to/file.txt", "File not found");
        other_errors.add_parsing_error(std::move(parsing_error));

        other_errors.add_validation_error(ValidationError::Code::MISSING_ATTRIBUTE, "Missing attribute");

        errors.merge(std::move(other_errors));

        const std::vector<Error>& error_vec = errors.get_errors();
        int parsing_errors = 0;
        int validation_errors = 0;
        for (const Error& error : error_vec) {
            if (std::holds_alternative<ParsingError>(error)) {
                ++parsing_errors;
            } else if (std::holds_alternative<ValidationError>(error)) {
                ++validation_errors;
            }
        }
        REQUIRE(error_vec.size() == 2);
        REQUIRE(parsing_errors == 1);
        REQUIRE(validation_errors == 1);
        check_error_code<ParsingError>(error_vec[0], ParsingError::Code::FILE_NOT_FOUND);
        check_error_code<ValidationError>(error_vec[1], ValidationError::Code::MISSING_ATTRIBUTE);
    }

    SECTION("Operator +=") {
        Errors other_errors;

        other_errors.add_parsing_error(ParsingError::Code::FILE_NOT_FOUND, "/path/to/file.txt", "File not found");

        ValidationError validation_error(ValidationError::Code::MISSING_ATTRIBUTE, "Missing attribute");
        other_errors.add_validation_error(std::move(validation_error));

        errors += std::move(other_errors);

        const std::vector<Error>& error_vec = errors.get_errors();
        int parsing_errors = 0;
        int validation_errors = 0;
        for (const Error& error : error_vec) {
            if (std::holds_alternative<ParsingError>(error)) {
                ++parsing_errors;
            } else if (std::holds_alternative<ValidationError>(error)) {
                ++validation_errors;
            }
        }
        REQUIRE(parsing_errors == 1);
        REQUIRE(validation_errors == 1);
        REQUIRE(error_vec.size() == 2);
        check_error_code<ParsingError>(error_vec[0], ParsingError::Code::FILE_NOT_FOUND);
        check_error_code<ValidationError>(error_vec[1], ValidationError::Code::MISSING_ATTRIBUTE);
    }
}

TEST_CASE("Errors::ok", tags) {
    Errors errors;
    const ValidationDataMock validation_data;

    SECTION("No errors") { REQUIRE(errors.ok()); }

    SECTION("Parsing errors") {
        ParsingError parsing_error(ParsingError::Code::FILE_NOT_FOUND, "/path/to/file.txt", "File not found");
        errors.add_parsing_error(std::move(parsing_error));

        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Validation errors") {
        ValidationError validation_error(ValidationError::Code::MISSING_ATTRIBUTE, "Missing attribute");
        errors.add_validation_error(std::move(validation_error));

        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Parsing and validation errors") {
        ParsingError parsing_error(ParsingError::Code::FILE_NOT_FOUND, "/path/to/file.txt", "File not found");
        errors.add_parsing_error(std::move(parsing_error));

        ValidationError validation_error(ValidationError::Code::MISSING_ATTRIBUTE, "Missing attribute");
        errors.add_validation_error(std::move(validation_error));

        REQUIRE_FALSE(errors.ok());
    }
}

} // namespace dnd::test
