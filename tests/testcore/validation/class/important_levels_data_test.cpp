#include <dnd_config.hpp>

#include <core/validation/class/important_levels_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][class]";

TEST_CASE("ImportantLevelsData::validate", tags) {
    ValidationDataMock parent;
    ImportantLevelsData data(&parent);
    Errors errors;

    SECTION("at least one feat level is required") {
        data.feat_levels = {};
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("feat levels must be between 1 and 20 (inclusive)") {
        data.feat_levels = {-1};
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.feat_levels = {0, 1, 6, 10};
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.feat_levels = {16, 24};
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.feat_levels = {4, 21, 19};
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("valid feat levels") {
        data.feat_levels = {1};
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.feat_levels = {1, 4, 8, 12, 16, 19};
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.feat_levels = {8, 4, 12, 16, 1, 19};
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.feat_levels = {13, 14, 12};
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());
    }
}

} // namespace dnd::test
