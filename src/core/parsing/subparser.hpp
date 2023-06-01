#ifndef SUBPARSER_HPP_
#define SUBPARSER_HPP_

#include "dnd_config.hpp"

#include <filesystem>

#include "core/parsing/parsing_types.hpp"

namespace dnd {

/**
 * @brief A class that is parsing a small part of a content JSON.
 * A content file parser can use multiple subparsers to parse a content JSON file.
 */
class Subparser {
public:
    /**
     * @brief Constructs a subparser with the parsing type and filepath
     * @param type the type of content of the file that is being parsed
     * @param filepath the file that is being parsed
     */
    Subparser(ParsingType type, const std::filesystem::path& filepath) noexcept;
    virtual ~Subparser() noexcept = default;

    // the type of content of the file that is being parsed
    const ParsingType type;
    // the file that is being parsed
    const std::filesystem::path filepath;
};

inline Subparser::Subparser(ParsingType type, const std::filesystem::path& filepath) noexcept
    : type(type), filepath(filepath) {}

} // namespace dnd

#endif // SUBPARSER_HPP_
