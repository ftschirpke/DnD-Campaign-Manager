#ifndef EFFECT_DATA_HPP_
#define EFFECT_DATA_HPP_

#include <dnd_config.hpp>

#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class EffectData : public ValidationSubdata {
public:
    EffectData(const ValidationData* parent) noexcept;
    /**
     * @brief Validates the data
     * @return the errors that occured during validation
     */
    virtual Errors validate() const override;

    std::string effect_str;
};

} // namespace dnd

#endif // EFFECT_DATA_HPP_
