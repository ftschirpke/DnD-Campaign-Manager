#ifndef CONTENT_PARSING
#define CONTENT_PARSING

#include <dnd_config.hpp>

#include <filesystem>
#include <string>

#include <core/content.hpp>
#include <core/errors/errors.hpp>

namespace dnd {

struct ParsingResult {
    ParsingResult() = default;
    ParsingResult(const ParsingResult&) = delete;
    ParsingResult& operator=(const ParsingResult&) = delete;
    ParsingResult(ParsingResult&&) noexcept = default;
    ParsingResult& operator=(ParsingResult&&) noexcept = default;

    Content content;
    Errors errors;
    std::set<std::filesystem::path> content_paths;
    std::string campaign_directory_name;
};

ParsingResult parse_content(const std::set<std::filesystem::path>& content_path);

} // namespace dnd

#endif // CONTENT_PARSING
