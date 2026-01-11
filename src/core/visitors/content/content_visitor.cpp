#include <dnd_config.hpp>

#include "content_visitor.hpp"

#include <variant>

#include <core/content.hpp>

#include <fmt/format.h>

namespace dnd {

void ContentVisitor::visit_variant(const ContentPieceVariant& variant) {
    std::visit([this](const auto& piece) { visit(piece); }, variant);
}

} // namespace dnd
