#ifndef DISPLAY_VISITOR_HPP_
#define DISPLAY_VISITOR_HPP_

#include <cli/output/command_line_output.hpp>
#include <cli/string_formatting/display_format_visitor.hpp>
#include <core/output/string_formatting/format_visitor.hpp>
#include <core/output/string_formatting/string_formatter.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

/**
 * @brief A visitor that displays the content using command line output.
 */
class DisplayVisitor : public ContentVisitor {
public:
    void operator()(const Character& character) override;
    void operator()(const Class& cls) override;
    void operator()(const Subclass& subclass) override;
    void operator()(const Species& species) override;
    void operator()(const Subspecies& subspecies) override;
    void operator()(const Item& item) override;
    void operator()(const Spell& spell) override;
    void operator()(const Feature& feature) override;
    void operator()(const Choosable& choosable) override;
private:
    CommandLineOutput output;
};

} // namespace dnd

#endif // DISPLAY_VISITOR_HPP_
