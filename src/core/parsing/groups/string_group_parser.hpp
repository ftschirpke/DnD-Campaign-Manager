#ifndef STRING_GROUP_PARSER_HPP_
#define STRING_GROUP_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <set>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/parsing/file_parser.hpp>

namespace dnd {

class StringGroupParser : public FileParser {
public:
    explicit StringGroupParser(const std::filesystem::path& file_path);
    virtual Errors parse() override;
    virtual void save_result(Content& content) override;
private:
    Errors parse_subgroups(nlohmann::ordered_json& json, const std::string& parent);

    std::unordered_map<std::string, std::set<std::string>> members;
    std::unordered_map<std::string, std::set<std::string>> subgroups;
};

} // namespace dnd

#endif // STRING_GROUP_PARSER_HPP_
