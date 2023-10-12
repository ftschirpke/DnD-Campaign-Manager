#ifndef EFFECT_HPP_
#define EFFECT_HPP_

#include <dnd_config.hpp>

#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace dnd {

enum class EffectTime {
    // earliest - applied first, e.g. setting a new default value
    EARLIEST = 0,
    // early - applied early, e.g. multiplication that shouldn't affect added boni
    EARLY = 1,
    // normal - intended for normal additive boni e.g. -1, +1, +2 etc.
    NORMAL = 2,
    // late - applied late, e.g. multiplication that should affect added boni
    LATE = 3,
    // latest - applied latest, e.g. setting fixed value
    LATEST = 4
};

/**
 * @brief A class representing the effect a content piece, e.g. a feature, has on the calculation of an attribute
 */
class Effect {
public:
    virtual ~Effect() noexcept = default;
    /**
     * @brief Applies the effect to a character's attributes given the attributes and constants of the character
     * @param attributes character attributes may be used for calculation and one of them will be changed
     * @param constants character constants may be used for calculation
     */
    virtual void apply_to(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const = 0;

    /**
     * @brief Returns the name of the attribute this effect affects
     * @return the name of the attribute this effect affects
     */
    const std::string& get_affected_attribute() const noexcept;
    /**
     * @brief Returns the time at which this effect should be applied in the order of execution
     * @return the time at which this effect should be applied in the order of execution
     */
    EffectTime get_time() const noexcept;
protected:
    /**
     * @brief Constructs an Effect with the attribute it affects, its execution time, and the name of the operation.
     * CAREFUL: if the operation is not found, the construction doesn't fail
     * @param affected_attribute the name of the attribute whose calculation is affected by this effect
     * @param time the time at which this effect should be applied in the order of execution
     * @param operation_name the name of the mathematical operation that is performed when applying this effect
     */
    Effect(const std::string& affected_attribute, EffectTime time, const std::string& operation_name) noexcept;
    /**
     * @brief Constructs an Effect with the attribute it affects, its execution time, and the name of the operation.
     * CAREFUL: if the operation is not found, the construction doesn't fail
     * @param affected_attribute the name of the attribute whose calculation is affected by this effect
     * @param time the time at which this effect should be applied in the order of execution
     * @param operation_name the name of the mathematical operation that is performed when applying this effect
     */
    Effect(std::string_view affected_attribute, EffectTime time, std::string_view operation_name) noexcept;

    std::string affected_attribute;
    std::function<int(int, int)> mathematical_operation;
private:
    EffectTime time;
};

} // namespace dnd

#endif // EFFECT_HPP_
