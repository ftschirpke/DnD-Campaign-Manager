#ifndef VALIDATION_SUBDATA_HPP_
#define VALIDATION_SUBDATA_HPP_

#include <dnd_config.hpp>

#include <compare>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class Content;

class ValidationSubdata {
public:
    virtual ~ValidationSubdata() noexcept = default;
    std::strong_ordering operator<=>(const ValidationSubdata&) const = default;

    const ValidationData* get_parent() const noexcept;
protected:
    ValidationSubdata(const ValidationData* parent) noexcept;

    const ValidationData* parent;
};

} // namespace dnd

#endif // VALIDATION_SUBDATA_HPP_
