#ifndef LITERAL_STAT_CHANGE_HPP_
#define LITERAL_STAT_CHANGE_HPP_

#include <dnd_config.hpp>

#include <string>
#include <string_view>
#include <unordered_map>

#include <core/models/effects/stat_change/stat_change.hpp>

namespace dnd {

/**
 * @brief A class that represents a stat change that modifies a value by a literal (boolean, integer, or float)
 */
class LiteralStatChange : public StatChange {
public:
    /**
     * @brief Constructs a stat change with the attribute it affects, its execution time, and the name and boolean value
     * for the operation.
     * CAREFUL: if the operation is not found, the construction doesn't fail, but this stat change will do nothing.
     * @param affected_attribute the name of the attribute whose calculation is affected by this stat change
     * @param time the time at which this stat change should be applied in the order of execution
     * @param operation_name the name of the mathematical operation that is performed when applying this stat change
     * @param value the boolean value that is used in the operation
     */
    LiteralStatChange(
        const std::string& affected_attribute, StatChangeTime time, const std::string& operation_name, bool value
    ) noexcept;
    /**
     * @brief Constructs a stat change with the attribute it affects, its execution time, and the name and boolean value
     * for the operation.
     * CAREFUL: if the operation is not found, the construction doesn't fail, but this stat change will do nothing.
     * @param affected_attribute the name of the attribute whose calculation is affected by this stat change
     * @param time the time at which this stat change should be applied in the order of execution
     * @param operation_name the name of the mathematical operation that is performed when applying this stat change
     * @param value the boolean value that is used in the operation
     */
    LiteralStatChange(
        std::string_view affected_attribute, StatChangeTime time, std::string_view operation_name, bool value
    ) noexcept;
    /**
     * @brief Constructs a stat change with the attribute it affects, its execution time, and the name and integer value
     * for the operation.
     * CAREFUL: if the operation is not found, the construction doesn't fail, but this stat change will do nothing.
     * @param affected_attribute the name of the attribute whose calculation is affected by this stat change
     * @param time the time at which this stat change should be applied in the order of execution
     * @param operation_name the name of the mathematical operation that is performed when applying this stat change
     * @param value the integer value that is used in the operation
     */
    LiteralStatChange(
        const std::string& affected_attribute, StatChangeTime time, const std::string& operation_name, int value
    ) noexcept;
    /**
     * @brief Constructs a stat change with the attribute it affects, its execution time, and the name and integer value
     * for the operation.
     * CAREFUL: if the operation is not found, the construction doesn't fail, but this stat change will do nothing.
     * @param affected_attribute the name of the attribute whose calculation is affected by this stat change
     * @param time the time at which this stat change should be applied in the order of execution
     * @param operation_name the name of the mathematical operation that is performed when applying this stat change
     * @param value the integer value that is used in the operation
     */
    LiteralStatChange(
        std::string_view affected_attribute, StatChangeTime time, std::string_view operation_name, int value
    ) noexcept;
    /**
     * @brief Constructs a stat change with the attribute it affects, its execution time, and the name and float value
     * for the operation.
     * CAREFUL: if the operation is not found, the construction doesn't fail, but this stat change will do nothing.
     * @param affected_attribute the name of the attribute whose calculation is affected by this stat change
     * @param time the time at which this stat change should be applied in the order of execution
     * @param operation_name the name of the mathematical operation that is performed when applying this stat change
     * @param value the float value that is used in the operation
     */
    LiteralStatChange(
        const std::string& affected_attribute, StatChangeTime time, const std::string& operation_name, float value
    ) noexcept;
    /**
     * @brief Constructs a stat change with the attribute it affects, its execution time, and the name and float value
     * for the operation.
     * CAREFUL: if the operation is not found, the construction doesn't fail, but this stat change will do nothing.
     * @param affected_attribute the name of the attribute whose calculation is affected by this stat change
     * @param time the time at which this stat change should be applied in the order of execution
     * @param operation_name the name of the mathematical operation that is performed when applying this stat change
     * @param value the float value that is used in the operation
     */
    LiteralStatChange(
        std::string_view affected_attribute, StatChangeTime time, std::string_view operation_name, float value
    ) noexcept;
    /**
     * @brief Applies the stat change to a character's attributes given the attributes and constants of the character
     * @param attributes character attributes may be used for calculation and one of them will be changed
     * @param constants character constants may be used for calculation
     */
    virtual void apply_to(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const override final;
private:
    int value;
};

} // namespace dnd

#endif // LITERAL_STAT_CHANGE_HPP_
