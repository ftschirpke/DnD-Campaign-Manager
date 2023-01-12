#include "parsing/models/spells_file_parser.hpp"

#include <cctype>
#include <memory>
#include <random>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "controllers/groups.hpp"
#include "models/spell.hpp"

// class that allows us to test the dnd::SpellsFileParser class
class TestSpellsFileParser : public dnd::SpellsFileParser {
public:
    TestSpellsFileParser(std::unordered_map<std::string, const dnd::Spell>& results, dnd::Groups& groups)
        : dnd::SpellsFileParser(results, groups) {}
    dnd::SpellType createSpellTypeForTesting(const std::string& spell_type_str) const {
        return dnd::SpellsFileParser::createSpellType(spell_type_str);
    }
    dnd::SpellComponents createSpellComponentsForTesting(const std::string& spell_components_str) const {
        return dnd::SpellsFileParser::createSpellComponents(spell_components_str);
    }
    void parse() override {}
    bool validate() const override { return true; }
    void saveResult() override {}
};

class SetupSpellsParserTest {
public:
    std::unordered_map<std::string, const dnd::Spell> spells;
    dnd::Groups groups;
    TestSpellsFileParser createParser();
};

inline TestSpellsFileParser SetupSpellsParserTest::createParser() { return TestSpellsFileParser(spells, groups); }


TEST_CASE("dnd::SpellsFileParser::createSpellComponents: parse invalid components") {
    SetupSpellsParserTest setup;
    auto parser = setup.createParser();

    SECTION("completely wrong letters not allowed") {
        REQUIRE_THROWS(parser.createSpellComponentsForTesting("A, B"));
        REQUIRE_THROWS(parser.createSpellComponentsForTesting("öikje"));
    }
    SECTION("M needs an explaination in parentheses") {
        REQUIRE_THROWS(parser.createSpellComponentsForTesting("M"));
        REQUIRE_THROWS(parser.createSpellComponentsForTesting("V, M"));
        REQUIRE_THROWS(parser.createSpellComponentsForTesting("S, M"));
        REQUIRE_THROWS(parser.createSpellComponentsForTesting("V, S, M"));
    }
    SECTION("but parentheses only when M is provided") {
        REQUIRE_THROWS(parser.createSpellComponentsForTesting("V, S (something)"));
    }
    SECTION("parentheses must be after a whitespace and need to open and close") {
        REQUIRE_THROWS(parser.createSpellComponentsForTesting("V, M(another thing)"));
        REQUIRE_THROWS(parser.createSpellComponentsForTesting("M (nothing"));
        REQUIRE_THROWS(parser.createSpellComponentsForTesting("M and another)"));
    }
    SECTION("letters VSM must be uppercase") {
        REQUIRE_THROWS(parser.createSpellComponentsForTesting("m (a thread)"));
        REQUIRE_THROWS(parser.createSpellComponentsForTesting("v, s"));
        REQUIRE_THROWS(parser.createSpellComponentsForTesting("v, S"));
        REQUIRE_THROWS(parser.createSpellComponentsForTesting("V, s"));
        REQUIRE_THROWS(parser.createSpellComponentsForTesting("v, M (everything)"));
    }
}

TEST_CASE("dnd::SpellsFileParser::createSpellComponents: parse valid components") {
    SetupSpellsParserTest setup;
    auto parser = setup.createParser();

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
        REQUIRE_NOTHROW(components = parser.createSpellComponentsForTesting(s));
        REQUIRE(components.verbal == true);
        REQUIRE(components.somatic == false);
        REQUIRE(components.material == false);
        REQUIRE(components.materials_needed == "");
    }
    SECTION("S") {
        s = "S";
        REQUIRE_NOTHROW(components = parser.createSpellComponentsForTesting(s));
        REQUIRE(components.verbal == false);
        REQUIRE(components.somatic == true);
        REQUIRE(components.material == false);
        REQUIRE(components.materials_needed == "");
    }
    SECTION("M") {
        s = "M (" + random_str + ")";
        REQUIRE_NOTHROW(components = parser.createSpellComponentsForTesting(s));
        REQUIRE(components.verbal == false);
        REQUIRE(components.somatic == false);
        REQUIRE(components.material == true);
        REQUIRE(components.materials_needed == random_str);
    }
    SECTION("V, S") {
        s = "V, S";
        REQUIRE_NOTHROW(components = parser.createSpellComponentsForTesting(s));
        REQUIRE(components.verbal == true);
        REQUIRE(components.somatic == true);
        REQUIRE(components.material == false);
        REQUIRE(components.materials_needed == "");
    }
    SECTION("V, M") {
        s = "V, M (" + random_str + ")";
        REQUIRE_NOTHROW(components = parser.createSpellComponentsForTesting(s));
        REQUIRE(components.verbal == true);
        REQUIRE(components.somatic == false);
        REQUIRE(components.material == true);
        REQUIRE(components.materials_needed == random_str);
    }
    SECTION("S, M") {
        s = "S, M (" + random_str + ")";
        REQUIRE_NOTHROW(components = parser.createSpellComponentsForTesting(s));
        REQUIRE(components.verbal == false);
        REQUIRE(components.somatic == true);
        REQUIRE(components.material == true);
        REQUIRE(components.materials_needed == random_str);
    }
    SECTION("V, S, M") {
        s = "V, S, M (" + random_str + ")";
        REQUIRE_NOTHROW(components = parser.createSpellComponentsForTesting(s));
        REQUIRE(components.verbal == true);
        REQUIRE(components.somatic == true);
        REQUIRE(components.material == true);
        REQUIRE(components.materials_needed == random_str);
    }
}

