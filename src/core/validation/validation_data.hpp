#ifndef VALIDATION_DATA_HPP_
#define VALIDATION_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <filesystem>
#include <string>

#include <fmt/format.h>

namespace dnd {

class Errors;

class ValidationData {
public:
    static std::string key(const std::string& name, const std::string& source_name);

    virtual ~ValidationData() = default;
    std::strong_ordering operator<=>(const ValidationData&) const = default;
    virtual std::string get_key() const;

    std::string name;
    std::string description;
    std::filesystem::path source_path;
    std::string source_name;
protected:
    ValidationData() = default;
};

Errors validate_name_description_and_source(const ValidationData& data);

} // namespace dnd

#endif // VALIDATION_DATA_HPP_
