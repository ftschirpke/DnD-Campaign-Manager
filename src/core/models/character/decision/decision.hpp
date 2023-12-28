#ifndef DECISION_HPP_
#define DECISION_HPP_

#include <dnd_config.hpp>

#include <core/models/effects/effects.hpp>
#include <core/validation/character/decision/decision_data.hpp>

namespace dnd {

class Content;

/**
 * @brief A class that represents a character's decision for a certain choice required or provided by a feature
 */
class Decision {
public:
    using Data = DecisionData;

    /**
     * @brief Constructs a decision from the given data and content
     * @param data the data to construct the decision from
     * @param content the content to use for the decision
     * @return the constructed decision
     * @throws invalid_data if the given data is invalid or is incompatible with the given content
     */
    static Decision create_for(Data&& data, const Content& content);

    Decision(const Decision&) = delete;
    Decision& operator=(const Decision&) = delete;
    Decision(Decision&&) = default;
    Decision& operator=(Decision&&) = default;

    /**
     * @brief Returns the target of the decision i.e. the choice that the decision is for
     * @return the target of the decision
     */
    const Effects* get_target() const noexcept;
    /**
     * @brief Returns the effects of the decision
     * @return the effects of the decision
     */
    const Effects& get_effects() const noexcept;
private:
    Decision(const Effects* target, Effects effects) noexcept;

    const Effects* target;
    Effects effects;
};

} // namespace dnd

#endif // DECISION_HPP_
