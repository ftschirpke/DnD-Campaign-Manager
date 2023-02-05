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
    /**
     * @brief Constructs an EffectHolderParser
     * @param groups the already-parsed groups
     */
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
    /**
     * @brief Parse and create an effect
     * @param effect_str the string that needs to be parsed
     * @return the created effect
     */
    std::unique_ptr<Effect> createEffect(const std::string& effect_str) const;
    /**
     * @brief Parse an effect and add it to an existing effect holder
     * @param effect_str the string that needs to be parsed
     * @param effect_holder the effect holder the effect will be added to
     */
    void parseAndAddEffect(const std::string& effect_str, EffectHolder* const effect_holder) const;
    /**
     * @brief Parse an activation and add it to an existing effect holder
     * @param activation_str the string that needs to be parsed
     * @param effect_holder the effect holder the activation will be added to
     */
    void parseAndAddActivation(const std::string& activation_str, EffectHolder* const effect_holder) const;
    /**
     * @brief Parse a choice and add it to an existing effect holder with choices
     * @param choice_key the key of the choice in the JSON map (the attribute name that is affected by the choice)
     * @param choice_json the body of the choice in the JSON map
     * @param effect_holder the effect holder with choices the choice will be added to
     */
    void parseAndAddChoice(
        const std::string& choice_key, const nlohmann::json& choice_json, EffectHolderWithChoices& effect_holder
    ) const;
private:
    // the regular expression to check the validity of an activation
    static const std::regex activation_regex;
    // the regular expression to check the validity of an effect
    static const std::regex effect_regex;
    // the already-parsed groups used for checking choices
    const Groups& groups;
};

inline EffectHolderParser::EffectHolderParser(const Groups& groups) noexcept : groups(groups) {}

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

} // namespace dnd

#endif // EFFECT_HOLDER_PARSER_HPP_
