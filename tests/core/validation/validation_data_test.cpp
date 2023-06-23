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
    dnd::ValidationDataMock validation_data;
    const std::filesystem::path dummy_path = std::filesystem::path(DND_MOCK_DIRECTORY) / "dummy_files" / "file1.json";

    SECTION("Valid Data") {
        validation_data.name = "Name";
        validation_data.description = "Description";
        validation_data.source_path = dummy_path;
        dnd::Errors errors = validation_data.validate();
        REQUIRE(errors.ok());
    }

    SECTION("Empty name") {
        validation_data.name = "";
        validation_data.description = "Description";
        validation_data.source_path = dummy_path;
        dnd::Errors errors = validation_data.validate();
        REQUIRE_FALSE(errors.ok());
        REQUIRE_FALSE(errors.get_validation_errors().empty());
        REQUIRE(errors.get_parsing_errors().empty());
    }

    SECTION("Empty description") {
        validation_data.name = "Name";
        validation_data.description = "";
        validation_data.source_path = dummy_path;
        dnd::Errors errors = validation_data.validate();
        REQUIRE_FALSE(errors.ok());
        REQUIRE_FALSE(errors.get_validation_errors().empty());
        REQUIRE(errors.get_parsing_errors().empty());
    }

    SECTION("Empty source path") {
        validation_data.name = "Name";
        validation_data.description = "Description";
        validation_data.source_path = std::filesystem::path("");
        dnd::Errors errors = validation_data.validate();
        REQUIRE_FALSE(errors.ok());
        REQUIRE_FALSE(errors.get_validation_errors().empty());
        REQUIRE(errors.get_parsing_errors().empty());
    }

    SECTION("Completely empty") {
        validation_data.name = "";
        validation_data.description = "";
        validation_data.source_path = std::filesystem::path("");
        dnd::Errors errors = validation_data.validate();
        REQUIRE_FALSE(errors.ok());
        REQUIRE_FALSE(errors.get_validation_errors().empty());
        REQUIRE(errors.get_parsing_errors().empty());
    }
}
