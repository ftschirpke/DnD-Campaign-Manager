#ifndef BOOL_FILTER_HPP_
#define BOOL_FILTER_HPP_

#include <dnd_config.hpp>

namespace dnd {

enum class BoolFilterType {
    IS_TRUE,
    IS_FALSE,
    NONE,
};

class BoolFilter {
public:
    BoolFilter();
    bool is_set() const;
    BoolFilterType get_type() const;
    void set_type(BoolFilterType new_type);
    void set(BoolFilterType new_type);
    void clear();
    bool matches(bool boolean) const;
private:
    BoolFilterType type;
};

} // namespace dnd

#endif // BOOL_FILTER_HPP_
