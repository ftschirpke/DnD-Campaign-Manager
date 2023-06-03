#ifndef DISPLAY_FORMAT_VISITOR_HPP_
#define DISPLAY_FORMAT_VISITOR_HPP_

#include "dnd_config.hpp"

#include <imgui/imgui.h>

#include "core/output/string_formatting/format_visitor.hpp"
#include "core/output/string_formatting/formats/bulleted_list.hpp"
#include "core/output/string_formatting/formats/paragraph.hpp"
#include "core/output/string_formatting/formats/table.hpp"

namespace dnd {

/**
 * @brief A visitor that displays the format elements using ImGui widgets
 */
class DisplayFormatVisitor : public FormatVisitor {
public:
    DisplayFormatVisitor(ImGuiTableFlags table_flags = 0);
    virtual void visit(BulletedList* bulleted_list) override;
    virtual void visit(Paragraph* paragraph) override;
    virtual void visit(Table* table) override;
private:
    ImGuiTableFlags table_flags;
};

inline DisplayFormatVisitor::DisplayFormatVisitor(ImGuiTableFlags table_flags) : table_flags(table_flags) {}

} // namespace dnd

#endif // DISPLAY_FORMAT_VISITOR_HPP_
