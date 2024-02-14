#include <dnd_config.hpp>

#include <core/basic_mechanics/skills.hpp>

#include <catch2/catch_test_macros.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][basic_mechanics][skills]";

TEST_CASE("is_skill // valid uppercase underscore", tags) {
    REQUIRE(is_skill("ACROBATICS"));
    REQUIRE(is_skill("ANIMAL_HANDLING"));
    REQUIRE(is_skill("ARCANA"));
    REQUIRE(is_skill("ATHLETICS"));
    REQUIRE(is_skill("DECEPTION"));
    REQUIRE(is_skill("HISTORY"));
    REQUIRE(is_skill("INSIGHT"));
    REQUIRE(is_skill("INTIMIDATION"));
    REQUIRE(is_skill("INVESTIGATION"));
    REQUIRE(is_skill("MEDICINE"));
    REQUIRE(is_skill("NATURE"));
    REQUIRE(is_skill("PERCEPTION"));
    REQUIRE(is_skill("PERFORMANCE"));
    REQUIRE(is_skill("PERSUASION"));
    REQUIRE(is_skill("RELIGION"));
    REQUIRE(is_skill("SLEIGHT_OF_HAND"));
    REQUIRE(is_skill("STEALTH"));
    REQUIRE(is_skill("SURVIVAL"));
}

TEST_CASE("is_skill // valid lowercase", tags) {
    REQUIRE(is_skill("acrobatics"));
    REQUIRE(is_skill("animal handling"));
    REQUIRE(is_skill("arcana"));
    REQUIRE(is_skill("athletics"));
    REQUIRE(is_skill("deception"));
    REQUIRE(is_skill("history"));
    REQUIRE(is_skill("insight"));
    REQUIRE(is_skill("intimidation"));
    REQUIRE(is_skill("investigation"));
    REQUIRE(is_skill("medicine"));
    REQUIRE(is_skill("nature"));
    REQUIRE(is_skill("perception"));
    REQUIRE(is_skill("performance"));
    REQUIRE(is_skill("persuasion"));
    REQUIRE(is_skill("religion"));
    REQUIRE(is_skill("sleight of hand"));
    REQUIRE(is_skill("stealth"));
    REQUIRE(is_skill("survival"));
}

TEST_CASE("is_skill // invalid", tags) {
    SECTION("not a skill") {
        REQUIRE_FALSE(is_skill(""));
        REQUIRE_FALSE(is_skill("not a skill"));
        REQUIRE_FALSE(is_skill("not_a_skill"));
        REQUIRE_FALSE(is_skill("NOT_A_SKILL"));
        REQUIRE_FALSE(is_skill("STRENGTH"));
        REQUIRE_FALSE(is_skill("strength"));
        REQUIRE_FALSE(is_skill("DEXTERITY"));
        REQUIRE_FALSE(is_skill("dexterity"));
        REQUIRE_FALSE(is_skill("CONSTITUTION"));
        REQUIRE_FALSE(is_skill("constitution"));
        REQUIRE_FALSE(is_skill("INTELLIGENCE"));
        REQUIRE_FALSE(is_skill("intelligence"));
        REQUIRE_FALSE(is_skill("WISDOM"));
        REQUIRE_FALSE(is_skill("wisdom"));
        REQUIRE_FALSE(is_skill("CHARISMA"));
        REQUIRE_FALSE(is_skill("charisma"));
    }

    SECTION("wrong formatting", tags) {
        REQUIRE_FALSE(is_skill("acro"));
        REQUIRE_FALSE(is_skill("athl"));
        REQUIRE_FALSE(is_skill("animal-handling"));
        REQUIRE_FALSE(is_skill("animal_handling"));
        REQUIRE_FALSE(is_skill("sleight-of-hand"));
        REQUIRE_FALSE(is_skill("sleight_of_hand"));
    }
}

} // namespace dnd::test
