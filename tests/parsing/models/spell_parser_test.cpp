#include <cctype>
#include <memory>
#include <random>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "models/spell.hpp"
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
    for (int i = 0; i < l; ++i) {
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

TEST_CASE("SpellParser: parse invalid types") {
    SECTION("other formats than standard D&D format not allowed") {
        REQUIRE_THROWS(dnd::SpellParser::createSpellComponents("Level 9 illusion"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellComponents("Cantrip - enchantment"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellComponents("0-level divination"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellComponents("9 level conjuration"));
    }
    SECTION("unknown magic school") {
        REQUIRE_THROWS(dnd::SpellParser::createSpellType("Disappointment cantrip"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellType("1st-level aspiration"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellType("9th-level intention (ritual)"));
    }
    SECTION("cantrips cannot be rituals") {
        REQUIRE_THROWS(dnd::SpellParser::createSpellType("Abjuration cantrip (ritual)"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellType("enchantment cantrip (ritual)"));
    }
    SECTION("when provided, parentheses must contain 'ritual'") {
        REQUIRE_THROWS(dnd::SpellParser::createSpellType("1st-level Enchantment (something)"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellType("8th-level illusion (ritula)"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellType("5th-level evocation (Ritual)"));
    }
    SECTION("parentheses must be after a whitespace and need to open and close") {
        REQUIRE_THROWS(dnd::SpellParser::createSpellType("2nd-level illusion(ritual)"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellType("3rd-level Divination (ritual"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellType("5th-level ritual)"));
    }
    SECTION("wrong capitalisation or uppercase in-word") {
        REQUIRE_THROWS(dnd::SpellParser::createSpellType("Abjuration Cantrip"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellType("divination CanTrip"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellType("ILLusion cantrip"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellType("enchantment Cantrip"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellType("3rD-level Divination"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellType("5Th-level (ritual)"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellType("2nd-LEVEL illusion"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellType("4th-Level necromancy"));
        REQUIRE_THROWS(dnd::SpellParser::createSpellType("1st-level TransMutation (ritual)"));
    }
}

TEST_CASE("SpellParser: parse valid types") {
    dnd::SpellType type;
    std::string s;

    const std::vector<std::string> allowed_magic_school_spellings = {
        "abjuration", "Abjuration",
        "conjuration", "Conjuration",
        "divination", "Divination",
        "enchantment", "Enchantment",
        "evocation", "Evocation",
        "illusion", "Illusion",
        "necromancy", "Necromancy",
        "transmutation", "Transmutation"
    };
    SECTION("cantrips") {
        for (const auto& spelling : allowed_magic_school_spellings) {
            s = spelling + " cantrip";
            REQUIRE_NOTHROW(type = *dnd::SpellParser::createSpellType(s));
            REQUIRE(type.level == 0);
            std::string lowercase_spelling = spelling;
            lowercase_spelling[0] = char(std::tolower(lowercase_spelling[0]));
            REQUIRE(type.magic_school == dnd::magic_schools.at(lowercase_spelling));
            REQUIRE(type.is_ritual == false);
        }
    }
    const std::vector<std::string> spell_levels = {
        "", "1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th", "9th"
    };
    SECTION("non-cantrip spells") {
        for (const auto& spelling : allowed_magic_school_spellings) {
            for (int level = 1; level <= 9; ++level) {
                s = spell_levels[level] + "-level " + spelling;
                REQUIRE_NOTHROW(type = *dnd::SpellParser::createSpellType(s));
                REQUIRE(type.level == level);
                std::string lowercase_spelling = spelling;
                lowercase_spelling[0] = char(std::tolower(lowercase_spelling[0]));
                REQUIRE(type.magic_school == dnd::magic_schools.at(lowercase_spelling));
                REQUIRE(type.is_ritual == false);
                s += " (ritual)";
                REQUIRE_NOTHROW(type = *dnd::SpellParser::createSpellType(s));
                REQUIRE(type.level == level);
                REQUIRE(type.magic_school == dnd::magic_schools.at(lowercase_spelling));
                REQUIRE(type.is_ritual == true);
            }
        }
    }
}
