#ifndef VALIDATION_SUBDATA_HPP_
#define VALIDATION_SUBDATA_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class ContentHolder;

/**
 * @brief A base class for the validation of data for parts of content pieces.
 */
class ValidationSubdata {
public:
    virtual ~ValidationSubdata() noexcept = default;

    /**
     * @brief Validates the data
     * @return the errors that occured during validation
     */
    virtual Errors validate() const = 0;
    /**
     * @brief Validates the relations of the data to other content pieces with regard to a given content holder
     * @param content the content holder to validate the relations against
     * @return the errors that occured during validation
     */
    virtual Errors validate_relations(const ContentHolder& content) const;

    const ValidationData* get_parent() const noexcept;
protected:
    ValidationSubdata(const ValidationData* parent) noexcept;

    const ValidationData* parent;
};

} // namespace dnd

#endif // VALIDATION_SUBDATA_HPP_
