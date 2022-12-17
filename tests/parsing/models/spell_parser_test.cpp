#include <memory>
#include <random>

#include <catch2/catch_test_macros.hpp>

#include "parsing/models/spell_parser.hpp"

TEST_CASE("SpellParser: parse invalid components") {
    SECTION("completely wrong letters not allowed") {
        REQUIRE_THROWS(dnd::SpellParser::createSpellComponents("A, B"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellComponents("öikje"));
    }
    SECTION("M needs an explaination in parentheses") {
        REQUIRE_THROWS(dnd::SpellParser::createSpellComponents("M"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellComponents("V, M"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellComponents("S, M"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellComponents("V, S, M"));
    }
    SECTION("but parentheses only when M is provided") {
        REQUIRE_THROWS(dnd::SpellParser::createSpellComponents("V, S (something)"));
    }
    SECTION("parentheses must be after a whitespace and need to open and close") {
        REQUIRE_THROWS(dnd::SpellParser::createSpellComponents("V, M(another thing)"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellComponents("M (nothing"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellComponents("M and another)"));
    }
    SECTION("letters VSM must be uppercase") {
        REQUIRE_THROWS(dnd::SpellParser::createSpellComponents("m (a thread)"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellComponents("v, s"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellComponents("v, S"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellComponents("V, s"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellComponents("v, M (everything)"));
    }
}

TEST_CASE("SpellParser: parse valid components") {
    dnd::SpellComponents components;
    std::string s;

    // generate a random string of random length
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> len_distrib(1, 255);
    std::uniform_int_distribution<> distrib(32, 255);
    std::string random_str;
    int l = len_distrib(gen);
    random_str.resize(l);
    for (int i = 0; i < l; i++) {
        int ascii = distrib(gen);
        while (126 < ascii && ascii < 161) {
            ascii = distrib(gen);
        }
        random_str[i] = char(ascii);
    }
    SECTION("V") {
        s = "V";
        REQUIRE_NOTHROW(components = *dnd::SpellParser::createSpellComponents(s));
        REQUIRE(components.verbal == true);
        REQUIRE(components.somatic == false);
        REQUIRE(components.material == false);
        REQUIRE(components.materials_needed == "");
    }
    SECTION("S") {
        s = "S";
        REQUIRE_NOTHROW(components = *dnd::SpellParser::createSpellComponents(s));
        REQUIRE(components.verbal == false);
        REQUIRE(components.somatic == true);
        REQUIRE(components.material == false);
        REQUIRE(components.materials_needed == "");
    }
    SECTION("M") {
        s = "M (" + random_str + ")";
        REQUIRE_NOTHROW(components = *dnd::SpellParser::createSpellComponents(s));
        REQUIRE(components.verbal == false);
        REQUIRE(components.somatic == false);
        REQUIRE(components.material == true);
        REQUIRE(components.materials_needed == random_str);
    }
    SECTION("V, S") {
        s = "V, S";
        REQUIRE_NOTHROW(components = *dnd::SpellParser::createSpellComponents(s));
        REQUIRE(components.verbal == true);
        REQUIRE(components.somatic == true);
        REQUIRE(components.material == false);
        REQUIRE(components.materials_needed == "");
    }
    SECTION("V, M") {
        s = "V, M (" + random_str + ")";
        REQUIRE_NOTHROW(components = *dnd::SpellParser::createSpellComponents(s));
        REQUIRE(components.verbal == true);
        REQUIRE(components.somatic == false);
        REQUIRE(components.material == true);
        REQUIRE(components.materials_needed == random_str);
    }
    SECTION("S, M") {
        s = "S, M (" + random_str + ")";
        REQUIRE_NOTHROW(components = *dnd::SpellParser::createSpellComponents(s));
        REQUIRE(components.verbal == false);
        REQUIRE(components.somatic == true);
        REQUIRE(components.material == true);
        REQUIRE(components.materials_needed == random_str);
    }
    SECTION("V, S, M") {
        s = "V, S, M (" + random_str + ")";
        REQUIRE_NOTHROW(components = *dnd::SpellParser::createSpellComponents(s));
        REQUIRE(components.verbal == true);
        REQUIRE(components.somatic == true);
        REQUIRE(components.material == true);
        REQUIRE(components.materials_needed == random_str);
    }
}