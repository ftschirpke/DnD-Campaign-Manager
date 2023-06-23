#include <core/basic_mechanics/character_progression.hpp>

#include <catch2/catch_test_macros.hpp>

static constexpr const char* tags = "[core][basic_mechanics][character][progression]";

TEST_CASE("dnd::level_to_xp", tags) {
    SECTION("Valid character levels") {
        REQUIRE(dnd::level_to_xp(1) == 0);
        REQUIRE(dnd::level_to_xp(2) == 300);
        REQUIRE(dnd::level_to_xp(3) == 900);
        REQUIRE(dnd::level_to_xp(4) == 2700);
        REQUIRE(dnd::level_to_xp(5) == 6500);
        REQUIRE(dnd::level_to_xp(6) == 14000);
        REQUIRE(dnd::level_to_xp(7) == 23000);
        REQUIRE(dnd::level_to_xp(8) == 34000);
        REQUIRE(dnd::level_to_xp(9) == 48000);
        REQUIRE(dnd::level_to_xp(10) == 64000);
        REQUIRE(dnd::level_to_xp(11) == 85000);
        REQUIRE(dnd::level_to_xp(12) == 100000);
        REQUIRE(dnd::level_to_xp(13) == 120000);
        REQUIRE(dnd::level_to_xp(14) == 140000);
        REQUIRE(dnd::level_to_xp(15) == 165000);
        REQUIRE(dnd::level_to_xp(16) == 195000);
        REQUIRE(dnd::level_to_xp(17) == 225000);
        REQUIRE(dnd::level_to_xp(18) == 265000);
        REQUIRE(dnd::level_to_xp(19) == 305000);
        REQUIRE(dnd::level_to_xp(20) == 355000);
    }

    SECTION("Invalid character levels") {
        REQUIRE_THROWS_AS(dnd::level_to_xp(0), std::invalid_argument);
        REQUIRE_THROWS_AS(dnd::level_to_xp(-1), std::invalid_argument);
        REQUIRE_THROWS_AS(dnd::level_to_xp(21), std::invalid_argument);
        REQUIRE_THROWS_AS(dnd::level_to_xp(100), std::invalid_argument);
    }
}

