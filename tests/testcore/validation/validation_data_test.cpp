#include <dnd_config.hpp>

#include <core/validation/validation_data.hpp>

#include <filesystem>
#include <string>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <core/text/text.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation]";

TEST_CASE("validate_name_description_and_source", tags) {
    ValidationDataMock data;
    const std::filesystem::path dummy_path = std::filesystem::path(DND_MOCK_DIRECTORY) / "dummy_files" / "file1.json";

    Errors errors;
    SECTION("Valid Data") {
        data.name = "Name";
        data.description = Text::simple("Description");
        data.source_path = dummy_path;
        REQUIRE_NOTHROW(errors = validate_name_description_and_source(data));
        REQUIRE(errors.ok());
    }

    SECTION("Empty name") {
        data.name = "";
        data.description = Text::simple("Description");
        data.source_path = dummy_path;
        REQUIRE_NOTHROW(errors = validate_name_description_and_source(data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Empty description") {
        data.name = "Name";
        data.description = Text::simple("");
        data.source_path = dummy_path;
        REQUIRE_NOTHROW(errors = validate_name_description_and_source(data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Empty source path") {
        data.name = "Name";
        data.description = Text::simple("Description");
        data.source_path = std::filesystem::path("");
        REQUIRE_NOTHROW(errors = validate_name_description_and_source(data));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Completely empty") {
        data.name = "";
        data.description = Text::simple("");
        data.source_path = std::filesystem::path("");
        REQUIRE_NOTHROW(errors = validate_name_description_and_source(data));
        REQUIRE_FALSE(errors.ok());
    }
}

} // namespace dnd::test
