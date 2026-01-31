#ifndef FILE_PARSER_HPP_
#define FILE_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/parsing/parser.hpp>
#include <core/types.hpp>
#include <x/content_pieces.hpp>

namespace dnd {

class Content;

class FileParser : public Parser {
public:
    explicit FileParser(const std::filesystem::path& filepath, bool multiple_pieces_per_file);
    Errors open_json();
    virtual Errors parse() = 0;
    bool continue_after_errors() const;
    virtual void set_context(const Content& content);
    virtual void save_result(Content& content) = 0;
protected:
    nlohmann::ordered_json json;
    bool multiple_pieces_per_file;
};

class FoundryFileParser {
public:
    explicit FoundryFileParser(Opt<CRef<std::filesystem::path>> filepath);
    Errors open_json();
#define DECL_GET_OBJ_ENTRY(C, U, j, a, p, P) Opt<CRef<nlohmann::json>> get_##j##_entry(const std::string& key) const;
    X_CONTENT_PIECES(DECL_GET_OBJ_ENTRY)
#undef DECL_GET_OBJ_ENTRY
private:
    Opt<CRef<std::filesystem::path>> filepath;
    nlohmann::json json;
};


} // namespace dnd

#endif // FILE_PARSER_HPP_