TEST_CASE("dnd::SpellsFileParser::createSpellType: parse invalid types") {
    SetupSpellsParserTest setup;
    auto parser = setup.createParser();

    SECTION("other formats than standard D&D format not allowed") {
        REQUIRE_THROWS(parser.createSpellTypeForTesting("Level 9 illusion"));
        REQUIRE_THROWS(parser.createSpellTypeForTesting("Cantrip - enchantment"));
        REQUIRE_THROWS(parser.createSpellTypeForTesting("0-level divination"));
        REQUIRE_THROWS(parser.createSpellTypeForTesting("9 level conjuration"));
    }
    SECTION("unknown magic school") {
        REQUIRE_THROWS(parser.createSpellTypeForTesting("Disappointment cantrip"));
        REQUIRE_THROWS(parser.createSpellTypeForTesting("1st-level aspiration"));
        REQUIRE_THROWS(parser.createSpellTypeForTesting("9th-level intention (ritual)"));
    }
    SECTION("cantrips cannot be rituals") {
        REQUIRE_THROWS(parser.createSpellTypeForTesting("Abjuration cantrip (ritual)"));
        REQUIRE_THROWS(parser.createSpellTypeForTesting("enchantment cantrip (ritual)"));
    }
    SECTION("when provided, parentheses must contain 'ritual'") {
        REQUIRE_THROWS(parser.createSpellTypeForTesting("1st-level Enchantment (something)"));
        REQUIRE_THROWS(parser.createSpellTypeForTesting("8th-level illusion (ritula)"));
        REQUIRE_THROWS(parser.createSpellTypeForTesting("5th-level evocation (Ritual)"));
    }
    SECTION("parentheses must be after a whitespace and need to open and close") {
        REQUIRE_THROWS(parser.createSpellTypeForTesting("2nd-level illusion(ritual)"));
        REQUIRE_THROWS(parser.createSpellTypeForTesting("3rd-level Divination (ritual"));
        REQUIRE_THROWS(parser.createSpellTypeForTesting("5th-level ritual)"));
    }
    SECTION("wrong capitalisation or uppercase in-word") {
        REQUIRE_THROWS(parser.createSpellTypeForTesting("Abjuration Cantrip"));
        REQUIRE_THROWS(parser.createSpellTypeForTesting("divination CanTrip"));
        REQUIRE_THROWS(parser.createSpellTypeForTesting("ILLusion cantrip"));
        REQUIRE_THROWS(parser.createSpellTypeForTesting("enchantment Cantrip"));
        REQUIRE_THROWS(parser.createSpellTypeForTesting("3rD-level Divination"));
        REQUIRE_THROWS(parser.createSpellTypeForTesting("5Th-level (ritual)"));
        REQUIRE_THROWS(parser.createSpellTypeForTesting("2nd-LEVEL illusion"));
        REQUIRE_THROWS(parser.createSpellTypeForTesting("4th-Level necromancy"));
        REQUIRE_THROWS(parser.createSpellTypeForTesting("1st-level TransMutation (ritual)"));
    }
}

TEST_CASE("dnd::SpellsFileParser::createSpellType: parse valid types") {
    SetupSpellsParserTest setup;
    auto parser = setup.createParser();

    dnd::SpellType type;
    std::string s;

    const std::vector<std::string> allowed_magic_school_spellings = {
        "abjuration",  "Abjuration",  "conjuration",   "Conjuration",   "divination", "Divination",
        "enchantment", "Enchantment", "evocation",     "Evocation",     "illusion",   "Illusion",
        "necromancy",  "Necromancy",  "transmutation", "Transmutation",
    };
    SECTION("cantrips") {
        for (const auto& spelling : allowed_magic_school_spellings) {
            s = spelling + " cantrip";
            REQUIRE_NOTHROW(type = parser.createSpellTypeForTesting(s));
            REQUIRE(type.level == 0);
            std::string lowercase_spelling = spelling;
            lowercase_spelling[0] = char(std::tolower(lowercase_spelling[0]));
            REQUIRE(type.magic_school == dnd::magic_schools.at(lowercase_spelling));
            REQUIRE(type.is_ritual == false);
        }
    }
    const std::vector<std::string> spell_levels = {"", "1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th", "9th"};
    SECTION("non-cantrip spells") {
        for (const auto& spelling : allowed_magic_school_spellings) {
            for (int level = 1; level <= 9; ++level) {
                s = spell_levels[level] + "-level " + spelling;
                REQUIRE_NOTHROW(type = parser.createSpellTypeForTesting(s));
                REQUIRE(type.level == level);
                std::string lowercase_spelling = spelling;
                lowercase_spelling[0] = char(std::tolower(lowercase_spelling[0]));
                REQUIRE(type.magic_school == dnd::magic_schools.at(lowercase_spelling));
                REQUIRE(type.is_ritual == false);
                s += " (ritual)";
                REQUIRE_NOTHROW(type = parser.createSpellTypeForTesting(s));
                REQUIRE(type.level == level);
                REQUIRE(type.magic_school == dnd::magic_schools.at(lowercase_spelling));
                REQUIRE(type.is_ritual == true);
            }
        }
    }
}
