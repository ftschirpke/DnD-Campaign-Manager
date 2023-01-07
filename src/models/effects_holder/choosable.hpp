#ifndef CHOOSABLE_HPP_
#define CHOOSABLE_HPP_

#include <memory>
#include <vector>

#include "models/character.hpp"
#include "models/effects_holder/prerequisite.hpp"

namespace dnd {

class Choosable {
public:
    std::vector<std::unique_ptr<Prerequisite>> prerequisites;
    bool isAllowed(const Character& character) const;
};

} // namespace dnd

#endif // CHOOSABLE_HPP_
