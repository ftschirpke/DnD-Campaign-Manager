#ifndef CMD_LAUNCHER_HPP_
#define CMD_LAUNCHER_HPP_

#include <memory>

#include <cxxopts.hpp>

#include "controllers/content_holder.hpp"
#include "output/output.hpp"

namespace dnd {

int launchCMD(cxxopts::ParseResult args);
bool content_search(ContentHolder& content, Output* output);

} // namespace dnd

#endif // CMD_LAUNCHER_HPP_
