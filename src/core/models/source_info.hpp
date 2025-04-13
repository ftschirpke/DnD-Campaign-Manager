#ifndef SOURCE_INFO_HPP_
#define SOURCE_INFO_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>

namespace dnd {

struct SourceInfo {
    std::filesystem::path path;
    std::string name;
};

} // namespace dnd

#endif // SOURCE_INFO_HPP_
