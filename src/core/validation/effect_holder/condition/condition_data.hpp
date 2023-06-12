#ifndef CONDITION_DATA_HPP_
#define CONDITION_DATA_HPP_

#include <dnd_config.hpp>

#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class ConditionData : public ValidationSubdata {
public:
    ConditionData(const ValidationData* parent) noexcept;
    /**
     * @brief Validates the data
     * @return the errors that occured during validation
     */
    virtual Errors validate() const override;

    std::string condition_str;
};

} // namespace dnd

#endif // CONDITION_DATA_HPP_