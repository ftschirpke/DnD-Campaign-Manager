#include <dnd_config.hpp>

#include <core/validation/feature/choosable_feature_data.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <testcore/validation/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

static constexpr const char* tags = "[core][validation][feature]";

TEST_CASE("dnd::ChoosableFeatureData::validate", tags) {
    dnd::ChoosableFeatureData data;
    dndtest::set_valid_mock_values(data, "Choosable Feature");
    dnd::Content content = dndtest::minimal_testing_content();
    dnd::Errors errors;

    SECTION("without any effect holders, at least a type is needed") {
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE_FALSE(errors.ok());

        data.type = "eldritch invocation";
        REQUIRE_NOTHROW(errors = data.validate());
        const auto& val_errors = errors.get_validation_errors();
        std::cout << val_errors.size() << std::endl;
        for (const auto& error : val_errors) {
            std::cout << error.get_error_message() << std::endl;
        }
        REQUIRE_NOTHROW(errors += data.validate_relations(content));
        REQUIRE(errors.ok());
    }
}
