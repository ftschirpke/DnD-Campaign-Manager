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
    StringFilter();
    bool is_set() const;
    StringFilterType get_type() const;
    std::string& get_value_mutable();
    const std::string& get_value() const;
    void set_type(StringFilterType new_type);
    void set_value(const std::string& new_value);
    void set_value(std::string&& new_value);
    void set(StringFilterType new_type, const std::string& new_value);
    void set(StringFilterType new_type, std::string&& new_value);
    void clear();
    bool matches(const std::string& str) const;
private:
    StringFilterType type;
    std::string value;
};

} // namespace dnd

#endif // STRING_FILTERING_HPP_
