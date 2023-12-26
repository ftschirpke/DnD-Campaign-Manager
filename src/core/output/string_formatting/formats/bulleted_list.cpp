#include <dnd_config.hpp>

#include "bulleted_list.hpp"

#include <string_view>
#include <vector>

#include <core/output/string_formatting/format_visitor.hpp>

namespace dnd {

BulletedList::BulletedList() noexcept : items({}) {}

void BulletedList::add_item(std::string_view item) { items.push_back(item); }

std::vector<std::string_view> BulletedList::get_items() const noexcept { return items; }

void BulletedList::accept(const FormatVisitor& visitor) const { visitor(*this); }

} // namespace dnd
