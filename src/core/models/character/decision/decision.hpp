#ifndef DECISION_HPP_
#define DECISION_HPP_

#include <dnd_config.hpp>

#include <core/models/character/character_without_data.hpp>
#include <core/models/effects/effects.hpp>
#include <core/utils/data_result.hpp>
#include <core/utils/types.hpp>

namespace dnd {

class Content;

/**
 * @brief A class that represents a character's decision for a certain choice required or provided by a feature
 */
class Decision {
public:
    struct Data;

    static CreateResult<Decision> create_for(
        Data&& data, const Character::Data& character_data, const Content& content
    );

    Decision(const Decision&) = delete;
    Decision& operator=(const Decision&) = delete;
    Decision(Decision&&) noexcept = default;
    Decision& operator=(Decision&&) noexcept = default;

    /**
     * @brief Returns the target of the decision i.e. the effects with the choice that the decision is for
     * @return the target of the decision
     */
    CRef<Effects> get_target() const;
    const Effects& get_effects() const;
private:
    Decision(CRef<Effects> target, Effects&& effects);

    CRef<Effects> target;
    Effects effects;
};

struct Decision::Data {
    Data(const Effects* target);
    std::strong_ordering operator<=>(const Data&) const = default;

    const Effects* get_target() const;
    void set_target(const Effects* new_target);

    std::string feature_name;
    std::map<std::string, std::vector<std::string>> selections;
private:
    const Effects* target;
};

inline Decision::Data::Data(const Effects* target) : target(target) {}

inline const Effects* Decision::Data::get_target() const { return target; }

inline void Decision::Data::set_target(const Effects* new_target) { target = new_target; }

} // namespace dnd

#endif // DECISION_HPP_
