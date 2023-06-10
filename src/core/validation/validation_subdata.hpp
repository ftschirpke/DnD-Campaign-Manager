#ifndef VALIDATION_SUBDATA_HPP_
#define VALIDATION_SUBDATA_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

/**
 * @brief A base class for the validation of data for parts of content pieces.
 */
class ValidationSubdata {
public:
    virtual ~ValidationSubdata() = default;
    virtual Errors validate() const = 0;
protected:
    ValidationSubdata(const ValidationData* parent);

    const ValidationData* parent;
};

} // namespace dnd

#endif // VALIDATION_SUBDATA_HPP_
