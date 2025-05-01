#include <dnd_config.hpp>

#include "content_piece.hpp"

#include <string>

#include <fmt/format.h>

namespace dnd {

std::string ContentPiece::key(const std::string& name, const std::string& source_name) {
    return fmt::format("{}##{}", name, source_name);
}

std::string ContentPiece::get_key() const { return key(get_name(), get_source_info().name); }

} // namespace dnd
