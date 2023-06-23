#include <dnd_config.hpp>

#include <core/validation/validation_data.hpp>

#include <filesystem>
#include <string>

#include <catch2/catch_test_macros.hpp>

#include <core/validation/validation_data_mock.hpp>

static constexpr const char* tags = "[core][validation]";

TEST_CASE("dnd::ValidationData", tags) {
    dnd::ValidationDataMock validation_data;

    SECTION("Valid Data") {
        validation_data.name = "Name";
        validation_data.description = "Description";
        validation_data.source_path("/path/to/source");
        dnd::Errors errors = validation_data.validate();
        REQUIRE(errors.ok());
    }

    SECTION("Empty name") {
        validation_data.name = "";
        validation_data.description = "Description";
        validation_data.source_path("/path/to/source");
        dnd::Errors errors = validation_data.validate();
        REQUIRE_FALSE(errors.ok());
        REQUIRE_FALSE(errors.get_validation_errors().empty());
        REQUIRE(errors.get_parsing_errors().empty());
    }

    SECTION("Empty description") {
        validation_data.name = "Name";
        validation_data.description = "";
        validation_data.source_path("/path/to/source");
        dnd::Errors errors = validation_data.validate();
        REQUIRE_FALSE(errors.ok());
        REQUIRE_FALSE(errors.get_validation_errors().empty());
        REQUIRE(errors.get_parsing_errors().empty());
    }

    SECTION("Empty source path") {
        validation_data.name = "Name";
        validation_data.description = "Description";
        validation_data.source_path("");
        dnd::Errors errors = validation_data.validate();
        REQUIRE_FALSE(errors.ok());
        REQUIRE_FALSE(errors.get_validation_errors().empty());
        REQUIRE(errors.get_parsing_errors().empty());
    }

    SECTION("Completely empty") {
        validation_data.name = "";
        validation_data.description = "";
        validation_data.source_path("");
        dnd::Errors errors = validation_data.validate();
        REQUIRE_FALSE(errors.ok());
        REQUIRE_FALSE(errors.get_validation_errors().empty());
        REQUIRE(errors.get_parsing_errors().empty());
    }
}
