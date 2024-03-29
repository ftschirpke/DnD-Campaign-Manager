#include <dnd_config.hpp>

#include <core/utils/string_manipulation.hpp>

#include <string>
#include <string_view>

#include <catch2/catch_test_macros.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][utils]";

TEST_CASE("string_lowercase_inplace", tags) {
    SECTION("Lowercase string") {
        std::string str1 = "hello world!";
        std::string str2 = "testing lowercase";
        string_lowercase_inplace(str1);
        string_lowercase_inplace(str2);
        REQUIRE(str1 == "hello world!");
        REQUIRE(str2 == "testing lowercase");
    }

    SECTION("Uppercase string") {
        std::string str1 = "HELLO WORLD!";
        std::string str2 = "TESTING UPPERCASE";
        string_lowercase_inplace(str1);
        string_lowercase_inplace(str2);
        REQUIRE(str1 == "hello world!");
        REQUIRE(str2 == "testing uppercase");
    }

    SECTION("Mixed case string") {
        std::string str1 = "Hello World!";
        std::string str2 = "MixED CaSe StRinG";
        string_lowercase_inplace(str1);
        string_lowercase_inplace(str2);
        REQUIRE(str1 == "hello world!");
        REQUIRE(str2 == "mixed case string");
    }

    SECTION("Empty string") {
        std::string str = "";
        string_lowercase_inplace(str);
        REQUIRE(str == "");
    }
}

TEST_CASE("string_uppercase_inplace", tags) {
    SECTION("Lowercase string") {
        std::string str1 = "hello world!";
        std::string str2 = "testing uppercase";
        string_uppercase_inplace(str1);
        string_uppercase_inplace(str2);
        REQUIRE(str1 == "HELLO WORLD!");
        REQUIRE(str2 == "TESTING UPPERCASE");
    }

    SECTION("Uppercase string") {
        std::string str1 = "HELLO WORLD!";
        std::string str2 = "TESTING UPPERCASE";
        string_uppercase_inplace(str1);
        string_uppercase_inplace(str2);
        REQUIRE(str1 == "HELLO WORLD!");
        REQUIRE(str2 == "TESTING UPPERCASE");
    }

    SECTION("Mixed case string") {
        std::string str1 = "Hello World!";
        std::string str2 = "MixED CaSe StRinG";
        string_uppercase_inplace(str1);
        string_uppercase_inplace(str2);
        REQUIRE(str1 == "HELLO WORLD!");
        REQUIRE(str2 == "MIXED CASE STRING");
    }

    SECTION("Empty string") {
        std::string str = "";
        string_uppercase_inplace(str);
        REQUIRE(str == "");
    }
}

TEST_CASE("string_lowercase_copy", tags) {
    SECTION("Lowercase string") {
        std::string str1 = "hello world!";
        std::string str2 = "testing lowercase";
        std::string lowercase1 = string_lowercase_copy(str1);
        std::string lowercase2 = string_lowercase_copy(str2);
        REQUIRE(lowercase1 == "hello world!");
        REQUIRE(lowercase2 == "testing lowercase");
        REQUIRE(str1 == "hello world!");
        REQUIRE(str2 == "testing lowercase");
    }

    SECTION("Uppercase string") {
        std::string str1 = "HELLO WORLD!";
        std::string str2 = "TESTING UPPERCASE";
        std::string lowercase1 = string_lowercase_copy(str1);
        std::string lowercase2 = string_lowercase_copy(str2);
        REQUIRE(lowercase1 == "hello world!");
        REQUIRE(lowercase2 == "testing uppercase");
        REQUIRE(str1 == "HELLO WORLD!");
        REQUIRE(str2 == "TESTING UPPERCASE");
    }

    SECTION("Mixed case string") {
        std::string str1 = "Hello World!";
        std::string str2 = "MixED CaSe StRinG";
        std::string lowercase1 = string_lowercase_copy(str1);
        std::string lowercase2 = string_lowercase_copy(str2);
        REQUIRE(lowercase1 == "hello world!");
        REQUIRE(lowercase2 == "mixed case string");
        REQUIRE(str1 == "Hello World!");
        REQUIRE(str2 == "MixED CaSe StRinG");
    }

    SECTION("Empty string") {
        std::string str = "";
        std::string lowercase = string_lowercase_copy(str);
        REQUIRE(lowercase == "");
        REQUIRE(str == "");
    }
}

