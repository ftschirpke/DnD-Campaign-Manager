#ifndef SPELL_COMPONENTS_DATA_HPP_
#define SPELL_COMPONENTS_DATA_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class SpellComponentsData : public ValidationSubdata {
public:
    SpellComponentsData(const ValidationData* parent) noexcept;
    virtual Errors validate() const override;

    std::string str;
};

} // namespace dnd

#endif // SPELL_COMPONENTS_DATA_HPP_
