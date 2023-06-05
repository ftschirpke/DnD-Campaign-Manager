#ifndef FORMAT_HPP_
#define FORMAT_HPP_

#include <dnd_config.hpp>

#include <core/output/string_formatting/format_visitor.hpp>

namespace dnd {

class Format {
public:
    virtual ~Format() = default;
    virtual void accept(FormatVisitor* visitor) = 0;
};

} // namespace dnd

#endif // FORMAT_HPP_
