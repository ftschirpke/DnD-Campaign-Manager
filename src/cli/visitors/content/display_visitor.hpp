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
    void visit(const Character& character) override;
    void visit(const CharacterClass& character_class) override;
    void visit(const CharacterSubclass& character_subclass) override;
    void visit(const CharacterRace& character_race) override;
    void visit(const CharacterSubrace& character_subrace) override;
    void visit(const Item& item) override;
    void visit(const Spell& spell) override;
    void visit(const Feature& feature) override;
    void visit(const Choosable& choosable) override;
private:
    CommandLineOutput output;
};

} // namespace dnd

#endif // DISPLAY_VISITOR_HPP_
