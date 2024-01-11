#include <dnd_config.hpp>

#include <core/basic_mechanics/character_progression.hpp>

#include <catch2/catch_test_macros.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][basic_mechanics][character][progression]";

TEST_CASE("level_to_xp", tags) {
    SECTION("Valid character levels") {
        REQUIRE(xp_for_level(1).value() == 0);
        REQUIRE(xp_for_level(2).value() == 300);
        REQUIRE(xp_for_level(3).value() == 900);
        REQUIRE(xp_for_level(4).value() == 2700);
        REQUIRE(xp_for_level(5).value() == 6500);
        REQUIRE(xp_for_level(6).value() == 14000);
        REQUIRE(xp_for_level(7).value() == 23000);
        REQUIRE(xp_for_level(8).value() == 34000);
        REQUIRE(xp_for_level(9).value() == 48000);
        REQUIRE(xp_for_level(10).value() == 64000);
        REQUIRE(xp_for_level(11).value() == 85000);
        REQUIRE(xp_for_level(12).value() == 100000);
        REQUIRE(xp_for_level(13).value() == 120000);
        REQUIRE(xp_for_level(14).value() == 140000);
        REQUIRE(xp_for_level(15).value() == 165000);
        REQUIRE(xp_for_level(16).value() == 195000);
        REQUIRE(xp_for_level(17).value() == 225000);
        REQUIRE(xp_for_level(18).value() == 265000);
        REQUIRE(xp_for_level(19).value() == 305000);
        REQUIRE(xp_for_level(20).value() == 355000);
    }

    SECTION("Invalid character levels") {
        REQUIRE_FALSE(xp_for_level(0).has_value());
        REQUIRE_FALSE(xp_for_level(-1).has_value());
        REQUIRE_FALSE(xp_for_level(21).has_value());
        REQUIRE_FALSE(xp_for_level(100).has_value());
    }
}

