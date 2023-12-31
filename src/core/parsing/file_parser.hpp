#ifndef FILE_PARSER_HPP_
#define FILE_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/parsing/parser.hpp>

namespace dnd {

class Content;

class FileParser : public Parser {
public:
    explicit FileParser(const std::filesystem::path& filepath, bool multiple_pieces_per_file) noexcept;
    virtual Errors open_json() final;
    virtual Errors parse() = 0;
    bool continue_after_errors() const noexcept;
    virtual void set_context(const Content& content);
    virtual void save_result(Content& content) = 0;
protected:
    nlohmann::ordered_json json;
    bool multiple_pieces_per_file;
};

} // namespace dnd

#endif // FILE_PARSER_HPP_
