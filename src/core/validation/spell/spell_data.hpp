#ifndef SPELL_DATA_HPP_
#define SPELL_DATA_HPP_

#include <dnd_config.hpp>

#include <memory>
#include <set>
#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/spell/spell_components_data.hpp>
#include <core/validation/spell/spell_type_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class SpellData : public ValidationData {
public:
    SpellData() noexcept;
    int operator<=>(const SpellData&) const noexcept = default;
    /**
     * @brief Packs the data into a ValidationData unique pointer
     * @return the packed data
     */
    virtual std::unique_ptr<ValidationData> pack() const override;

    /**
     * @brief Validates the data
     * @return the errors that occured during validation
     */
    virtual Errors validate() const override;

    SpellComponentsData components_data;
    SpellTypeData type_data;

    std::string casting_time;
    std::string range;
    std::string duration;
    std::set<std::string> classes;
};

} // namespace dnd

#endif // SPELL_DATA_HPP_
