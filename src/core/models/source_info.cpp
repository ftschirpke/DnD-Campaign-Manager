#include <dnd_config.hpp>

#include "source_info.hpp"

#include <filesystem>
#include <string>

static std::string nth_parent_stem(const std::filesystem::path& path, int n) noexcept {
    std::filesystem::path parent = path;
    for (int i = 0; i < n; ++i) {
        parent = parent.parent_path();
    }
    return parent.stem().string();
}

dnd::SourceInfo::SourceInfo(const std::filesystem::path& source_path) noexcept
    : source_path(source_path), source_book(nth_parent_stem(source_path, 3) == "general"),
      source_group_name(nth_parent_stem(source_path, 2)), source_type_name(nth_parent_stem(source_path, 1)),
      source_name(source_path.stem().string()) {}

dnd::SourceInfo::SourceInfo(std::filesystem::path&& source_path) noexcept
    : source_path(std::move(source_path)), source_book(nth_parent_stem(source_path, 3) == "general"),
      source_group_name(nth_parent_stem(source_path, 2)), source_type_name(nth_parent_stem(source_path, 1)),
      source_name(source_path.stem().string()) {}

const std::filesystem::path& dnd::SourceInfo::get_source_path() const noexcept { return source_path; }

bool dnd::SourceInfo::is_from_source_book() const noexcept { return source_book; }

const std::string& dnd::SourceInfo::get_source_group_name() const noexcept { return source_group_name; }

const std::string& dnd::SourceInfo::get_source_type_name() const noexcept { return source_type_name; }

const std::string& dnd::SourceInfo::get_source_name() const noexcept { return source_name; }
