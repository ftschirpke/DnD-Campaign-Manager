#ifndef COMMAND_LINE_OUTPUT_HPP_
#define COMMAND_LINE_OUTPUT_HPP_

#include <dnd_config.hpp>

#include <fmt/format.h>

namespace dnd {

/**
 * @brief A class for displaying things on the command line.
 */
class CommandLineOutput {
public:
    /**
     * @brief Display a c-style string
     * @param text the c-style string to display
     */
    void text(const char* text);
    /**
     * @brief Display a string
     * @param text the string to display
     */
    void text(std::string_view text);
    /**
     * @brief Display a formatted text in the style of the fmt library.
     * @tparam ...T the types of the formatting arguments
     * @param fmt the formatting string
     * @param ...args the formatting arguments
     */
    template <typename... T>
    void formatted_text(std::string_view fmt, T&&... args);
    /**
     * @brief Display a c-style error message
     * @param error_msg the c-style error message
     */
    void error(const char* error_msg);
    /**
     * @brief Display a error message
     * @param error_msg the error message
     */
    void error(std::string_view error_msg);
    /**
     * @brief Display a formatted error message in the style of the fmt library.
     * @tparam ...T the types of the formatting arguments
     * @param fmt the formatting string
     * @param ...args the formatting arguments
     */
    template <typename... T>
    void formatted_error(std::string_view fmt, T&&... args);
    /**
     * @brief Ask user for input
     * @param prompt_msg the message asking for input
     * @param out the string to write the users input to
     */
    void prompt_input(std::string_view prompt_msg, std::string& out);
};

template <typename... T>
void CommandLineOutput::formatted_text(std::string_view fmt, T&&... args) {
    text(fmt::vformat(fmt, fmt::make_format_args(std::forward<T>(args)...)));
}

template <typename... T>
void CommandLineOutput::formatted_error(std::string_view fmt, T&&... args) {
    error(fmt::vformat(fmt, fmt::make_format_args(std::forward<T>(args)...)));
}

} // namespace dnd

#endif // COMMAND_LINE_OUTPUT_HPP_
