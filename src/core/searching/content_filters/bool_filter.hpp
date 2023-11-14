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
    BoolFilter() noexcept;
    bool is_set() const noexcept;
    BoolFilterType get_type() const noexcept;
    void set_type(BoolFilterType new_type) noexcept;
    void set(BoolFilterType new_type) noexcept;
    void clear() noexcept;
    bool matches(bool boolean) const noexcept;
private:
    BoolFilterType type;
};

} // namespace dnd

#endif // BOOL_FILTER_HPP_
