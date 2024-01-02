#include <dnd_config.hpp>

#include <core/validation/effects_provider/choosable_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][effects_provider]";

TEST_CASE("Validate Choosable", tags) {
    Choosable::Data data;
    set_valid_mock_values(data, "Choosable Feature");
    Errors errors;

    SECTION("a choosable type is required") {
        REQUIRE_NOTHROW(errors = validate_choosable_nonrecursively(data));
        REQUIRE_FALSE(errors.ok());

        data.type = "eldritch invocation";
        REQUIRE_NOTHROW(errors = validate_choosable_nonrecursively(data));
        REQUIRE(errors.ok());
    }
}

} // namespace dnd::test
