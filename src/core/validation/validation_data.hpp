#ifndef VALIDATION_DATA_HPP_
#define VALIDATION_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <filesystem>
#include <string>

namespace dnd {

class Errors;

class ValidationData {
public:
    virtual ~ValidationData() = default;
    std::strong_ordering operator<=>(const ValidationData&) const = default;

    std::string name;
    std::string description;
    std::filesystem::path source_path;
protected:
    ValidationData() = default;
};

Errors validate_name_description_and_source(const ValidationData& data);

} // namespace dnd

#endif // VALIDATION_DATA_HPP_
