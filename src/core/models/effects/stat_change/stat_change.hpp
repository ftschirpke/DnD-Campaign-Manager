#ifndef EFFECT_HPP_
#define EFFECT_HPP_

#include <dnd_config.hpp>

#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace dnd {

enum class StatChangeTime {
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

class StatChange {
public:
    struct Data;

    virtual ~StatChange() noexcept = default;

    StatChangeTime get_time() const noexcept;

    /**
     * @brief Applies the stat change to a character's attributes given the attributes and constants of the character
     * @param attributes character attributes may be used for calculation and one of them will be changed
     * @param constants character constants may be used for calculation
     */
    virtual void apply_to(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const = 0;
protected:
    /**
     * @brief Constructs a stat change with the attribute it affects, its execution time, and the name of the operation.
     * CAREFUL: if the operation is not found, the construction doesn't fail
     * @param affected_attribute the name of the attribute whose calculation is affected by this stat change
     * @param time the time at which this stat change should be applied in the order of execution
     * @param operation_name the name of the mathematical operation that is performed when applying this stat change
     */
    StatChange(const std::string& affected_attribute, StatChangeTime time, const std::string& operation_name) noexcept;
    /**
     * @brief Constructs an Effect with the attribute it affects, its execution time, and the name of the operation.
     * CAREFUL: if the operation is not found, the construction doesn't fail
     * @param affected_attribute the name of the attribute whose calculation is affected by this stat change
     * @param time the time at which this stat change should be applied in the order of execution
     * @param operation_name the name of the mathematical operation that is performed when applying this stat change
     */
    StatChange(std::string_view affected_attribute, StatChangeTime time, std::string_view operation_name) noexcept;

    std::string affected_attribute;
    std::function<int(int, int)> mathematical_operation;
private:
    StatChangeTime time;
};

struct StatChange::Data {
    std::strong_ordering operator<=>(const Data&) const noexcept = default;

    std::string stat_change_str;
};

} // namespace dnd

#endif // EFFECT_HPP_
