#include <core/parsing/models/spells_file_parser.hpp>

#include <cctype>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <core/controllers/content_library.hpp>
#include <core/controllers/groups.hpp>
#include <core/models/spell.hpp>
#include <core/utils/char_manipulation.hpp>

/**
 * @brief A class that allows us to test the dnd::SpellsFileParser class
 */
class TestSpellsFileParser : public dnd::SpellsFileParser {
public:
    TestSpellsFileParser(dnd::StorageContentLibrary<const dnd::Spell>& spells, dnd::Groups& groups)
        : dnd::SpellsFileParser("", spells, groups) {}
    dnd::SpellType create_spell_type_for_testing(const std::string& spell_type_str) const {
        return dnd::SpellsFileParser::create_spell_type(spell_type_str);
    }
    dnd::SpellComponents create_spell_components_for_testing(const std::string& spell_components_str) const {
        return dnd::SpellsFileParser::create_spell_components(spell_components_str);
    }
    void parse() override {}
    bool validate() const override { return true; }
    void save_result() override {}
};

class SetupSpellsParserTest {
public:
    dnd::StorageContentLibrary<const dnd::Spell> spells;
    dnd::Groups groups;
    TestSpellsFileParser createParser();
};

inline TestSpellsFileParser SetupSpellsParserTest::createParser() { return TestSpellsFileParser(spells, groups); }


TEST_CASE("dnd::SpellsFileParser::create_spell_components: parse invalid components") {
    SetupSpellsParserTest setup;
    auto parser = setup.createParser();

    SECTION("completely wrong letters not allowed") {
        REQUIRE_THROWS(parser.create_spell_components_for_testing("A, B"));
        REQUIRE_THROWS(parser.create_spell_components_for_testing("öikje"));
    }
    SECTION("M needs an explaination in parentheses") {
        REQUIRE_THROWS(parser.create_spell_components_for_testing("M"));
        REQUIRE_THROWS(parser.create_spell_components_for_testing("V, M"));
        REQUIRE_THROWS(parser.create_spell_components_for_testing("S, M"));
        REQUIRE_THROWS(parser.create_spell_components_for_testing("V, S, M"));
    }
    SECTION("but parentheses only when M is provided") {
        REQUIRE_THROWS(parser.create_spell_components_for_testing("V, S (something)"));
    }
    SECTION("parentheses must be after a whitespace and need to open and close") {
        REQUIRE_THROWS(parser.create_spell_components_for_testing("V, M(another thing)"));
        REQUIRE_THROWS(parser.create_spell_components_for_testing("M (nothing"));
        REQUIRE_THROWS(parser.create_spell_components_for_testing("M and another)"));
    }
    SECTION("letters VSM must be uppercase") {
        REQUIRE_THROWS(parser.create_spell_components_for_testing("m (a thread)"));
        REQUIRE_THROWS(parser.create_spell_components_for_testing("v, s"));
        REQUIRE_THROWS(parser.create_spell_components_for_testing("v, S"));
        REQUIRE_THROWS(parser.create_spell_components_for_testing("V, s"));
        REQUIRE_THROWS(parser.create_spell_components_for_testing("v, M (everything)"));
    }
}

TEST_CASE("dnd::SpellsFileParser::create_spell_components: parse valid components") {
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
    size_t l = static_cast<size_t>(len_distrib(gen));
    random_str.resize(l);
    for (size_t i = 0; i < l; ++i) {
        int ascii = distrib(gen);
        while (126 < ascii && ascii < 161) {
            ascii = distrib(gen);
        }
        random_str[i] = char(ascii);
    }
    SECTION("V") {
        s = "V";
        REQUIRE_NOTHROW(components = parser.create_spell_components_for_testing(s));
        REQUIRE(components.verbal == true);
        REQUIRE(components.somatic == false);
        REQUIRE(components.material == false);
        REQUIRE(components.materials_needed == "");
    }
    SECTION("S") {
        s = "S";
        REQUIRE_NOTHROW(components = parser.create_spell_components_for_testing(s));
        REQUIRE(components.verbal == false);
        REQUIRE(components.somatic == true);
        REQUIRE(components.material == false);
        REQUIRE(components.materials_needed == "");
    }
    SECTION("M") {
        s = "M (" + random_str + ")";
        REQUIRE_NOTHROW(components = parser.create_spell_components_for_testing(s));
        REQUIRE(components.verbal == false);
        REQUIRE(components.somatic == false);
        REQUIRE(components.material == true);
        REQUIRE(components.materials_needed == random_str);
    }
    SECTION("V, S") {
        s = "V, S";
        REQUIRE_NOTHROW(components = parser.create_spell_components_for_testing(s));
        REQUIRE(components.verbal == true);
        REQUIRE(components.somatic == true);
        REQUIRE(components.material == false);
        REQUIRE(components.materials_needed == "");
    }
    SECTION("V, M") {
        s = "V, M (" + random_str + ")";
        REQUIRE_NOTHROW(components = parser.create_spell_components_for_testing(s));
        REQUIRE(components.verbal == true);
        REQUIRE(components.somatic == false);
        REQUIRE(components.material == true);
        REQUIRE(components.materials_needed == random_str);
    }
    SECTION("S, M") {
        s = "S, M (" + random_str + ")";
        REQUIRE_NOTHROW(components = parser.create_spell_components_for_testing(s));
        REQUIRE(components.verbal == false);
        REQUIRE(components.somatic == true);
        REQUIRE(components.material == true);
        REQUIRE(components.materials_needed == random_str);
    }
    SECTION("V, S, M") {
        s = "V, S, M (" + random_str + ")";
        REQUIRE_NOTHROW(components = parser.create_spell_components_for_testing(s));
        REQUIRE(components.verbal == true);
        REQUIRE(components.somatic == true);
        REQUIRE(components.material == true);
        REQUIRE(components.materials_needed == random_str);
    }
}

