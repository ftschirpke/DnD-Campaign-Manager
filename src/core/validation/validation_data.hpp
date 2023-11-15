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
    virtual Errors validate() const;
    virtual Errors validate_relations(const Content& content) const;

    std::string name;
    std::string description;
    std::filesystem::path source_path;
protected:
    ValidationData() = default;
};

} // namespace dnd

#endif // VALIDATION_DATA_HPP_
