#ifndef EFFECT_HOLDER_PARSER_HPP_
#define EFFECT_HOLDER_PARSER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/parsing/parser.hpp>
#include <core/validation/effect_holder/effect_holder_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class EffectHolderParser : public Parser {
public:
    /**
     * @brief Parses the data from the json into the data object
     * @param json the json to parse
     * @param data the data object to parse into
     * @return the errors that occured during parsing
     */
    Errors parse(nlohmann::ordered_json&& json, EffectHolderData& data) const;
private:
    Errors parse_activation_conditions(
        nlohmann::ordered_json& json, std::vector<ConditionData>& data, const ValidationData* parent
    ) const;
    Errors parse_choices(nlohmann::ordered_json& json, std::vector<ChoiceData>& data, const ValidationData* parent)
        const;
    Errors parse_effects(nlohmann::ordered_json& json, std::vector<EffectData>& data, const ValidationData* parent)
        const;
    Errors parse_action_holder(nlohmann::ordered_json& json, ActionHolderData& data) const;
    Errors parse_extra_spells_holder(nlohmann::ordered_json& json, ExtraSpellsHolderData& data) const;
    Errors parse_proficiency_holder(nlohmann::ordered_json& json, ProficiencyHolderData& data) const;
    Errors parse_riv_holder(nlohmann::ordered_json& json, RIVHolderData& data) const;
};

} // namespace dnd

#endif // EFFECT_HOLDER_PARSER_HPP_
