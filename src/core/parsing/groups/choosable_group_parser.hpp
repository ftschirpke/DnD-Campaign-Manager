#ifndef CHOOSABLE_GROUP_PARSER_HPP_
#define CHOOSABLE_GROUP_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/parsing/effects_provider/choosable_parser.hpp>
#include <core/parsing/file_parser.hpp>

namespace dnd {

class ChoosableGroupParser : public FileParser {
public:
    explicit ChoosableGroupParser(const std::filesystem::path& filepath);
    virtual Errors parse() override;
    virtual void save_result(Content& content) override;
protected:
    ChoosableParser choosable_parser;
    std::string group_name;
    std::vector<Choosable::Data> data;
};

} // namespace dnd

#endif // CHOOSABLE_GROUP_PARSER_HPP_
