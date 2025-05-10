#ifndef LATEX_HPP_
#define LATEX_HPP_

#include <dnd_config.hpp>

#include <core/output/latex_builder/latex_scope.hpp>
#include <core/text/text.hpp>

namespace dnd {

std::vector<LatexScope> text_to_latex(const Text& text);

} // namespace dnd

#endif // LATEX_HPP_
