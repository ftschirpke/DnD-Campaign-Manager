#ifndef ITEMS_FILE_PARSER_HPP_
#define ITEMS_FILE_PARSER_HPP_

#include <dnd_config.hpp>

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/controllers/content_library.hpp>
#include <core/controllers/groups.hpp>
#include <core/models/item.hpp>
#include <core/parsing/content_file_parser.hpp>
#include <core/parsing/parsing_types.hpp>

namespace dnd {

/**
 * @brief A struct to save a item's information during parsing
 */
struct ItemParsingInfo {
    std::string name;
    bool requires_attunement;
    // a functional description of the item (how it works and what it does)
    std::string description;
    // a description of the item focusing on its purely cosmetic (non-functional) aspects
    std::string cosmetic_description;
};

/**
 * @brief A class for parsing items (multi-file)
 */
class ItemsFileParser : public ContentFileParser {
public:
    ItemsFileParser(
        const std::filesystem::path& filepath, StoringContentLibrary<const Item>& items, Groups& groups
    ) noexcept;
    /**
     * @brief Parses JSON file containing a collection of spells
     * @throws parsing_error if any error occured while trying to parse the content file
     * @throws nlohmann::json::out_of_range if any required attribute does not exist
     * @throws nlohmann::json::type_error if any of the parsed attributes have the wrong type
     */
    virtual void parse() override;
    /**
     * @brief Checks whether the parsed spells are valid
     * @return "true" if the spells are valid, "false" otherwise
     */
    virtual bool validate() const override;
    /**
     * @brief Saves the parsed spells
     */
    virtual void saveResult() override;
    /**
     * @brief Returns the type of content that this parser parses - spells
     * @return the type of content that this parser parses - spells
     */
    virtual constexpr ParsingType getType() const override { return type; };
protected:
    void createItem(std::string_view item_name, const nlohmann::json& item_json);
private:
    // the type of content that this parser parses - items
    static constexpr ParsingType type = ParsingType::ITEM;
    // the already-parsed items to add the parsed items to
    StoringContentLibrary<const Item>& items;
    // the already-parsed groups to add item-groups to
    Groups& groups;
    // the amount of items to be parsed in the current file
    size_t items_in_file;
    // the items already parsed from the current file
    std::vector<ItemParsingInfo> item_parsing_info;
    // a vector to keep track of which items in this file are valid
    mutable std::vector<bool> valid;
};

inline ItemsFileParser::ItemsFileParser(
    const std::filesystem::path& filepath, StoringContentLibrary<const Item>& items, Groups& groups
) noexcept
    : ContentFileParser(filepath), items(items), groups(groups) {}

} // namespace dnd

#endif // ITEMS_FILE_PARSER_HPP_
