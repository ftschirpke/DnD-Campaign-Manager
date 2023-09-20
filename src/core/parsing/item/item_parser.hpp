#ifndef ITEM_PARSER_HPP_
#define ITEM_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/validation/item/item_data.hpp>

namespace dnd {

class ItemParser : public FileParser {
public:
    explicit ItemParser(const std::filesystem::path& file_path);
    /**
     * @brief Parses the json into item data
     * @return the errors that occured while parsing
     */
    virtual Errors parse() override;
    /**
     * @brief Returns whether the parser should continue after an invalid parsing
     * @return "true" because there are multiple items in one file and some may still be valid
     */
    virtual bool continue_after_errors() const noexcept override;
    /**
     * @brief Validates the parsed item data using the given content
     * @param content the content to validate against
     * @return the errors that occured while validating
     */
    virtual Errors validate(const Content& content) const override;
    /**
     * @brief Saves the parsed item data into the given content
     * @param content the content to save the parsed item into
     */
    virtual void save_result(Content& content) override;
private:
    size_t items_in_file;
    std::vector<ItemData> item_data;
    mutable std::vector<bool> item_data_valid;
};

} // namespace dnd

#endif // ITEM_PARSER_HPP_
