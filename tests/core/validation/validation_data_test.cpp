#include <dnd_config.hpp>

#include <core/validation/validation_data.hpp>

#include <filesystem>
#include <iostream>
#include <string>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data_mock.hpp>

static constexpr const char* tags = "[core][validation]";

TEST_CASE("dnd::ValidationData::validate", tags) {
    dndtest::ValidationDataMock data;
    const std::filesystem::path dummy_path = std::filesystem::path(DND_MOCK_DIRECTORY) / "dummy_files" / "file1.json";

    dnd::Errors errors;
    SECTION("Valid Data") {
        data.name = "Name";
        data.description = "Description";
        data.source_path = dummy_path;
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());
    }

    SECTION("Empty name") {
        data.name = "";
        data.description = "Description";
        data.source_path = dummy_path;
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Empty description") {
        data.name = "Name";
        data.description = "";
        data.source_path = dummy_path;
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Empty source path") {
        data.name = "Name";
        data.description = "Description";
        data.source_path = std::filesystem::path("");
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("Completely empty") {
        data.name = "";
        data.description = "";
        data.source_path = std::filesystem::path("");
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }
}
