#ifndef IDENTIFIER_STAT_CHANGE_HPP_
#define IDENTIFIER_STAT_CHANGE_HPP_

#include <dnd_config.hpp>

#include <string>
#include <string_view>
#include <unordered_map>

#include <core/models/effects/stat_change/stat_change.hpp>

namespace dnd {

/**
 * @brief A stat change that uses a value identifier to find the value to use in the operation
 */
class IdentifierStatChange : public StatChange {
public:
    /**
     * @brief Constructs a stat change with the attribute it affects, its execution time, and the name and value
     * identifier for the operation.
     * CAREFUL: if the operation is not found, the construction doesn't fail, but this stat change will do nothing.
     * @param affected_attribute the name of the attribute whose calculation is affected by this stat change
     * @param time the time at which this stat change should be applied in the order of execution
     * @param operation_name the name of the mathematical operation that is performed when applying this stat change
     * @param value_identifier the identifier for the value that is used in the operation
     */
    IdentifierStatChange(
        const std::string& affected_attribute, StatChangeTime time, const std::string& operation_name,
        const std::string& value_identifier
    ) noexcept;
    /**
     * @brief Constructs a stat change with the attribute it affects, its execution time, and the name and value
     * identifier for the operation.
     * CAREFUL: if the operation is not found, the construction doesn't fail, but this stat change will do nothing.
     * @param affected_attribute the name of the attribute whose calculation is affected by this stat change
     * @param time the time at which this stat change should be applied in the order of execution
     * @param operation_name the name of the mathematical operation that is performed when applying this stat change
     * @param value_identifier the identifier for the value that is used in the operation
     */
    IdentifierStatChange(
        std::string_view affected_attribute, StatChangeTime time, std::string_view operation_name,
        std::string_view value_identifier
    ) noexcept;
    /**
     * @brief Applies the stat change to a character's attributes given the attributes and constants of the character
     * @param attributes character attributes may be used for calculation and one of them will be changed
     * @param constants character constants may be used for calculation
     * @throws std::out_of_range if no value is found for the value identifier
     */
    virtual void apply_to(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const override final;
private:
    std::string value_identifier;
};

} // namespace dnd

#endif // IDENTIFIER_STAT_CHANGE_HPP_
