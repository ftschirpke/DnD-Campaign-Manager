#ifndef COMMAND_LINE_OUTPUT_HPP_
#define COMMAND_LINE_OUTPUT_HPP_

#include "dnd_config.hpp"

#include "models/item.hpp"
#include "models/spell.hpp"
#include "output/output.hpp"

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
};

} // namespace dnd

#endif // COMMAND_LINE_OUTPUT_HPP_
