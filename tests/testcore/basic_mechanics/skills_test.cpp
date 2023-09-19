#include <dnd_config.hpp>

#include <core/basic_mechanics/skills.hpp>

#include <catch2/catch_test_macros.hpp>

static constexpr const char* tags = "[core][basic_mechanics][skills]";

TEST_CASE("dnd::is_skill // valid uppercase underscore") {
    REQUIRE(dnd::is_skill("ACROBATICS"));
    REQUIRE(dnd::is_skill("ANIMAL_HANDLING"));
    REQUIRE(dnd::is_skill("ARCANA"));
    REQUIRE(dnd::is_skill("ATHLETICS"));
    REQUIRE(dnd::is_skill("DECEPTION"));
    REQUIRE(dnd::is_skill("HISTORY"));
    REQUIRE(dnd::is_skill("INSIGHT"));
    REQUIRE(dnd::is_skill("INTIMIDATION"));
    REQUIRE(dnd::is_skill("INVESTIGATION"));
    REQUIRE(dnd::is_skill("MEDICINE"));
    REQUIRE(dnd::is_skill("NATURE"));
    REQUIRE(dnd::is_skill("PERCEPTION"));
    REQUIRE(dnd::is_skill("PERFORMANCE"));
    REQUIRE(dnd::is_skill("PERSUASION"));
    REQUIRE(dnd::is_skill("RELIGION"));
    REQUIRE(dnd::is_skill("SLEIGHT_OF_HAND"));
    REQUIRE(dnd::is_skill("STEALTH"));
    REQUIRE(dnd::is_skill("SURVIVAL"));
}

TEST_CASE("dnd::is_skill // valid lowercase") {
    REQUIRE(dnd::is_skill("acrobatics"));
    REQUIRE(dnd::is_skill("animal handling"));
    REQUIRE(dnd::is_skill("animal_handling"));
    REQUIRE(dnd::is_skill("arcana"));
    REQUIRE(dnd::is_skill("athletics"));
    REQUIRE(dnd::is_skill("deception"));
    REQUIRE(dnd::is_skill("history"));
    REQUIRE(dnd::is_skill("insight"));
    REQUIRE(dnd::is_skill("intimidation"));
    REQUIRE(dnd::is_skill("investigation"));
    REQUIRE(dnd::is_skill("medicine"));
    REQUIRE(dnd::is_skill("nature"));
    REQUIRE(dnd::is_skill("perception"));
    REQUIRE(dnd::is_skill("performance"));
    REQUIRE(dnd::is_skill("persuasion"));
    REQUIRE(dnd::is_skill("religion"));
    REQUIRE(dnd::is_skill("sleight of hand"));
    REQUIRE(dnd::is_skill("sleight_of_hand"));
    REQUIRE(dnd::is_skill("stealth"));
    REQUIRE(dnd::is_skill("survival"));
}

TEST_CASE("dnd::is_skill // invalid") {
    SECTION("not a skill") {
        REQUIRE_FALSE(dnd::is_skill(""));
        REQUIRE_FALSE(dnd::is_skill("not a skill"));
        REQUIRE_FALSE(dnd::is_skill("not_a_skill"));
        REQUIRE_FALSE(dnd::is_skill("NOT_A_SKILL"));
        REQUIRE_FALSE(dnd::is_skill("STRENGTH"));
        REQUIRE_FALSE(dnd::is_skill("strength"));
        REQUIRE_FALSE(dnd::is_skill("DEXTERITY"));
        REQUIRE_FALSE(dnd::is_skill("dexterity"));
        REQUIRE_FALSE(dnd::is_skill("CONSTITUTION"));
        REQUIRE_FALSE(dnd::is_skill("constitution"));
        REQUIRE_FALSE(dnd::is_skill("INTELLIGENCE"));
        REQUIRE_FALSE(dnd::is_skill("intelligence"));
        REQUIRE_FALSE(dnd::is_skill("WISDOM"));
        REQUIRE_FALSE(dnd::is_skill("wisdom"));
        REQUIRE_FALSE(dnd::is_skill("CHARISMA"));
        REQUIRE_FALSE(dnd::is_skill("charisma"));
    }

    SECTION("wrong formatting") {
        REQUIRE_FALSE(dnd::is_skill("acro"));
        REQUIRE_FALSE(dnd::is_skill("athl"));
        REQUIRE_FALSE(dnd::is_skill("animal-handling"));
        REQUIRE_FALSE(dnd::is_skill("sleight-of-hand"));
    }
}
