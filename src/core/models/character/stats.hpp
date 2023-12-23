#ifndef STATS_HPP_
#define STATS_HPP_

#include <dnd_config.hpp>

#include <string>
#include <unordered_map>

namespace dnd {

class Stats {
public:
    static Stats create_default() noexcept;
    Stats() noexcept;

    bool is_complete() const noexcept;
    void derive_implied_values() noexcept;
private:
    std::unordered_map<std::string, int> constant_values;
    std::unordered_map<std::string, int> mutable_values;
};

} // namespace dnd

#endif // STATS_HPP_
