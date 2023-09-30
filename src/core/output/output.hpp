#ifndef OUTPUT_HPP_
#define OUTPUT_HPP_

#include <dnd_config.hpp>

#include <string_view>

#include <fmt/core.h>

#include <core/models/feature/choosable.hpp>
#include <core/models/feature/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/spell/spell.hpp>
namespace dnd {

/**
 * @brief An abstract class describing the interface of classes for displaying things to the user.
 */
class Output {
public:
    virtual ~Output() = default;
    /**
     * @brief Display a c-style string
     * @param text the c-style string to display
     */
    virtual void text(const char* text) = 0;
    /**
     * @brief Display a string
     * @param text the string to display
     */
    virtual void text(std::string_view text) = 0;
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
    virtual void error(const char* error_msg) = 0;
    /**
     * @brief Display a error message
     * @param error_msg the error message
     */
    virtual void error(std::string_view error_msg) = 0;
    /**
     * @brief Display a formatted error message in the style of the fmt library.
     * @tparam ...T the types of the formatting arguments
     * @param fmt the formatting string
     * @param ...args the formatting arguments
     */
    template <typename... T>
    void formatted_error(std::string_view fmt, T&&... args);
    /**
     * @brief Display an item
     * @param item a pointer to the item
     */
    virtual void display(const Item* item) = 0;
    /**
     * @brief Display a spell
     * @param spell a pointer to the spell
     */
    virtual void display(const Spell* spell) = 0;
    /**
     * @brief Display a feature
     * @param feature a pointer to the feature
     */
    virtual void display(const Feature* feature) = 0;
    /**
     * @brief Display a choosable
     * @param choosable a pointer to the choosable
     */
    virtual void display(const Choosable* choosable) = 0;
    /**
     * @brief Ask user for input
     * @param prompt_msg the message asking for input
     * @param out the string to write the users input to
     */
    virtual void prompt_input(std::string_view prompt_msg, std::string& out) = 0;
};

template <typename... T>
void Output::formatted_text(std::string_view fmt, T&&... args) {
    text(fmt::vformat(fmt, fmt::make_format_args(std::forward<T>(args)...)));
}

template <typename... T>
void Output::formatted_error(std::string_view fmt, T&&... args) {
    error(fmt::vformat(fmt, fmt::make_format_args(std::forward<T>(args)...)));
}


} // namespace dnd

#endif // OUTPUT_HPP_
