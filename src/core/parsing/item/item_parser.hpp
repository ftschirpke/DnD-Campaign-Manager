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
    virtual Errors parse() override;
    virtual void save_result(Content& content) override;
private:
    std::vector<ItemData> item_data;
};

} // namespace dnd

#endif // ITEM_PARSER_HPP_
