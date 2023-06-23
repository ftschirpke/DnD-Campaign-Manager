#ifndef VALIDATION_DATA_HPP_
#define VALIDATION_DATA_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <memory>
#include <string>

namespace dnd {

class ContentHolder;
class Errors;

/**
 * @brief A base class for the validation of data for content pieces.
 */
class ValidationData {
public:
    virtual ~ValidationData() = default;
    int operator<=>(const ValidationData&) const = default;
    /**
     * @brief Packs the data into a ValidationData unique pointer
     * @return the packed data
     */
    virtual std::unique_ptr<ValidationData> pack() const = 0;
    /**
     * @brief Validates the data
     * @return the errors that occured during validation
     */
    virtual Errors validate() const;
    /**
     * @brief Validates the relations of the data to other content pieces with regard to a given content holder
     * @param content the content holder to validate the relations against
     * @return the errors that occured during validation
     */
    virtual Errors validate_relations(const ContentHolder& content) const;

    std::string name;
    std::string description;
    std::filesystem::path source_path;
protected:
    ValidationData() = default;
};

} // namespace dnd

#endif // VALIDATION_DATA_HPP_
