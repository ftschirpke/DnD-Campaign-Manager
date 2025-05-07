#ifndef LATEX_OBJECT_HPP_
#define LATEX_OBJECT_HPP_

#include <dnd_config.hpp>

#include <string>

namespace dnd {

class LatexObject {
public:
    virtual ~LatexObject() = default;
    virtual std::string str() const = 0;
    virtual size_t text_size() const = 0;
};

} // namespace dnd

#endif // LATEX_OBJECT_HPP_
