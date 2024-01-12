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
    std::filesystem::path content_path;
    std::string campaign_directory_name;
};

ParsingResult parse_content(const std::filesystem::path& content_path, const std::string& campaign_dir_name);

} // namespace dnd

#endif // CONTENT_PARSING
