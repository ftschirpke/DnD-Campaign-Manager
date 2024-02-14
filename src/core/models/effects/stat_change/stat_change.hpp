#ifndef EFFECT_HPP_
#define EFFECT_HPP_

#include <dnd_config.hpp>

#include <string>
#include <string_view>

#include <core/errors/errors.hpp>

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

enum class StatChangeOperation {
    ADD,
    SUB,
    MULT,
    DIV,
    SET,
    MAX,
    MIN,
};

class Stats;

class StatChange {
public:
    struct Data;

    virtual ~StatChange() = default;

    StatChangeTime get_time() const;
    const std::string& get_affected_attribute() const;

    virtual Errors apply(Stats& stats) const = 0;
protected:
    StatChange(const std::string& affected_attribute, StatChangeTime time, StatChangeOperation operation);
    StatChange(std::string_view affected_attribute, StatChangeTime time, StatChangeOperation operation);

    Errors apply_with_value(Stats& stats, int value) const;
private:
    std::string affected_attribute;
    StatChangeOperation operation;
    StatChangeTime time;
};

struct StatChange::Data {
    std::strong_ordering operator<=>(const Data&) const = default;

    std::string stat_change_str;
};

} // namespace dnd

#endif // EFFECT_HPP_
