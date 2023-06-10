#ifndef VALIDATION_DATA_HPP_
#define VALIDATION_DATA_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>

#include <core/errors/errors.hpp>

namespace dnd {

/**
 * @brief A base class for the validation of data for content pieces.
 */
class ValidationData {
public:
    virtual ~ValidationData() = default;
    virtual Errors validate() const;

    std::string name;
    std::string description;
    std::filesystem::path source_path;
protected:
    ValidationData() = default;
};

} // namespace dnd

#endif // VALIDATION_DATA_HPP_
