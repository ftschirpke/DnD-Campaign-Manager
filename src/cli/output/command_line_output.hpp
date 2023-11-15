#ifndef COMMAND_LINE_OUTPUT_HPP_
#define COMMAND_LINE_OUTPUT_HPP_

#include <dnd_config.hpp>

#include <fmt/format.h>

namespace dnd {

class CommandLineOutput {
public:
    void newline() const;
    void text(const char* text) const;
    void text(std::string_view text) const;
    template <typename... T>
    void formatted_text(std::string_view fmt, const T&... args) const;
    void error(const char* error_msg) const;
    void error(std::string_view error_msg) const;
    template <typename... T>
    void formatted_error(std::string_view fmt, const T&... args) const;
    void prompt_input(std::string_view prompt_msg, std::string& out) const;
};

template <typename... T>
void CommandLineOutput::formatted_text(std::string_view fmt, const T&... args) const {
    text(fmt::vformat(fmt, fmt::make_format_args(std::forward<const T&>(args)...)));
}

template <typename... T>
void CommandLineOutput::formatted_error(std::string_view fmt, const T&... args) const {
    error(fmt::vformat(fmt, fmt::make_format_args(std::forward<const T&>(args)...)));
}

} // namespace dnd

#endif // COMMAND_LINE_OUTPUT_HPP_
