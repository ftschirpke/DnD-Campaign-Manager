#ifndef DECISION_HPP_
#define DECISION_HPP_

#include <dnd_config.hpp>

#include <core/models/effects/effects.hpp>
#include <core/utils/data_result.hpp>
#include <core/validation/character/character_data.hpp>
#include <core/validation/character/decision/decision_data.hpp>

namespace dnd {

class Content;

/**
 * @brief A class that represents a character's decision for a certain choice required or provided by a feature
 */
class Decision {
public:
    using Data = DecisionData;

    static CreateResult<Decision> create_for(Data&& data, const CharacterData& character_data, const Content& content);

    Decision(const Decision&) = delete;
    Decision& operator=(const Decision&) = delete;
    Decision(Decision&&) = default;
    Decision& operator=(Decision&&) = default;

    /**
     * @brief Returns the target of the decision i.e. the choice that the decision is for
     * @return the target of the decision
     */
    CRef<Effects> get_target() const noexcept;
    /**
     * @brief Returns the effects of the decision
     * @return the effects of the decision
     */
    const Effects& get_effects() const noexcept;
private:
    Decision(CRef<Effects> target, Effects&& effects) noexcept;

    CRef<Effects> target;
    Effects effects;
};

} // namespace dnd

#endif // DECISION_HPP_
