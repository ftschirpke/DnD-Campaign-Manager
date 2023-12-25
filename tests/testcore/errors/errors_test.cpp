#include <dnd_config.hpp>

#include <core/errors/errors.hpp>

#include <filesystem>
#include <utility>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/parsing_error.hpp>
#include <core/errors/validation_error.hpp>
#include <testcore/validation/validation_data_mock.hpp>

static constexpr const char* tags = "[core][errors]";

TEST_CASE("dnd::Errors // Construction and Getter Methods", tags) {
    dnd::Errors errors;
    const dndtest::ValidationDataMock validation_data;

    SECTION("Initial state") {
        REQUIRE(errors.ok());
        REQUIRE(errors.get_parsing_errors().empty());
        REQUIRE(errors.get_validation_errors().empty());
    }

    SECTION("Add parsing error") {
        dnd::ParsingError parsing_error1(dnd::ParsingErrorCode::FILE_NOT_FOUND, "/path/to/file.txt", "File not found");
        errors.add_parsing_error(std::move(parsing_error1));

        errors.add_parsing_error(
            dnd::ParsingErrorCode::INVALID_FILE_FORMAT, "/path/to/file.txt", "Invalid file format"
        );

        const std::vector<dnd::ParsingError>& parsing_errors = errors.get_parsing_errors();
        REQUIRE(parsing_errors.size() == 2);
        REQUIRE(parsing_errors[0].get_error_code() == dnd::ParsingErrorCode::FILE_NOT_FOUND);
        REQUIRE(parsing_errors[1].get_error_code() == dnd::ParsingErrorCode::INVALID_FILE_FORMAT);
    }

    SECTION("Add validation error") {
        dnd::ValidationError validation_error1(
            dnd::ValidationErrorCode::MISSING_ATTRIBUTE, &validation_data, "Missing attribute"
        );
        errors.add_validation_error(std::move(validation_error1));

        errors.add_validation_error(
            dnd::ValidationErrorCode::INVALID_ATTRIBUTE_FORMAT, &validation_data, "Invalid attribute format"
        );

        const std::vector<dnd::ValidationError>& validation_errors = errors.get_validation_errors();
        REQUIRE(validation_errors.size() == 2);
        REQUIRE(validation_errors[0].get_error_code() == dnd::ValidationErrorCode::MISSING_ATTRIBUTE);
        REQUIRE(validation_errors[1].get_error_code() == dnd::ValidationErrorCode::INVALID_ATTRIBUTE_FORMAT);
    }

    SECTION("Merge errors") {
        dnd::Errors other_errors;

        dnd::ParsingError parsing_error(dnd::ParsingErrorCode::FILE_NOT_FOUND, "/path/to/file.txt", "File not found");
        other_errors.add_parsing_error(std::move(parsing_error));

        other_errors.add_validation_error(
            dnd::ValidationErrorCode::MISSING_ATTRIBUTE, &validation_data, "Missing attribute"
        );

        errors.merge(std::move(other_errors));

        const std::vector<dnd::ParsingError>& parsing_errors = errors.get_parsing_errors();
        const std::vector<dnd::ValidationError>& validation_errors = errors.get_validation_errors();
        REQUIRE(parsing_errors.size() == 1);
        REQUIRE(validation_errors.size() == 1);
        REQUIRE(parsing_errors[0].get_error_code() == dnd::ParsingErrorCode::FILE_NOT_FOUND);
        REQUIRE(validation_errors[0].get_error_code() == dnd::ValidationErrorCode::MISSING_ATTRIBUTE);
    }

    SECTION("Operator +=") {
        dnd::Errors other_errors;

        other_errors.add_parsing_error(dnd::ParsingErrorCode::FILE_NOT_FOUND, "/path/to/file.txt", "File not found");

        dnd::ValidationError validation_error(
            dnd::ValidationErrorCode::MISSING_ATTRIBUTE, &validation_data, "Missing attribute"
        );
        other_errors.add_validation_error(std::move(validation_error));

        errors += std::move(other_errors);

        const std::vector<dnd::ParsingError>& parsing_errors = errors.get_parsing_errors();
        const std::vector<dnd::ValidationError>& validation_errors = errors.get_validation_errors();
        REQUIRE(parsing_errors.size() == 1);
        REQUIRE(validation_errors.size() == 1);
        REQUIRE(parsing_errors[0].get_error_code() == dnd::ParsingErrorCode::FILE_NOT_FOUND);
        REQUIRE(validation_errors[0].get_error_code() == dnd::ValidationErrorCode::MISSING_ATTRIBUTE);
    }
}

TEST_CASE("dnd::Errors::ok", tags) {
    dnd::Errors errors;
    const dndtest::ValidationDataMock validation_data;

    SECTION("No errors") { REQUIRE(errors.ok()); }

    SECTION("Parsing errors") {
        dnd::ParsingError parsing_error(dnd::ParsingErrorCode::FILE_NOT_FOUND, "/path/to/file.txt", "File not found");
        errors.add_parsing_error(std::move(parsing_error));

        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Validation errors") {
        dnd::ValidationError validation_error(
            dnd::ValidationErrorCode::MISSING_ATTRIBUTE, &validation_data, "Missing attribute"
        );
        errors.add_validation_error(std::move(validation_error));

        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Parsing and validation errors") {
        dnd::ParsingError parsing_error(dnd::ParsingErrorCode::FILE_NOT_FOUND, "/path/to/file.txt", "File not found");
        errors.add_parsing_error(std::move(parsing_error));

        dnd::ValidationError validation_error(
            dnd::ValidationErrorCode::MISSING_ATTRIBUTE, &validation_data, "Missing attribute"
        );
        errors.add_validation_error(std::move(validation_error));

        REQUIRE_FALSE(errors.ok());
    }
}
