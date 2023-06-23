#include <core/errors/parsing_error.hpp>

#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <string>

static constexpr const char* tags = "[core/errors/parsing_error]";

TEST_CASE("dnd::ParsingError: basic getters", tags) {
    std::filesystem::path filepath("/path/to/file.txt");
    std::string error_message = "An error occurred";

    SECTION("Get error code") {
        dnd::ParsingError error1(dnd::ParsingErrorCode::FILE_NOT_FOUND, filepath, error_message);
        REQUIRE(error1.get_error_code() == dnd::ParsingErrorCode::FILE_NOT_FOUND);

        dnd::ParsingError error2(dnd::ParsingErrorCode::INVALID_FILE_FORMAT, filepath, error_message);
        REQUIRE(error2.get_error_code() == dnd::ParsingErrorCode::INVALID_FILE_FORMAT);
    }

    SECTION("Get file path") {
        dnd::ParsingError error1(dnd::ParsingErrorCode::MISSING_ATTRIBUTE, filepath, error_message);
        REQUIRE(error1.get_filepath() == filepath);

        std::filesystem::path other_filepath("/another/path/file.txt");
        dnd::ParsingError error2(dnd::ParsingErrorCode::INVALID_ATTRIBUTE_TYPE, other_filepath, error_message);
        REQUIRE(error2.get_filepath() == other_filepath);
    }

    SECTION("Get error message") {
        dnd::ParsingError error1(dnd::ParsingErrorCode::UNEXPECTED_ATTRIBUTE, filepath, error_message);
        REQUIRE(error1.get_error_message() == error_message);

        std::string other_message = "Another error occurred";
        dnd::ParsingError error2(dnd::ParsingErrorCode::UNKNOWN_ERROR, filepath, other_message);
        REQUIRE(error2.get_error_message() == other_message);
    }
}
