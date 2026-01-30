#ifndef SPELL_SOURCES_FILE_PARSER_HPP_
#define SPELL_SOURCES_FILE_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <unordered_map>

#include <core/errors/errors.hpp>
#include <core/parsing/file_parser.hpp>

namespace dnd {

using SpellSources = std::unordered_map<std::string, std::unordered_map<std::string, std::set<std::string>>>;

class SpellSourcesFileParser : public FileParser {
public:
    explicit SpellSourcesFileParser(const std::filesystem::path& filepath);
    virtual Errors parse() override final;
    virtual void save_result(Content& content) override final;

    SpellSources spell_classes_by_source;
};

} // namespace dnd

#endif // SPELL_SOURCES_FILE_PARSER_HPP_
