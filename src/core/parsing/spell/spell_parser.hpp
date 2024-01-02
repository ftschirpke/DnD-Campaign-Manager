#ifndef SPELL_PARSER_HPP_
#define SPELL_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/models/spell/spell.hpp>
#include <core/parsing/file_parser.hpp>

namespace dnd {

class SpellParser : public FileParser {
public:
    explicit SpellParser(const std::filesystem::path& file_path);
    virtual Errors parse() override;
    virtual void save_result(Content& content) override;
private:
    std::vector<Spell::Data> spell_data;
};

} // namespace dnd

#endif // SPELL_PARSER_HPP_
