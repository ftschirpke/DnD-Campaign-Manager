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
    void operator()(const CharacterClass& character_class) override;
    void operator()(const CharacterSubclass& character_subclass) override;
    void operator()(const CharacterSpecies& character_species) override;
    void operator()(const CharacterSubspecies& character_subspecies) override;
    void operator()(const Item& item) override;
    void operator()(const Spell& spell) override;
    void operator()(const Feature& feature) override;
    void operator()(const Choosable& choosable) override;
private:
    CommandLineOutput output;
};

} // namespace dnd

#endif // DISPLAY_VISITOR_HPP_
