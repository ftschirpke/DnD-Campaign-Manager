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

class StringFilter {
public:
    StringFilter() noexcept;
    bool is_set() const noexcept;
    StringFilterType get_type() const noexcept;
    std::string& get_value_mutable() noexcept;
    const std::string& get_value() const noexcept;
    void set_type(StringFilterType new_type) noexcept;
    void set_value(const std::string& new_value) noexcept;
    void set_value(std::string&& new_value) noexcept;
    void set(StringFilterType new_type, const std::string& new_value) noexcept;
    void set(StringFilterType new_type, std::string&& new_value) noexcept;
    void clear() noexcept;
    bool matches(const std::string& str) const noexcept;
private:
    StringFilterType type;
    std::string value;
};

} // namespace dnd

#endif // STRING_FILTERING_HPP_
