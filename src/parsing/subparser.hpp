#ifndef SUBPARSER_HPP_
#define SUBPARSER_HPP_

#include "dnd_config.hpp"

#include <filesystem>
#include <stdexcept>

#include "parsing/parsing_types.hpp"

namespace dnd {

/**
 * @brief A class that is parsing a small part of a content JSON.
 * A content file parser can use multiple subparsers to parse a content JSON file.
 */
class Subparser {
public:
    Subparser() noexcept;
    /**
     * @brief Configure the parsing type and the file path for the subparser that will be used when throwing exceptions
     * for example.
     * @param type the type of content of the file that is being parsed
     * @param filepath the file that is being parsed
     */
    virtual void configure(ParsingType conf_type, const std::filesystem::path& conf_filepath) noexcept;
protected:
    // the type of content of the file that is being parsed
    ParsingType type;
    // the file that is being parsed
    std::filesystem::path filepath;
    /**
     * @brief Asserts whether the subparser was configured.
     * @throws std::logic_error if subparser is unconfigured
     */
    virtual void requiresConfiguration() const;
private:
    bool configured;
};

inline Subparser::Subparser() noexcept
    : type(ParsingType::CHARACTER /* just setting some inital value */), filepath(), configured(false) {}

inline void Subparser::configure(ParsingType conf_type, const std::filesystem::path& conf_filepath) noexcept {
    type = conf_type;
    filepath = conf_filepath;
    configured = true;
}

inline void Subparser::requiresConfiguration() const {
    if (!configured) {
        throw std::logic_error("Subparser was not configured. Call configure(type, path) first.");
    }
}

} // namespace dnd

#endif // SUBPARSER_HPP_
