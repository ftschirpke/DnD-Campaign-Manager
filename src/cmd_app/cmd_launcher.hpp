#ifndef CMD_LAUNCHER_HPP_
#define CMD_LAUNCHER_HPP_

#include <core/controllers/content_holder.hpp>
#include <core/output/output.hpp>

namespace dnd {

int launch(int argc, char** argv);
bool content_search(ContentHolder& content, Output* output);

} // namespace dnd

#endif // CMD_LAUNCHER_HPP_
