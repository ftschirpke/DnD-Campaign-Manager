#include <dnd_config.hpp>

#include "source_info.hpp"

#include <filesystem>
#include <string>

namespace dnd {

static std::string nth_parent_stem(const std::filesystem::path& path, int n) {
    std::filesystem::path parent = path;
    for (int i = 0; i < n; ++i) {
        parent = parent.parent_path();
    }
    return parent.stem().string();
}

static std::string beautify_source_path(const std::filesystem::path& source_path) {
    std::filesystem::path relative_path = std::filesystem::relative(
        source_path, source_path.parent_path().parent_path()
    );
    relative_path.replace_extension();
    return relative_path.string();
}

SourceInfo::SourceInfo(const std::filesystem::path& source_path)
    : source_path(source_path), beautified_source_path(), source_book(nth_parent_stem(source_path, 3) == "general"),
      source_group_name(nth_parent_stem(source_path, 2)), source_type_name(nth_parent_stem(source_path, 1)),
      source_name(source_path.stem().string()) {
    beautified_source_path = beautify_source_path(this->source_path);
}

SourceInfo::SourceInfo(std::filesystem::path&& source_path)
    : source_path(std::move(source_path)), beautified_source_path(),
      source_book(nth_parent_stem(this->source_path, 3) == "general"),
      source_group_name(nth_parent_stem(this->source_path, 2)), source_type_name(nth_parent_stem(this->source_path, 1)),
      source_name(this->source_path.stem().string()) {
    beautified_source_path = beautify_source_path(this->source_path);
}

const std::filesystem::path& SourceInfo::get_source_path() const { return source_path; }

const std::string& SourceInfo::get_beautified_source_path() const { return beautified_source_path; }

bool SourceInfo::is_from_source_book() const { return source_book; }

const std::string& SourceInfo::get_source_group_name() const { return source_group_name; }

const std::string& SourceInfo::get_source_type_name() const { return source_type_name; }

const std::string& SourceInfo::get_source_name() const { return source_name; }

} // namespace dnd
