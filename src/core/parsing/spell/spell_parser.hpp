#ifndef SPELL_PARSER_HPP_
#define SPELL_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/validation/spell/spell_data.hpp>

namespace dnd {

class SpellParser : public FileParser {
public:
    explicit SpellParser(const std::filesystem::path& file_path);
    /**
     * @brief Parses the json into spell data
     * @return the errors that occured while parsing
     */
    virtual Errors parse() override;
    /**
     * @brief Returns whether the parser should continue after an invalid parsing
     * @return "true" because there are multiple spells in one file and some may still be valid
     */
    virtual bool continue_after_errors() const noexcept override;
    /**
     * @brief Validates the parsed spell data using the given content
     * @param content the content to validate against
     * @return the errors that occured while validating
     */
    virtual Errors validate(const Content& content) const override;
    /**
     * @brief Saves the parsed spell data into the given content
     * @param content the content to save the parsed spell into
     */
    virtual void save_result(Content& content) override;
private:
    size_t spells_in_file;
    std::vector<SpellData> spell_data;
    mutable std::vector<bool> spell_data_valid;
};

} // namespace dnd

#endif // SPELL_PARSER_HPP_
