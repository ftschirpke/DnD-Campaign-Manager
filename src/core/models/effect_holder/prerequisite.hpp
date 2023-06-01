#ifndef PREREQUISITE_HPP_
#define PREREQUISITE_HPP_

#include "dnd_config.hpp"

#include "core/models/effect_holder/activation.hpp"

namespace dnd {

/**
 * @brief A class representing a condition that a character needs to fulfill before being able to choose a certain
 * choosable i.e. a prerequisite
 */
class Prerequisite : public Activation {
public:
    virtual ~Prerequisite() noexcept = default;
    // TODO
};

} // namespace dnd

#endif // PREREQUISITE_HPP_
