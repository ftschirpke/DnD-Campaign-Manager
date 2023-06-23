#include <core/utils/char_manipulation.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("dnd::uchar_to_lowercase", "[utils][char_manipulation]") {
    REQUIRE(dnd::uchar_to_lowercase('A') == 'a');
    REQUIRE(dnd::uchar_to_lowercase('Z') == 'z');
    REQUIRE(dnd::uchar_to_lowercase('a') == 'a');
    REQUIRE(dnd::uchar_to_lowercase('z') == 'z');
    REQUIRE(dnd::uchar_to_lowercase('0') == '0');
    REQUIRE(dnd::uchar_to_lowercase('9') == '9');
    REQUIRE(dnd::uchar_to_lowercase('!') == '!');
    REQUIRE(dnd::uchar_to_lowercase(' ') == ' ');
    REQUIRE(dnd::uchar_to_lowercase('\n') == '\n');
    REQUIRE(dnd::uchar_to_lowercase('\t') == '\t');
    REQUIRE(dnd::uchar_to_lowercase('\0') == '\0');
}

TEST_CASE("dnd::char_to_lowercase", "[utils][char_manipulation]") {
    REQUIRE(dnd::char_to_lowercase('A') == 'a');
    REQUIRE(dnd::char_to_lowercase('Z') == 'z');
    REQUIRE(dnd::char_to_lowercase('a') == 'a');
    REQUIRE(dnd::char_to_lowercase('z') == 'z');
    REQUIRE(dnd::char_to_lowercase('0') == '0');
    REQUIRE(dnd::char_to_lowercase('9') == '9');
    REQUIRE(dnd::char_to_lowercase('!') == '!');
    REQUIRE(dnd::char_to_lowercase(' ') == ' ');
    REQUIRE(dnd::char_to_lowercase('\n') == '\n');
    REQUIRE(dnd::char_to_lowercase('\t') == '\t');
    REQUIRE(dnd::char_to_lowercase('\0') == '\0');
}

TEST_CASE("dnd::uchar_to_uppercase", "[utils][char_manipulation]") {
    REQUIRE(dnd::uchar_to_uppercase('A') == 'A');
    REQUIRE(dnd::uchar_to_uppercase('Z') == 'Z');
    REQUIRE(dnd::uchar_to_uppercase('a') == 'A');
    REQUIRE(dnd::uchar_to_uppercase('z') == 'Z');
    REQUIRE(dnd::uchar_to_uppercase('0') == '0');
    REQUIRE(dnd::uchar_to_uppercase('9') == '9');
    REQUIRE(dnd::uchar_to_uppercase('!') == '!');
    REQUIRE(dnd::uchar_to_uppercase(' ') == ' ');
    REQUIRE(dnd::uchar_to_uppercase('\n') == '\n');
    REQUIRE(dnd::uchar_to_uppercase('\t') == '\t');
    REQUIRE(dnd::uchar_to_uppercase('\0') == '\0');
}

TEST_CASE("dnd::char_to_uppercase", "[utils][char_manipulation]") {
    REQUIRE(dnd::char_to_uppercase('A') == 'A');
    REQUIRE(dnd::char_to_uppercase('Z') == 'Z');
    REQUIRE(dnd::char_to_uppercase('a') == 'A');
    REQUIRE(dnd::char_to_uppercase('z') == 'Z');
    REQUIRE(dnd::char_to_uppercase('0') == '0');
    REQUIRE(dnd::char_to_uppercase('9') == '9');
    REQUIRE(dnd::char_to_uppercase('!') == '!');
    REQUIRE(dnd::char_to_uppercase(' ') == ' ');
    REQUIRE(dnd::char_to_uppercase('\n') == '\n');
    REQUIRE(dnd::char_to_uppercase('\t') == '\t');
    REQUIRE(dnd::char_to_uppercase('\0') == '\0');
}