TEST_CASE("xp_to_level", tags) {
    SECTION("Valid xp amounts - level 1") {
        REQUIRE(level_for_xp(0).value() == 1);
        REQUIRE(level_for_xp(60).value() == 1);
        REQUIRE(level_for_xp(150).value() == 1);
        REQUIRE(level_for_xp(299).value() == 1);
    }
    SECTION("Valid xp amounts - level 2") {
        REQUIRE(level_for_xp(300).value() == 2);
        REQUIRE(level_for_xp(450).value() == 2);
        REQUIRE(level_for_xp(613).value() == 2);
        REQUIRE(level_for_xp(899).value() == 2);
    }
    SECTION("Valid xp amounts - level 3") {
        REQUIRE(level_for_xp(900).value() == 3);
        REQUIRE(level_for_xp(1200).value() == 3);
        REQUIRE(level_for_xp(1799).value() == 3);
        REQUIRE(level_for_xp(2699).value() == 3);
    }
    SECTION("Valid xp amounts - level 4") {
        REQUIRE(level_for_xp(2700).value() == 4);
        REQUIRE(level_for_xp(3000).value() == 4);
        REQUIRE(level_for_xp(4692).value() == 4);
        REQUIRE(level_for_xp(6499).value() == 4);
    }
    SECTION("Valid xp amounts - level 5") {
        REQUIRE(level_for_xp(6500).value() == 5);
        REQUIRE(level_for_xp(7302).value() == 5);
        REQUIRE(level_for_xp(9999).value() == 5);
        REQUIRE(level_for_xp(13999).value() == 5);
    }
    SECTION("Valid xp amounts - level 6") {
        REQUIRE(level_for_xp(14000).value() == 6);
        REQUIRE(level_for_xp(15000).value() == 6);
        REQUIRE(level_for_xp(19398).value() == 6);
        REQUIRE(level_for_xp(22999).value() == 6);
    }
    SECTION("Valid xp amounts - level 7") {
        REQUIRE(level_for_xp(23000).value() == 7);
        REQUIRE(level_for_xp(25700).value() == 7);
        REQUIRE(level_for_xp(30328).value() == 7);
        REQUIRE(level_for_xp(33999).value() == 7);
    }
    SECTION("Valid xp amounts - level 8") {
        REQUIRE(level_for_xp(34000).value() == 8);
        REQUIRE(level_for_xp(37299).value() == 8);
        REQUIRE(level_for_xp(39020).value() == 8);
        REQUIRE(level_for_xp(41998).value() == 8);
        REQUIRE(level_for_xp(47999).value() == 8);
    }
    SECTION("Valid xp amounts - level 9") {
        REQUIRE(level_for_xp(48000).value() == 9);
        REQUIRE(level_for_xp(51000).value() == 9);
        REQUIRE(level_for_xp(53998).value() == 9);
        REQUIRE(level_for_xp(58139).value() == 9);
        REQUIRE(level_for_xp(60077).value() == 9);
        REQUIRE(level_for_xp(63999).value() == 9);
    }
    SECTION("Valid xp amounts - level 10") {
        REQUIRE(level_for_xp(64000).value() == 10);
        REQUIRE(level_for_xp(67000).value() == 10);
        REQUIRE(level_for_xp(71999).value() == 10);
        REQUIRE(level_for_xp(74999).value() == 10);
        REQUIRE(level_for_xp(79999).value() == 10);
        REQUIRE(level_for_xp(84999).value() == 10);
    }
    SECTION("Valid xp amounts - level 11") {
        REQUIRE(level_for_xp(85000).value() == 11);
        REQUIRE(level_for_xp(88000).value() == 11);
        REQUIRE(level_for_xp(92103).value() == 11);
        REQUIRE(level_for_xp(95999).value() == 11);
        REQUIRE(level_for_xp(99999).value() == 11);
    }
    SECTION("Valid xp amounts - level 12") {
        REQUIRE(level_for_xp(100000).value() == 12);
        REQUIRE(level_for_xp(103000).value() == 12);
        REQUIRE(level_for_xp(107471).value() == 12);
        REQUIRE(level_for_xp(114321).value() == 12);
        REQUIRE(level_for_xp(119999).value() == 12);
    }
    SECTION("Valid xp amounts - level 13") {
        REQUIRE(level_for_xp(120000).value() == 13);
        REQUIRE(level_for_xp(123000).value() == 13);
        REQUIRE(level_for_xp(129871).value() == 13);
        REQUIRE(level_for_xp(134999).value() == 13);
        REQUIRE(level_for_xp(139999).value() == 13);
    }
    SECTION("Valid xp amounts - level 14") {
        REQUIRE(level_for_xp(140000).value() == 14);
        REQUIRE(level_for_xp(143000).value() == 14);
        REQUIRE(level_for_xp(151891).value() == 14);
        REQUIRE(level_for_xp(164999).value() == 14);
    }
    SECTION("Valid xp amounts - level 15") {
        REQUIRE(level_for_xp(165000).value() == 15);
        REQUIRE(level_for_xp(168000).value() == 15);
        REQUIRE(level_for_xp(181471).value() == 15);
        REQUIRE(level_for_xp(194999).value() == 15);
    }
    SECTION("Valid xp amounts - level 16") {
        REQUIRE(level_for_xp(195000).value() == 16);
        REQUIRE(level_for_xp(198000).value() == 16);
        REQUIRE(level_for_xp(207929).value() == 16);
        REQUIRE(level_for_xp(224999).value() == 16);
    }
    SECTION("Valid xp amounts - level 17") {
        REQUIRE(level_for_xp(225000).value() == 17);
        REQUIRE(level_for_xp(228000).value() == 17);
        REQUIRE(level_for_xp(239871).value() == 17);
        REQUIRE(level_for_xp(264999).value() == 17);
    }
    SECTION("Valid xp amounts - level 18") {
        REQUIRE(level_for_xp(265000).value() == 18);
        REQUIRE(level_for_xp(268000).value() == 18);
        REQUIRE(level_for_xp(290112).value() == 18);
        REQUIRE(level_for_xp(304999).value() == 18);
    }
    SECTION("Valid xp amounts - level 19") {
        REQUIRE(level_for_xp(305000).value() == 19);
        REQUIRE(level_for_xp(308000).value() == 19);
        REQUIRE(level_for_xp(334871).value() == 19);
        REQUIRE(level_for_xp(349999).value() == 19);
    }
    SECTION("Valid xp amounts - level 20") {
        REQUIRE(level_for_xp(355000).value() == 20);
        REQUIRE(level_for_xp(373000).value() == 20);
        REQUIRE(level_for_xp(384999).value() == 20);
        REQUIRE(level_for_xp(399999).value() == 20);
        REQUIRE(level_for_xp(424999).value() == 20);
        REQUIRE(level_for_xp(449999).value() == 20);
        REQUIRE(level_for_xp(999999).value() == 20);
    }

    SECTION("Invalid xp amounts") {
        REQUIRE_FALSE(level_for_xp(-1).has_value());
        REQUIRE_FALSE(level_for_xp(-37).has_value());
        REQUIRE_FALSE(level_for_xp(-100).has_value());
        REQUIRE_FALSE(level_for_xp(-5000).has_value());
    }
}

} // namespace dnd::test
