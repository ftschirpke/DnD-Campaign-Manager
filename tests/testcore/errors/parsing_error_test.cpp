#include <dnd_config.hpp>

#include <core/errors/parsing_error.hpp>

#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <string>

namespace dnd::test {

static constexpr const char* tags = "[core][parsing][errors]";

TEST_CASE("ParsingError // basic getters", tags) {
    std::filesystem::path filepath("/path/to/file.txt");
    std::string error_message = "An error occurred";

    SECTION("Get error code") {
        ParsingError error1(ParsingErrorCode::FILE_NOT_FOUND, filepath, error_message);
        REQUIRE(error1.get_error_code() == ParsingErrorCode::FILE_NOT_FOUND);

        ParsingError error2(ParsingErrorCode::INVALID_FILE_FORMAT, filepath, error_message);
        REQUIRE(error2.get_error_code() == ParsingErrorCode::INVALID_FILE_FORMAT);
    }

    SECTION("Get file path") {
        ParsingError error1(ParsingErrorCode::MISSING_ATTRIBUTE, filepath, error_message);
        REQUIRE(error1.get_filepath() == filepath);

        std::filesystem::path other_filepath("/another/path/file.txt");
        ParsingError error2(ParsingErrorCode::INVALID_ATTRIBUTE_TYPE, other_filepath, error_message);
        REQUIRE(error2.get_filepath() == other_filepath);
    }

    SECTION("Get error message") {
        ParsingError error1(ParsingErrorCode::UNEXPECTED_ATTRIBUTE, filepath, error_message);
        REQUIRE(error1.get_error_message() == error_message);

        std::string other_message = "Another error occurred";
        ParsingError error2(ParsingErrorCode::UNKNOWN_ERROR, filepath, other_message);
        REQUIRE(error2.get_error_message() == other_message);
    }
}

} // namespace dnd::test
