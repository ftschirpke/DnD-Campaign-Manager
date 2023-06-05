#ifndef LATEX_OBJECT_HPP_
#define LATEX_OBJECT_HPP_

#include <dnd_config.hpp>

#include <string>

namespace dnd {

class LatexObject {
public:
    virtual ~LatexObject() = default;
    /**
     * @brief Creates the LaTeX string for this object
     * @return the LaTeX string
     */
    virtual std::string str() const = 0;
};

} // namespace dnd

#endif // LATEX_OBJECT_HPP_
