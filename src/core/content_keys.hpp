#ifndef CONTENT_KEYS_HPP_
#define CONTENT_KEYS_HPP_

#include <dnd_config.hpp>

#include <string>

#include <fmt/format.h>

namespace dnd {

// defines keying scheme for most types of content
std::string default_key(const std::string& content_name, const std::string& content_source_name);

// defines keying scheme for class features which requires more information because of possible name duplication
std::string class_feature_key(
    const std::string& feature_name, const std::string& feature_source_name, int feature_level
);

} // namespace dnd


#endif // CONTENT_KEYS_HPP_
