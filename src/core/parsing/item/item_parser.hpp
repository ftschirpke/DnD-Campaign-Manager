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
    virtual Errors validate(const Content& content) const override;
    virtual void save_result(Content& content) override;
private:
    size_t items_in_file;
    std::vector<ItemData> item_data;
    mutable std::vector<bool> item_data_valid;
};

} // namespace dnd

#endif // ITEM_PARSER_HPP_
