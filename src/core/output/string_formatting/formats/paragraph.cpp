#include <dnd_config.hpp>

#include "paragraph.hpp"

#include <string_view>

#include <core/output/string_formatting/format_visitor.hpp>

dnd::Paragraph::Paragraph(std::string_view text, bool empty_line_after) noexcept
    : text(text), empty_line_after(empty_line_after) {}

std::string_view dnd::Paragraph::get_text() const noexcept { return text; }

bool dnd::Paragraph::get_empty_line_after() const noexcept { return empty_line_after; }

void dnd::Paragraph::accept(const FormatVisitor& visitor) const { visitor.visit(*this); }
