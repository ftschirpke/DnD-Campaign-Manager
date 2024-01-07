#ifndef EFFECTS_PROVIDER_HPP_
#define EFFECTS_PROVIDER_HPP_

#include <dnd_config.hpp>

namespace dnd {

class Effects;

/**
 * @brief Interface for classes that provide effects e.g. features.
 */
class EffectsProvider {
public:
    virtual ~EffectsProvider() = default;
    virtual const Effects& get_main_effects() const noexcept = 0;
};

} // namespace dnd

#endif // EFFECTS_PROVIDER_HPP_
