#ifndef SPELL_FILE_PARSER_HPP_
#define SPELL_FILE_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <unordered_map>

#include <core/errors/errors.hpp>
#include <core/models/spell/spell.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/parsing/spell_sources_file_parser.hpp>

namespace dnd {

class Content;

class SpellFileParser : public FileParser {
public:
    explicit SpellFileParser(const std::filesystem::path& filepath, const SpellSources& spell_sources);
    virtual Errors parse();
    virtual void save_result(Content& content);
private:
    const SpellSources& spell_sources;
    std::unordered_map<std::string, Spell::Data> parsed_data;
};


} // namespace dnd

#endif // SPELL_FILE_PARSER_HPP_
