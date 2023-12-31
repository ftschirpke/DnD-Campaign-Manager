#ifndef DECISION_DATA_HPP_
#define DECISION_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <map>
#include <string>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class CharacterData;
class Effects;

class DecisionData : public ValidationSubdata {
public:
    DecisionData(const CharacterData* parent, const Effects* target) noexcept;
    std::strong_ordering operator<=>(const DecisionData&) const noexcept = default;

    const CharacterData* get_character_data() const noexcept;
    const Effects* get_target() const noexcept;

    void set_target(const Effects* new_target) noexcept;

    std::string feature_name;
    std::map<std::string, std::vector<std::string>> selections;
private:
    const CharacterData* character_data;
    const Effects* target;
};

Errors validate_decision_for(const DecisionData& data, const Content& content);

} // namespace dnd

#endif // DECISION_DATA_HPP_
