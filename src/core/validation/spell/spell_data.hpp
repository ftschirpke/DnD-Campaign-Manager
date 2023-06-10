#ifndef SPELL_DATA_HPP_
#define SPELL_DATA_HPP_

#include <dnd_config.hpp>

#include <set>
#include <string>

#include <core/errors/errors.hpp>
#include <core/models/spell/spell_components.hpp>
#include <core/models/spell/spell_type.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class SpellData : public ValidationData {
public:
    virtual Errors validate() const override;
    Errors validate_components() const;
    Errors validate_type() const;

    /**
     * @brief Creates a SpellComponents object from the components_str
     * @return the SpellComponents object
     * @throw dnd::invalid_data if the components_str is invalid
     */
    SpellComponents create_components() const;
    SpellType create_type() const;

    std::string components_str;
    std::string type_str;

    std::string casting_time;
    std::string range;
    std::string duration;
    std::set<std::string> classes;
};

} // namespace dnd

#endif // SPELL_DATA_HPP_
