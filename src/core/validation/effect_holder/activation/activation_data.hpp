#ifndef ACTIVATION_DATA_HPP_
#define ACTIVATION_DATA_HPP_

#include <dnd_config.hpp>

#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class ActivationData : public ValidationSubdata {
public:
    ActivationData(const ValidationData* parent) noexcept;
    /**
     * @brief Validates the data
     * @return the errors that occured during validation
     */
    virtual Errors validate() const override;

    std::string activation_str;
};

} // namespace dnd

#endif // ACTIVATION_DATA_HPP_
