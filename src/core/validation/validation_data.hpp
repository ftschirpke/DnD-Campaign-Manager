#ifndef VALIDATION_DATA_HPP_
#define VALIDATION_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <filesystem>
#include <memory>
#include <string>

namespace dnd {

class Content;
class Errors;

class ValidationData {
public:
    virtual ~ValidationData() = default;
    std::strong_ordering operator<=>(const ValidationData&) const = default;
    virtual std::unique_ptr<ValidationData> pack() const = 0;

    std::string name;
    std::string description;
    std::filesystem::path source_path;
protected:
    ValidationData() = default;
};

Errors validate_name_description_and_source(const ValidationData& data);

} // namespace dnd

#endif // VALIDATION_DATA_HPP_
