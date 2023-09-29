#ifndef CLI_LAUNCHER_HPP_
#define CLI_LAUNCHER_HPP_

#include <core/content.hpp>
#include <core/output/output.hpp>

namespace dnd {

int launch(int argc, char** argv);
bool content_search(Content& content, Output* output);

} // namespace dnd

#endif // CLI_LAUNCHER_HPP_
