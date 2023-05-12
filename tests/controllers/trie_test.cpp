#include "controllers/trie.hpp"

#include <catch2/catch_test_macros.hpp>

#include <unordered_set>

TEST_CASE("Simple Trie with integer data", "[controllers][trie]") {
    dnd::Trie<int> trie;
    int data1 = 1, data2 = 2, data3 = 3;

    trie.insert("hello", &data1);
    trie.insert("world", &data2);
    trie.insert("help", &data3);

    SECTION("Search function") {
        REQUIRE(trie.search("hello") == &data1);
        REQUIRE(trie.search("world") == &data2);
        REQUIRE(trie.search("foo") == nullptr);
    }

    SECTION("Search prefix function") {
        std::unordered_set<int*> prefix_results = trie.search_prefix("hel");
        REQUIRE(prefix_results.size() == 2);
        REQUIRE(prefix_results.contains(&data1));
        REQUIRE(prefix_results.contains(&data3));
    }
}

struct MyData {
    int value;
    std::string name;
};

TEST_CASE("Simple Trie with custom struct data", "[controllers][trie]") {
    dnd::Trie<MyData> trie;
    MyData data1 = {1, "one"};
    MyData data2 = {2, "two"};
    MyData data3 = {3, "three"};
    MyData data4 = {4, "four"};
    MyData data5 = {5, "five"};

    trie.insert("hello", &data1);
    trie.insert("world", &data2);
    trie.insert("help", &data3);
    trie.insert("foo", &data4);
    trie.insert("five", &data5);

    SECTION("Search function") {
        REQUIRE(trie.search("hello") == &data1);
        REQUIRE(trie.search("world") == &data2);
        REQUIRE(trie.search("foo") == &data4);
        REQUIRE(trie.search("nonexistent") == nullptr);
    }

    SECTION("Search prefix function") {
        std::unordered_set<MyData*> prefix_results1 = trie.search_prefix("h");
        REQUIRE(prefix_results1.size() == 2);
        REQUIRE(prefix_results1.contains(&data1));
        REQUIRE(prefix_results1.contains(&data3));

        std::unordered_set<MyData*> prefix_results2 = trie.search_prefix("wo");
        REQUIRE(prefix_results2.size() == 1);
        REQUIRE(prefix_results2.contains(&data2));

        std::unordered_set<MyData*> prefix_results3 = trie.search_prefix("f");
        REQUIRE(prefix_results3.size() == 2);
        REQUIRE(prefix_results3.contains(&data4));
        REQUIRE(prefix_results3.contains(&data5));

        std::unordered_set<MyData*> prefix_results4 = trie.search_prefix("fi");
        REQUIRE(prefix_results4.size() == 1);
        REQUIRE(prefix_results4.contains(&data5));

        std::unordered_set<MyData*> prefix_results5 = trie.search_prefix("nonexistent");
        REQUIRE(prefix_results5.empty());
    }
}
