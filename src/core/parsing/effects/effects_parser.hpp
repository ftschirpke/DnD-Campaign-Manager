#ifndef EFFECTS_PARSER_HPP_
#define EFFECTS_PARSER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/parsing/parser.hpp>
#include <core/validation/effects/effects_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class EffectsParser : public Parser {
public:
    explicit EffectsParser(const std::filesystem::path& filepath) noexcept;
    Errors parse_into(nlohmann::ordered_json&& json, EffectsData& data) const;
private:
    Errors parse_activation_conditions_into(nlohmann::ordered_json& json, std::vector<ConditionData>& data) const;
    Errors parse_choices_into(nlohmann::ordered_json& json, std::vector<ChoiceData>& data) const;
    Errors parse_stat_changes_into(nlohmann::ordered_json& json, std::vector<StatChangeData>& data) const;
    Errors parse_action_holder_into(nlohmann::ordered_json& json, ActionHolderData& data) const;
    Errors parse_extra_spells_holder_into(nlohmann::ordered_json& json, ExtraSpellsHolderData& data) const;
    Errors parse_proficiency_holder_into(nlohmann::ordered_json& json, ProficiencyHolderData& data) const;
    Errors parse_riv_holder_into(nlohmann::ordered_json& json, RIVHolderData& data) const;
};

} // namespace dnd

#endif // EFFECTS_PARSER_HPP_
