#ifndef SOURCE_INFO_HPP_
#define SOURCE_INFO_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>

namespace dnd {

class SourceInfo {
public:
    SourceInfo(const std::filesystem::path& source_path) noexcept;
    SourceInfo(std::filesystem::path&& source_path) noexcept;

    const std::string& get_source_name() const noexcept;
    const std::filesystem::path& get_source_path() const noexcept;
    const std::string& get_beautified_source_path() const noexcept;
    bool is_from_source_book() const noexcept;
    /**
     * @brief Returns the name of the source group (source book name or campaign name)
     * @return name of the source book or campaign the content piece is part of
     */
    const std::string& get_source_group_name() const noexcept;
    /**
     * @brief Returns the name of the content piece source directory
     * @return the name of the content piece source directory
     */
    const std::string& get_source_type_name() const noexcept;
private:
    std::filesystem::path source_path;
    std::string beautified_source_path;
    bool source_book;
    std::string source_group_name;
    std::string source_type_name;
    std::string source_name;
};

} // namespace dnd

#endif // SOURCE_INFO_HPP_