TEST_CASE("string_uppercase_copy", tags) {
    SECTION("Lowercase string") {
        std::string str1 = "hello world!";
        std::string str2 = "testing uppercase";
        std::string uppercase1 = string_uppercase_copy(str1);
        std::string uppercase2 = string_uppercase_copy(str2);
        REQUIRE(uppercase1 == "HELLO WORLD!");
        REQUIRE(uppercase2 == "TESTING UPPERCASE");
        REQUIRE(str1 == "hello world!");
        REQUIRE(str2 == "testing uppercase");
    }

    SECTION("Uppercase string") {
        std::string str1 = "HELLO WORLD!";
        std::string str2 = "TESTING UPPERCASE";
        std::string uppercase1 = string_uppercase_copy(str1);
        std::string uppercase2 = string_uppercase_copy(str2);
        REQUIRE(uppercase1 == "HELLO WORLD!");
        REQUIRE(uppercase2 == "TESTING UPPERCASE");
        REQUIRE(str1 == "HELLO WORLD!");
        REQUIRE(str2 == "TESTING UPPERCASE");
    }

    SECTION("Mixed case string") {
        std::string str1 = "Hello World!";
        std::string str2 = "MixED CaSe StRinG";
        std::string uppercase1 = string_uppercase_copy(str1);
        std::string uppercase2 = string_uppercase_copy(str2);
        REQUIRE(uppercase1 == "HELLO WORLD!");
        REQUIRE(uppercase2 == "MIXED CASE STRING");
        REQUIRE(str1 == "Hello World!");
        REQUIRE(str2 == "MixED CaSe StRinG");
    }

    SECTION("Empty string") {
        std::string str = "";
        std::string uppercase = string_uppercase_copy(str);
        REQUIRE(uppercase == "");
        REQUIRE(str == "");
    }
}

TEST_CASE("snake_case_to_spaced_words", tags) {
    SECTION("Single word snake_case") {
        std::string str1 = "hello_world";
        std::string str2 = "testing_snake_";
        snake_case_to_capitalized_spaced_words(str1);
        snake_case_to_capitalized_spaced_words(str2);
        REQUIRE(str1 == "Hello World");
        REQUIRE(str2 == "Testing Snake ");
    }

    SECTION("Multiple word snake_case") {
        std::string str1 = "another_example_string";
        std::string str2 = "_some_long_snake_case_phrase";
        snake_case_to_capitalized_spaced_words(str1);
        snake_case_to_capitalized_spaced_words(str2);
        REQUIRE(str1 == "Another Example String");
        REQUIRE(str2 == " Some Long Snake Case Phrase");
    }

    SECTION("Empty string") {
        std::string str = "";
        snake_case_to_capitalized_spaced_words(str);
        REQUIRE(str == "");
    }
}

TEST_CASE("str_view // creating string views from iterators", tags) {
    std::string input1 = "Hello, world!";
    std::string input2 = "Testing str_view function";
    std::string::const_iterator first1 = input1.begin();
    std::string::const_iterator last1 = input1.end();
    std::string::const_iterator first2 = input2.begin();
    std::string::const_iterator last2 = input2.end();

    SECTION("String view of entire string") {
        std::string_view view1 = str_view(first1, last1);
        std::string_view view2 = str_view(first2, last2);
        REQUIRE(view1 == "Hello, world!");
        REQUIRE(view2 == "Testing str_view function");
    }

    SECTION("String view of partial string") {
        std::advance(first1, 7); // Move iterator to start at "world!"
        std::advance(first2, 9); // Move iterator to start at "str_view"
        std::string_view view1 = str_view(first1, last1);
        std::string_view view2 = str_view(first2, last2);
        REQUIRE(view1 == "world!");
        REQUIRE(view2 == "tr_view function");
    }

    SECTION("String view with different range") {
        first1 = input1.begin() + 2; // Start at "llo, world!"
        last1 = input1.end() - 1;    // Exclude the exclamation mark
        first2 = input2.begin() + 4; // Start at "ing"
        last2 = input2.begin() + 12; // End at "str_view"
        std::string_view view1 = str_view(first1, last1);
        std::string_view view2 = str_view(first2, last2);
        REQUIRE(view1 == "llo, world");
        REQUIRE(view2 == "ing str_");
    }

    SECTION("String view of empty range") {
        first1 = input1.begin() + 5;  // Start in the middle of the string
        last1 = input1.begin() + 5;   // Empty range
        first2 = input2.begin() + 20; // Start at "function"
        last2 = input2.begin() + 20;  // Empty range
        std::string_view view1 = str_view(first1, last1);
        std::string_view view2 = str_view(first2, last2);
        REQUIRE(view1.empty());
        REQUIRE(view2.empty());
    }
}

} // namespace dnd::test
