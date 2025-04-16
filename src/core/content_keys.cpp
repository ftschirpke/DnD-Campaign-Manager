#include <dnd_config.hpp>

#include "content_keys.hpp"

#include <string>

#include <fmt/format.h>

namespace dnd {

std::string default_key(const std::string& content_name, const std::string& content_source_name) {
    return fmt::format("{}|{}", content_name, content_source_name);
}

std::string class_feature_key(
    const std::string& feature_name, const std::string& feature_source_name, int feature_level
) {
    return fmt::format("{}|{}|{}", feature_name, feature_source_name, feature_level);
}

} // namespace dnd
