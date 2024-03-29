#include <dnd_config.hpp>

#include <core/validation/effects/subholders/riv_holder_validation.hpp>

#include <catch2/catch_test_macros.hpp>

#include <core/errors/errors.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][effects]";

TEST_CASE("Validate RIVHolder", tags) {
    ValidationDataMock parent;
    RIVHolder::Data data;
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("empty") {
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("damage resistances only") {
        data.damage_resistances = {"piercing"};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.damage_resistances = {"bludgeoning", "slashing"};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("damage immunities only") {
        data.damage_immunities = {"bludgeoning"};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.damage_immunities = {"piercing", "slashing"};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("damage vulnerabilities only") {
        data.damage_vulnerabilities = {"slashing"};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.damage_vulnerabilities = {"bludgeoning", "piercing"};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("condition immunities only") {
        data.condition_immunities = {"blinded"};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE(errors.ok());

        data.condition_immunities = {"charmed", "deafened"};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }

    SECTION("all") {
        data.damage_resistances = {"piercing"};
        data.damage_immunities = {"bludgeoning"};
        data.damage_vulnerabilities = {"slashing"};
        data.condition_immunities = {"charmed", "deafened"};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE(errors.ok());
    }
}

TEST_CASE("Validate RIVHolder // invalid RIV holders") {
    ValidationDataMock parent;
    RIVHolder::Data data;
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("invalid damage resistances") {
        data.damage_resistances = {""};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.damage_resistances = {"bludgeoning", ""};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("invalid damage immunities") {
        data.damage_immunities = {""};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.damage_immunities = {"piercing", ""};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("invalid damage vulnerabilities") {
        data.damage_vulnerabilities = {""};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.damage_vulnerabilities = {"slashing", ""};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("invalid condition immunities") {
        data.condition_immunities = {""};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.condition_immunities = {"blinded", ""};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("multiple invalid values") {
        data.damage_resistances = {""};
        data.damage_vulnerabilities = {""};
        data.damage_immunities = {""};
        data.condition_immunities = {""};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.damage_resistances = {"bludgeoning", ""};
        data.damage_immunities = {"piercing", ""};
        data.damage_vulnerabilities = {"slashing", ""};
        data.condition_immunities = {"blinded", "", "charmed"};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("Validate RIVHolder // invalid RIV holder relations") {
    ValidationDataMock parent;
    RIVHolder::Data data;
    Content content = minimal_testing_content();
    Errors errors;

    SECTION("unknown damage resistance") {
        data.damage_resistances = {"unknown"};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.damage_resistances = {"bludgeoning", "unknown"};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown damage immunity") {
        data.damage_immunities = {"unknown"};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.damage_immunities = {"piercing", "unknown"};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown damage vulnerability") {
        data.damage_vulnerabilities = {"unknown"};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.damage_vulnerabilities = {"slashing", "unknown"};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("unknown condition immunity") {
        data.condition_immunities = {"unknown"};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.condition_immunities = {"blinded", "unknown"};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }

    SECTION("multiple unknowns") {
        data.damage_resistances = {"unknown"};
        data.damage_immunities = {"unknown"};
        data.damage_vulnerabilities = {"unknown"};
        data.condition_immunities = {"unknown"};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());

        data.damage_resistances = {"bludgeoning", "unknown"};
        data.damage_immunities = {"piercing", "unknown"};
        data.damage_vulnerabilities = {"slashing", "unknown"};
        data.condition_immunities = {"blinded", "unknown", "charmed"};
        REQUIRE_NOTHROW(errors = validate_riv_holder_for_content(data, content));
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("RIVHolder::Data::empty", tags) {
    ValidationDataMock parent;
    RIVHolder::Data data;

    REQUIRE(data.empty());

    SECTION("damage resistance") {
        data.damage_resistances = {"bludgeoning", "slashing"};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("damage immunity") {
        data.damage_immunities = {"piercing", "slashing"};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("damage vulnerability") {
        data.damage_vulnerabilities = {"bludgeoning", "piercing"};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("condition immunity") {
        data.condition_immunities = {"blinded", "charmed"};
        REQUIRE_FALSE(data.empty());
    }

    SECTION("all") {
        data.damage_resistances = {"bludgeoning", "slashing"};
        data.damage_immunities = {"piercing", "slashing"};
        data.damage_vulnerabilities = {"bludgeoning", "piercing"};
        data.condition_immunities = {"blinded", "charmed"};
        REQUIRE_FALSE(data.empty());
    }
}

} // namespace dnd::test
