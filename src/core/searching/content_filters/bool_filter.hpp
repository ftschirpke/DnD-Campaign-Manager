#ifndef BOOL_FILTER_HPP_
#define BOOL_FILTER_HPP_

#include <dnd_config.hpp>

namespace dnd {

enum class BoolFilterType {
    IS_TRUE,
    IS_FALSE,
    NONE,
};

/**
 * @brief A class representing a filter for booleans.
 */
class BoolFilter {
public:
    BoolFilter() noexcept;

    /**
     * @brief Get the type of the filter.
     * @return the type of the filter
     */
    BoolFilterType get_type() const noexcept;
    /**
     * @brief Set the type of the filter.
     * @param type the type of the filter
     */
    void set_type(BoolFilterType type) noexcept;
    /**
     * @brief Set the filter.
     * @param type the type of the filter
     */
    void set(BoolFilterType type) noexcept;
    /**
     * @brief Clears the filter.
     */
    void clear() noexcept;

    /**
     * @brief Determines if a boolean matches the filter.
     * @param boolean the boolean to check
     * @return "true" if the boolean matches the filter, "false" otherwise
     */
    bool matches(bool boolean) const noexcept;
private:
    BoolFilterType type;
};

} // namespace dnd

#endif // BOOL_FILTER_HPP_
