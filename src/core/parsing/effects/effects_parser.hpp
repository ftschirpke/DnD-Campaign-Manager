#ifndef EFFECTS_PARSER_HPP_
#define EFFECTS_PARSER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/models/effects/choice/choice.hpp>
#include <core/models/effects/condition/condition.hpp>
#include <core/models/effects/effects.hpp>
#include <core/models/effects/stat_change/stat_change.hpp>
#include <core/models/effects/subholders/action_holder.hpp>
#include <core/models/effects/subholders/extra_spells_holder.hpp>
#include <core/models/effects/subholders/proficiency_holder.hpp>
#include <core/models/effects/subholders/riv_holder.hpp>
#include <core/parsing/parser.hpp>

namespace dnd {

class EffectsParser : public Parser {
public:
    explicit EffectsParser(const std::filesystem::path& filepath);
    Errors parse_into(nlohmann::ordered_json&& json, Effects::Data& data) const;
private:
    Errors parse_activation_conditions_into(nlohmann::ordered_json& json, std::vector<Condition::Data>& data) const;
    Errors parse_choices_into(nlohmann::ordered_json& json, std::vector<Choice::Data>& data) const;
    Errors parse_stat_changes_into(nlohmann::ordered_json& json, std::vector<StatChange::Data>& data) const;
    Errors parse_action_holder_into(nlohmann::ordered_json& json, ActionHolder::Data& data) const;
    Errors parse_extra_spells_holder_into(nlohmann::ordered_json& json, ExtraSpellsHolder::Data& data) const;
    Errors parse_proficiency_holder_into(nlohmann::ordered_json& json, ProficiencyHolder::Data& data) const;
    Errors parse_riv_holder_into(nlohmann::ordered_json& json, RIVHolder::Data& data) const;
};

} // namespace dnd

#endif // EFFECTS_PARSER_HPP_