TEST_CASE("dnd::SpellsFileParser::create_spell_type: parse invalid types") {
    SetupSpellsParserTest setup;
    auto parser = setup.createParser();

    SECTION("other formats than standard D&D format not allowed") {
        REQUIRE_THROWS(parser.create_spell_type_for_testing("Level 9 illusion"));
        REQUIRE_THROWS(parser.create_spell_type_for_testing("Cantrip - enchantment"));
        REQUIRE_THROWS(parser.create_spell_type_for_testing("0-level divination"));
        REQUIRE_THROWS(parser.create_spell_type_for_testing("9 level conjuration"));
    }
    SECTION("unknown magic school") {
        REQUIRE_THROWS(parser.create_spell_type_for_testing("Disappointment cantrip"));
        REQUIRE_THROWS(parser.create_spell_type_for_testing("1st-level aspiration"));
        REQUIRE_THROWS(parser.create_spell_type_for_testing("9th-level intention (ritual)"));
    }
    SECTION("cantrips cannot be rituals") {
        REQUIRE_THROWS(parser.create_spell_type_for_testing("Abjuration cantrip (ritual)"));
        REQUIRE_THROWS(parser.create_spell_type_for_testing("enchantment cantrip (ritual)"));
    }
    SECTION("when provided, parentheses must contain 'ritual'") {
        REQUIRE_THROWS(parser.create_spell_type_for_testing("1st-level Enchantment (something)"));
        REQUIRE_THROWS(parser.create_spell_type_for_testing("8th-level illusion (ritula)"));
        REQUIRE_THROWS(parser.create_spell_type_for_testing("5th-level evocation (Ritual)"));
    }
    SECTION("parentheses must be after a whitespace and need to open and close") {
        REQUIRE_THROWS(parser.create_spell_type_for_testing("2nd-level illusion(ritual)"));
        REQUIRE_THROWS(parser.create_spell_type_for_testing("3rd-level Divination (ritual"));
        REQUIRE_THROWS(parser.create_spell_type_for_testing("5th-level ritual)"));
    }
    SECTION("wrong capitalisation or uppercase in-word") {
        REQUIRE_THROWS(parser.create_spell_type_for_testing("Abjuration Cantrip"));
        REQUIRE_THROWS(parser.create_spell_type_for_testing("divination CanTrip"));
        REQUIRE_THROWS(parser.create_spell_type_for_testing("ILLusion cantrip"));
        REQUIRE_THROWS(parser.create_spell_type_for_testing("enchantment Cantrip"));
        REQUIRE_THROWS(parser.create_spell_type_for_testing("3rD-level Divination"));
        REQUIRE_THROWS(parser.create_spell_type_for_testing("5Th-level (ritual)"));
        REQUIRE_THROWS(parser.create_spell_type_for_testing("2nd-LEVEL illusion"));
        REQUIRE_THROWS(parser.create_spell_type_for_testing("4th-Level necromancy"));
        REQUIRE_THROWS(parser.create_spell_type_for_testing("1st-level TransMutation (ritual)"));
    }
}

TEST_CASE("dnd::SpellsFileParser::create_spell_type: parse valid types") {
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
            REQUIRE_NOTHROW(type = parser.create_spell_type_for_testing(s));
            REQUIRE(type.level_number() == 0);
            std::string lowercase_spelling = spelling;
            lowercase_spelling[0] = dnd::char_to_lowercase(lowercase_spelling[0]);
            REQUIRE(type.magic_school == dnd::magic_school_from_name(lowercase_spelling));
            REQUIRE(type.is_ritual == false);
        }
    }
    const std::vector<std::string> spell_levels = {"", "1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th", "9th"};
    SECTION("non-cantrip spells") {
        for (const auto& spelling : allowed_magic_school_spellings) {
            auto spell_level_it = spell_levels.cbegin();
            for (int level = 1; level <= 9; ++level) {
                s = *(++spell_level_it) + "-level " + spelling;
                REQUIRE_NOTHROW(type = parser.create_spell_type_for_testing(s));
                REQUIRE(type.level_number() == level);
                std::string lowercase_spelling = spelling;
                lowercase_spelling[0] = dnd::char_to_lowercase(lowercase_spelling[0]);
                REQUIRE(type.magic_school == dnd::magic_school_from_name(lowercase_spelling));
                REQUIRE(type.is_ritual == false);
                s += " (ritual)";
                REQUIRE_NOTHROW(type = parser.create_spell_type_for_testing(s));
                REQUIRE(type.level_number() == level);
                REQUIRE(type.magic_school == dnd::magic_school_from_name(lowercase_spelling));
                REQUIRE(type.is_ritual == true);
            }
        }
    }
}
