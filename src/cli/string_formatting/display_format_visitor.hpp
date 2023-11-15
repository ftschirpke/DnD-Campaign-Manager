#ifndef DISPLAY_FORMAT_VISITOR_HPP_
#define DISPLAY_FORMAT_VISITOR_HPP_

#include <dnd_config.hpp>

#include <string_view>

#include <cli/output/command_line_output.hpp>
#include <core/output/string_formatting/formats/bulleted_list.hpp>
#include <core/output/string_formatting/formats/paragraph.hpp>
#include <core/output/string_formatting/formats/table.hpp>

namespace dnd {

class DisplayFormatVisitor : public FormatVisitor {
public:
    virtual void operator()(const BulletedList& bulleted_list) const override;
    virtual void operator()(const Paragraph& paragraph) const override;
    virtual void operator()(const Table& table) const override;
private:
    CommandLineOutput output;
};

} // namespace dnd

#endif // DISPLAY_FORMAT_VISITOR_HPP_
