#ifndef STRING_FILTERING_HPP_
#define STRING_FILTERING_HPP_

#include <dnd_config.hpp>

#include <string>

namespace dnd {

enum class StringFilterType {
    EQUAL,
    NOT_EQUAL,
    CONTAINS,
    NOT_CONTAINS,
    STARTS_WITH,
    NOT_STARTS_WITH,
    ENDS_WITH,
    NOT_ENDS_WITH,
    NONE,
};

/**
 * @brief A class representing a filter for strings.
 */
class StringFilter {
public:
    StringFilter() noexcept;

    /**
     * @brief Determines if the filter is set.
     * @return "true" if the filter is set, "false" otherwise
     * @details A filter is set if the type is not "NONE".
     */
    bool is_set() const noexcept;
    /**
     * @brief Get the type of the filter.
     * @return the type of the filter
     */
    StringFilterType get_type() const noexcept;
    /**
     * @brief Get the value of the filter.
     * @return the value of the filter
     */
    std::string& get_value() noexcept;
    /**
     * @brief Get the value of the filter.
     * @return the value of the filter
     */
    const std::string& get_value() const noexcept;
    /**
     * @brief Set the type of the filter.
     * @param new_type the type of the filter
     */
    void set_type(StringFilterType new_type) noexcept;
    /**
     * @brief Set the value of the filter.
     * @param new_value the value of the filter
     */
    void set_value(const std::string& new_value) noexcept;
    /**
     * @brief Set the value of the filter.
     * @param new_value the value of the filter
     */
    void set_value(std::string&& new_value) noexcept;
    /**
     * @brief Set the filter.
     * @param new_type the type of the filter
     * @param new_value the value of the filter
     */
    void set(StringFilterType new_type, const std::string& new_value) noexcept;
    /**
     * @brief Set the filter.
     * @param new_type the type of the filter
     * @param new_value the value of the filter
     */
    void set(StringFilterType new_type, std::string&& new_value) noexcept;
    /**
     * @brief Clears the filter.
     */
    void clear() noexcept;

    /**
     * @brief Determines if a string matches the filter.
     * @param str the string to check
     * @return "true" if the string matches the filter, "false" otherwise
     */
    bool matches(const std::string& str) const noexcept;
private:
    StringFilterType type;
    std::string value;
};

} // namespace dnd

#endif // STRING_FILTERING_HPP_
