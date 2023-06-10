#ifndef COMMAND_LINE_OUTPUT_HPP_
#define COMMAND_LINE_OUTPUT_HPP_

#include <dnd_config.hpp>

#include <core/models/effect_holder/choosable.hpp>
#include <core/models/effect_holder/feature.hpp>
#include <core/models/item.hpp>
#include <core/models/spell.hpp>
#include <core/output/output.hpp>

namespace dnd {

/**
 * @brief A class for displaying things on the command line.
 */
class CommandLineOutput : public Output {
public:
    /**
     * @brief Display a c-style string
     * @param text the c-style string to display
     */
    virtual void text(const char* text) override;
    /**
     * @brief Display a string
     * @param text the string to display
     */
    virtual void text(std::string_view text) override;
    /**
     * @brief Display a c-style error message
     * @param error_msg the c-style error message
     */
    virtual void error(const char* error_msg) override;
    /**
     * @brief Display a error message
     * @param error_msg the error message
     */
    virtual void error(std::string_view error_msg) override;
    /**
     * @brief Display an item
     * @param item a pointer to the item
     */
    virtual void display(const Item* item) override;
    /**
     * @brief Display a spell
     * @param spell a pointer to the spell
     */
    virtual void display(const Spell* spell) override;
    /**
     * @brief Display a feature
     * @param feature a pointer to the feature
     */
    virtual void display(const Feature* feature) override;
    /**
     * @brief Display a choosable
     * @param choosable a pointer to the choosable
     */
    virtual void display(const Choosable* choosable) override;
    /**
     * @brief Ask user for input
     * @param prompt_msg the message asking for input
     * @param out the string to write the users input to
     */
    virtual void prompt_input(std::string_view prompt_msg, std::string& out) override;
};

} // namespace dnd

#endif // COMMAND_LINE_OUTPUT_HPP_