TEST_CASE("dnd::xp_to_level", tags) {
    SECTION("Valid xp amounts - level 1") {
        REQUIRE(dnd::xp_to_level(0) == 1);
        REQUIRE(dnd::xp_to_level(60) == 1);
        REQUIRE(dnd::xp_to_level(150) == 1);
        REQUIRE(dnd::xp_to_level(299) == 1);
    }
    SECTION("Valid xp amounts - level 2") {
        REQUIRE(dnd::xp_to_level(300) == 2);
        REQUIRE(dnd::xp_to_level(450) == 2);
        REQUIRE(dnd::xp_to_level(613) == 2);
        REQUIRE(dnd::xp_to_level(899) == 2);
    }
    SECTION("Valid xp amounts - level 3") {
        REQUIRE(dnd::xp_to_level(900) == 3);
        REQUIRE(dnd::xp_to_level(1200) == 3);
        REQUIRE(dnd::xp_to_level(1799) == 3);
        REQUIRE(dnd::xp_to_level(2699) == 3);
    }
    SECTION("Valid xp amounts - level 4") {
        REQUIRE(dnd::xp_to_level(2700) == 4);
        REQUIRE(dnd::xp_to_level(3000) == 4);
        REQUIRE(dnd::xp_to_level(4692) == 4);
        REQUIRE(dnd::xp_to_level(6499) == 4);
    }
    SECTION("Valid xp amounts - level 5") {
        REQUIRE(dnd::xp_to_level(6500) == 5);
        REQUIRE(dnd::xp_to_level(7302) == 5);
        REQUIRE(dnd::xp_to_level(9999) == 5);
        REQUIRE(dnd::xp_to_level(13999) == 5);
    }
    SECTION("Valid xp amounts - level 6") {
        REQUIRE(dnd::xp_to_level(14000) == 6);
        REQUIRE(dnd::xp_to_level(15000) == 6);
        REQUIRE(dnd::xp_to_level(19398) == 6);
        REQUIRE(dnd::xp_to_level(22999) == 6);
    }
    SECTION("Valid xp amounts - level 7") {
        REQUIRE(dnd::xp_to_level(23000) == 7);
        REQUIRE(dnd::xp_to_level(25700) == 7);
        REQUIRE(dnd::xp_to_level(30328) == 7);
        REQUIRE(dnd::xp_to_level(33999) == 7);
    }
    SECTION("Valid xp amounts - level 8") {
        REQUIRE(dnd::xp_to_level(34000) == 8);
        REQUIRE(dnd::xp_to_level(37299) == 8);
        REQUIRE(dnd::xp_to_level(39020) == 8);
        REQUIRE(dnd::xp_to_level(41998) == 8);
        REQUIRE(dnd::xp_to_level(47999) == 8);
    }
    SECTION("Valid xp amounts - level 9") {
        REQUIRE(dnd::xp_to_level(48000) == 9);
        REQUIRE(dnd::xp_to_level(51000) == 9);
        REQUIRE(dnd::xp_to_level(53998) == 9);
        REQUIRE(dnd::xp_to_level(58139) == 9);
        REQUIRE(dnd::xp_to_level(60077) == 9);
        REQUIRE(dnd::xp_to_level(63999) == 9);
    }
    SECTION("Valid xp amounts - level 10") {
        REQUIRE(dnd::xp_to_level(64000) == 10);
        REQUIRE(dnd::xp_to_level(67000) == 10);
        REQUIRE(dnd::xp_to_level(71999) == 10);
        REQUIRE(dnd::xp_to_level(74999) == 10);
        REQUIRE(dnd::xp_to_level(79999) == 10);
        REQUIRE(dnd::xp_to_level(84999) == 10);
    }
    SECTION("Valid xp amounts - level 11") {
        REQUIRE(dnd::xp_to_level(85000) == 11);
        REQUIRE(dnd::xp_to_level(88000) == 11);
        REQUIRE(dnd::xp_to_level(92103) == 11);
        REQUIRE(dnd::xp_to_level(95999) == 11);
        REQUIRE(dnd::xp_to_level(99999) == 11);
    }
    SECTION("Valid xp amounts - level 12") {
        REQUIRE(dnd::xp_to_level(100000) == 12);
        REQUIRE(dnd::xp_to_level(103000) == 12);
        REQUIRE(dnd::xp_to_level(107471) == 12);
        REQUIRE(dnd::xp_to_level(114321) == 12);
        REQUIRE(dnd::xp_to_level(119999) == 12);
    }
    SECTION("Valid xp amounts - level 13") {
        REQUIRE(dnd::xp_to_level(120000) == 13);
        REQUIRE(dnd::xp_to_level(123000) == 13);
        REQUIRE(dnd::xp_to_level(129871) == 13);
        REQUIRE(dnd::xp_to_level(134999) == 13);
        REQUIRE(dnd::xp_to_level(139999) == 13);
    }
    SECTION("Valid xp amounts - level 14") {
        REQUIRE(dnd::xp_to_level(140000) == 14);
        REQUIRE(dnd::xp_to_level(143000) == 14);
        REQUIRE(dnd::xp_to_level(151891) == 14);
        REQUIRE(dnd::xp_to_level(164999) == 14);
    }
    SECTION("Valid xp amounts - level 15") {
        REQUIRE(dnd::xp_to_level(165000) == 15);
        REQUIRE(dnd::xp_to_level(168000) == 15);
        REQUIRE(dnd::xp_to_level(181471) == 15);
        REQUIRE(dnd::xp_to_level(194999) == 15);
    }
    SECTION("Valid xp amounts - level 16") {
        REQUIRE(dnd::xp_to_level(195000) == 16);
        REQUIRE(dnd::xp_to_level(198000) == 16);
        REQUIRE(dnd::xp_to_level(207929) == 16);
        REQUIRE(dnd::xp_to_level(224999) == 16);
    }
    SECTION("Valid xp amounts - level 17") {
        REQUIRE(dnd::xp_to_level(225000) == 17);
        REQUIRE(dnd::xp_to_level(228000) == 17);
        REQUIRE(dnd::xp_to_level(239871) == 17);
        REQUIRE(dnd::xp_to_level(264999) == 17);
    }
    SECTION("Valid xp amounts - level 18") {
        REQUIRE(dnd::xp_to_level(265000) == 18);
        REQUIRE(dnd::xp_to_level(268000) == 18);
        REQUIRE(dnd::xp_to_level(290112) == 18);
        REQUIRE(dnd::xp_to_level(304999) == 18);
    }
    SECTION("Valid xp amounts - level 19") {
        REQUIRE(dnd::xp_to_level(305000) == 19);
        REQUIRE(dnd::xp_to_level(308000) == 19);
        REQUIRE(dnd::xp_to_level(334871) == 19);
        REQUIRE(dnd::xp_to_level(349999) == 19);
    }
    SECTION("Valid xp amounts - level 20") {
        REQUIRE(dnd::xp_to_level(355000) == 20);
        REQUIRE(dnd::xp_to_level(373000) == 20);
        REQUIRE(dnd::xp_to_level(384999) == 20);
        REQUIRE(dnd::xp_to_level(399999) == 20);
        REQUIRE(dnd::xp_to_level(424999) == 20);
        REQUIRE(dnd::xp_to_level(449999) == 20);
        REQUIRE(dnd::xp_to_level(999999) == 20);
    }

    SECTION("Invalid xp amounts") {
        REQUIRE_THROWS_AS(dnd::xp_to_level(-1), std::invalid_argument);
        REQUIRE_THROWS_AS(dnd::xp_to_level(-37), std::invalid_argument);
        REQUIRE_THROWS_AS(dnd::xp_to_level(-100), std::invalid_argument);
        REQUIRE_THROWS_AS(dnd::xp_to_level(-5000), std::invalid_argument);
    }
}
