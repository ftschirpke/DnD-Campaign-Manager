#ifndef RIV_HOLDER_VALIDATION_HPP_
#define RIV_HOLDER_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/models/effects/subholders/riv_holder.hpp>

namespace dnd {

class Content;

Errors validate_riv_holder_for_content(const RIVHolder::Data& data, const Content& content);

} // namespace dnd

#endif // RIV_HOLDER_VALIDATION_HPP_
