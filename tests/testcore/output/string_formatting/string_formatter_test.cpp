#include <core/output/string_formatting/string_formatter.hpp>

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <core/output/string_formatting/formats/bulleted_list.hpp>
#include <core/output/string_formatting/formats/format.hpp>
#include <core/output/string_formatting/formats/paragraph.hpp>
#include <core/output/string_formatting/formats/table.hpp>

namespace dnd::test {

TEST_CASE("StringFormatter::parse_formats // empty text") {
    StringFormatter formatter;
    std::string text = "";

    std::vector<std::unique_ptr<Format>> formats = formatter.parse_formats(text);

    REQUIRE(formats.empty());
}

TEST_CASE("StringFormatter::parse_formats // single paragraph") {
    StringFormatter formatter;
    std::string text = "This is a paragraph.";

    std::vector<std::unique_ptr<Format>> formats = formatter.parse_formats(text);

    REQUIRE(formats.size() == 1);
    REQUIRE(dynamic_cast<Paragraph*>(formats[0].get()) != nullptr);
}

TEST_CASE("StringFormatter::parse_formats // simple bulleted list") {
    StringFormatter formatter;
    std::string text = "- Item 1\n- Item 2\n- Item 3";

    std::vector<std::unique_ptr<Format>> formats = formatter.parse_formats(text);

    REQUIRE(formats.size() == 1);
    REQUIRE(dynamic_cast<BulletedList*>(formats[0].get()) != nullptr);
}

TEST_CASE("StringFormatter::parse_formats // simple table") {
    StringFormatter formatter;
    std::string text = "Header 1|Header 2\nCell 1|Cell 2";

    std::vector<std::unique_ptr<Format>> formats = formatter.parse_formats(text);

    REQUIRE(formats.size() == 1);
    Table* table = dynamic_cast<Table*>(formats[0].get());
    REQUIRE(table != nullptr);
    std::vector<std::vector<std::string_view>> expected_rows = {{"Header 1", "Header 2"}, {"Cell 1", "Cell 2"}};
    REQUIRE(table->get_rows() == expected_rows);
    REQUIRE(table->get_num_columns() == 2);
}

} // namespace dnd::test
