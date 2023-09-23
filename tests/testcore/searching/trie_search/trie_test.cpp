#include <dnd_config.hpp>

#include <core/searching/trie_search/trie.hpp>

#include <unordered_set>

#include <catch2/catch_test_macros.hpp>

static constexpr const char* tags = "[core][searching][trie]";

TEST_CASE("dnd::Trie // Simple Trie with integer data", tags) {
    dnd::Trie<int> trie;
    int data1 = 1, data2 = 2, data3 = 3;

    trie.insert("hello", &data1);
    trie.insert("world", &data2);
    trie.insert("help", &data3);

    SECTION("Search function") {
        std::vector<int*> hello_result = trie.search("hello");
        REQUIRE(hello_result.size() == 1);
        REQUIRE(hello_result[0] == &data1);
        std::vector<int*> world_result = trie.search("world");
        REQUIRE(world_result.size() == 1);
        REQUIRE(world_result[0] == &data2);
        REQUIRE(trie.search("foo").size() == 0);
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

TEST_CASE("dnd::Trie // Simple Trie with custom struct data", tags) {
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
        std::vector<MyData*> hello_result = trie.search("hello");
        REQUIRE(hello_result.size() == 1);
        REQUIRE(hello_result[0] == &data1);
        std::vector<MyData*> world_result = trie.search("world");
        REQUIRE(world_result.size() == 1);
        REQUIRE(world_result[0] == &data2);
        std::vector<MyData*> foo_result = trie.search("foo");
        REQUIRE(foo_result.size() == 1);
        REQUIRE(foo_result[0] == &data4);
        REQUIRE(trie.search("nonexistent").size() == 0);
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
