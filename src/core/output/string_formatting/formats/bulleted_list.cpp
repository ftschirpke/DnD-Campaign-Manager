#include <dnd_config.hpp>

#include "bulleted_list.hpp"

#include <string_view>
#include <vector>

#include <core/output/string_formatting/format_visitor.hpp>

dnd::BulletedList::BulletedList() noexcept : items({}) {}

void dnd::BulletedList::accept(const FormatVisitor& visitor) const { visitor.visit(*this); }

void dnd::BulletedList::add_item(std::string_view item) { items.push_back(item); }

std::vector<std::string_view> dnd::BulletedList::get_items() const noexcept { return items; }
