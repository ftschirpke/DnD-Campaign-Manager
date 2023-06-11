#ifndef EFFECT_FACTORY_HPP_
#define EFFECT_FACTORY_HPP_

#include <dnd_config.hpp>

#include <memory>

#include <core/models/effect_holder/effect/effect.hpp>
#include <core/validation/effect_holder/effect/effect_data.hpp>

namespace dnd {

/**
 * @brief Constructs an effect
 * @param effect_data the data to construct the effect from
 * @return the constructed effect
 * @throw dnd::invalid_data if the data is invalid
 */
std::unique_ptr<Effect> create_effect(EffectData&& effect_data);

} // namespace dnd

#endif // EFFECT_FACTORY_HPP_
