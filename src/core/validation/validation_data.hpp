#ifndef VALIDATION_DATA_HPP_
#define VALIDATION_DATA_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>

#include <core/errors/errors.hpp>

namespace dnd {

class ValidationData {
public:
    virtual Errors validate() const = 0;

    std::string name;
    std::string description;
    std::filesystem::path source_file_path;
};

} // namespace dnd

#endif // VALIDATION_DATA_HPP_
