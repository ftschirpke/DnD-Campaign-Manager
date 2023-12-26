#include <dnd_config.hpp>

#include <core/validation/effects_provider/choosable_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][effects_provider]";

TEST_CASE("ChoosableFeatureData::validate", tags) {
    ChoosableData data;
    set_valid_mock_values(data, "Choosable Feature");
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("without any effect holders, at least a type is needed") {
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());

        data.type = "eldritch invocation";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE(errors.ok());
    }
}

} // namespace dnd::test
