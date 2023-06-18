#ifndef DECISION_HPP_
#define DECISION_HPP_

#include <dnd_config.hpp>

#include <core/models/effect_holder/effect_holder.hpp>
#include <core/validation/character/decision/decision_data.hpp>

namespace dnd {

class ContentHolder;

/**
 * @brief A class that represents a character's decision for a certain choice required or provided by a feature
 */
class Decision {
public:
    /**
     * @brief Constructs a decision from the given data and content
     * @param data the data to construct the decision from
     * @param content the content to use for the decision
     * @return the constructed decision
     * @throws dnd::invalid_data if the given data is invalid or is incompatible with the given content
     */
    static Decision create(DecisionData&& data, const ContentHolder& content);

    Decision(const Decision&) = delete;
    Decision& operator=(const Decision&) = delete;
    Decision(Decision&&) = default;
    Decision& operator=(Decision&&) = default;

    const EffectHolder& get_effects() const noexcept;
private:
    Decision(const EffectHolder* target, EffectHolder effects) noexcept;

    const EffectHolder* target;
    EffectHolder effects;
};

} // namespace dnd

#endif // DECISION_HPP_
