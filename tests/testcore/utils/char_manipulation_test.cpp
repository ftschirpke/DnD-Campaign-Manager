#include <dnd_config.hpp>

#include <core/utils/char_manipulation.hpp>

#include <catch2/catch_test_macros.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][utils]";

TEST_CASE("uchar_to_lowercase", tags) {
    REQUIRE(uchar_to_lowercase('A') == 'a');
    REQUIRE(uchar_to_lowercase('Z') == 'z');
    REQUIRE(uchar_to_lowercase('a') == 'a');
    REQUIRE(uchar_to_lowercase('z') == 'z');
    REQUIRE(uchar_to_lowercase('0') == '0');
    REQUIRE(uchar_to_lowercase('9') == '9');
    REQUIRE(uchar_to_lowercase('!') == '!');
    REQUIRE(uchar_to_lowercase(' ') == ' ');
    REQUIRE(uchar_to_lowercase('\n') == '\n');
    REQUIRE(uchar_to_lowercase('\t') == '\t');
    REQUIRE(uchar_to_lowercase('\0') == '\0');
}

TEST_CASE("char_to_lowercase", tags) {
    REQUIRE(char_to_lowercase('A') == 'a');
    REQUIRE(char_to_lowercase('Z') == 'z');
    REQUIRE(char_to_lowercase('a') == 'a');
    REQUIRE(char_to_lowercase('z') == 'z');
    REQUIRE(char_to_lowercase('0') == '0');
    REQUIRE(char_to_lowercase('9') == '9');
    REQUIRE(char_to_lowercase('!') == '!');
    REQUIRE(char_to_lowercase(' ') == ' ');
    REQUIRE(char_to_lowercase('\n') == '\n');
    REQUIRE(char_to_lowercase('\t') == '\t');
    REQUIRE(char_to_lowercase('\0') == '\0');
}

TEST_CASE("uchar_to_uppercase", tags) {
    REQUIRE(uchar_to_uppercase('A') == 'A');
    REQUIRE(uchar_to_uppercase('Z') == 'Z');
    REQUIRE(uchar_to_uppercase('a') == 'A');
    REQUIRE(uchar_to_uppercase('z') == 'Z');
    REQUIRE(uchar_to_uppercase('0') == '0');
    REQUIRE(uchar_to_uppercase('9') == '9');
    REQUIRE(uchar_to_uppercase('!') == '!');
    REQUIRE(uchar_to_uppercase(' ') == ' ');
    REQUIRE(uchar_to_uppercase('\n') == '\n');
    REQUIRE(uchar_to_uppercase('\t') == '\t');
    REQUIRE(uchar_to_uppercase('\0') == '\0');
}

TEST_CASE("char_to_uppercase", tags) {
    REQUIRE(char_to_uppercase('A') == 'A');
    REQUIRE(char_to_uppercase('Z') == 'Z');
    REQUIRE(char_to_uppercase('a') == 'A');
    REQUIRE(char_to_uppercase('z') == 'Z');
    REQUIRE(char_to_uppercase('0') == '0');
    REQUIRE(char_to_uppercase('9') == '9');
    REQUIRE(char_to_uppercase('!') == '!');
    REQUIRE(char_to_uppercase(' ') == ' ');
    REQUIRE(char_to_uppercase('\n') == '\n');
    REQUIRE(char_to_uppercase('\t') == '\t');
    REQUIRE(char_to_uppercase('\0') == '\0');
}

} // namespace dnd::test
