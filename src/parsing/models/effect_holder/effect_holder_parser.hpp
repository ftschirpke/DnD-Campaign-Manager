#ifndef EFFECT_HOLDER_PARSER_HPP_
#define EFFECT_HOLDER_PARSER_HPP_

#include "dnd_config.hpp"

#include <memory>
#include <regex>
#include <string>

#include <nlohmann/json.hpp>

#include "controllers/groups.hpp"
#include "models/effect_holder/effect.hpp"
#include "models/effect_holder/effect_holder.hpp"
#include "models/effect_holder/effect_holder_with_choices.hpp"
#include "parsing/subparser.hpp"

namespace dnd {

/**
 * @brief A subparser for parsing effect holders.
 */
class EffectHolderParser : public Subparser {
public:
    EffectHolderParser(const Groups& groups) noexcept;
    /**
     * @brief Parse and create an effect holder (without any choices to be made)
     * @param effect_holder_json the JSON containing the effect holder
     * @return the created effect holder
     */
    EffectHolder createEffectHolder(const nlohmann::json& effect_holder_json) const;
    /**
     * @brief Parse and create an effect holder with choices to be made
     * @param effect_holder_json the JSON containing the effect holder
     * @return the created effect holder with choices
     */
    EffectHolderWithChoices createEffectHolderWithChoices(const nlohmann::json& effect_holder_json) const;
    /**
     * @brief Parse all (optional) parts from a JSON into an existing effect holder
     * @param effect_holder_json the JSON containing the effect holder
     * @param effect_holder the effect holder to parse the values into
     */
    void parseEffectHolder(const nlohmann::json& effect_holder_json, EffectHolder* const effect_holder) const;
protected:
    std::unique_ptr<Effect> createEffect(const std::string& effect_str) const;
    void parseAndAddEffect(const std::string& effect_str, EffectHolder* const effect_holder) const;
    void parseAndAddActivation(const std::string& activation_str, EffectHolder* const effect_holder) const;
    void parseAndAddChoice(
        const std::string& choice_key, const nlohmann::json& choice_json, EffectHolderWithChoices& effect_holder
    ) const;
private:
    const Groups& groups;
    static const std::regex activation_regex, effect_regex;
};

inline const std::regex EffectHolderParser::activation_regex(
    "[A-Z][_A-Z0-9]+ (==|!=|>=|<=|>|<) ([A-Z][_A-Z0-9]+|-?\\d+(\\.\\d\\d?)?|true|false)"
);

inline const std::regex EffectHolderParser::effect_regex("[A-Z][_A-Z0-9]+ (earliest|early|normal|late|latest) (("
                                                         "(add|mult|div|set|max|min) -?\\d+(\\.\\d\\d?)?)"
                                                         "|("
                                                         "addOther|multOther|divOther|setOther|maxOther|minOther"
                                                         "|"
                                                         "addConst|multConst|divConst|setConst|maxConst|minConst"
                                                         ") [A-Z][_A-Z0-9]+)");

inline EffectHolderParser::EffectHolderParser(const Groups& groups) noexcept : groups(groups) {}

} // namespace dnd

#endif // EFFECT_HOLDER_PARSER_HPP_
