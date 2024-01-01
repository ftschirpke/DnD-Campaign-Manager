#ifndef DECISION_DATA_HPP_
#define DECISION_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <map>
#include <string>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/utils/types.hpp>

namespace dnd {

class Content;
class Effects;
class CharacterData;

class DecisionData {
public:
    DecisionData(const Effects* target) noexcept;
    std::strong_ordering operator<=>(const DecisionData&) const noexcept = default;

    const Effects* get_target() const noexcept;
    void set_target(const Effects* new_target) noexcept;

    std::string feature_name;
    std::map<std::string, std::vector<std::string>> selections;
private:
    const Effects* target;
};

Errors validate_decision_for_character_and_content(
    const DecisionData& data, const CharacterData& character_data, const Content& content
);

} // namespace dnd

#endif // DECISION_DATA_HPP_
