#include <dnd_config.hpp>

#include "paragraph.hpp"

#include <string_view>

#include <core/output/string_formatting/format_visitor.hpp>

namespace dnd {

Paragraph::Paragraph(std::string_view text, bool empty_line_after)
    : text(text), empty_line_after(empty_line_after) {}

std::string_view Paragraph::get_text() const { return text; }

bool Paragraph::get_empty_line_after() const { return empty_line_after; }

void Paragraph::accept(const FormatVisitor& visitor) const { visitor(*this); }

} // namespace dnd
