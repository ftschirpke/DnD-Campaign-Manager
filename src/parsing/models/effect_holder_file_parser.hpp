#ifndef EFFECT_HOLDER_FILE_PARSER_HPP_
#define EFFECT_HOLDER_FILE_PARSER_HPP_

#include <regex>
#include <string>

#include <nlohmann/json.hpp>

#include "controllers/groups.hpp"
#include "models/effect_holder/effect.hpp"
#include "models/effect_holder/effect_holder.hpp"
#include "models/effect_holder/effect_holder_with_choices.hpp"
#include "parsing/content_file_parser.hpp"

namespace dnd {

class EffectHolderFileParser : public ContentFileParser {
public:
    EffectHolderFileParser(const Groups& groups) noexcept;
protected:
    const Groups& groups;
    static const std::regex activation_regex, effect_regex;
    std::unique_ptr<Effect> createEffect(const std::string& effect_str) const;
    void parseAndAddEffect(const std::string& effect_str, EffectHolder* const effect_holder) const;
    void parseAndAddActivation(const std::string& activation_str, EffectHolder* const effect_holder) const;
    void parseEffectHolder(const nlohmann::json& effect_holder_json, EffectHolder* const effect_holder) const;
    void parseAndAddChoice(
        const std::string& choice_key, const nlohmann::json& choice_json, EffectHolderWithChoices& effect_holder
    ) const;
    EffectHolder createEffectHolder(const nlohmann::json& effect_holder_json) const;
    EffectHolderWithChoices createEffectHolderWithChoices(const nlohmann::json& effect_holder_json) const;
};

inline const std::regex EffectHolderFileParser::activation_regex(
    "[A-Z][_A-Z0-9]+ (==|!=|>=|<=|>|<) ([A-Z][_A-Z0-9]+|-?\\d+(\\.\\d\\d?)?|true|false)"
);

inline const std::regex EffectHolderFileParser::effect_regex("[A-Z][_A-Z0-9]+ (earliest|early|normal|late|latest) (("
                                                             "(add|mult|div|set|max|min) -?\\d+(\\.\\d\\d?)?)"
                                                             "|("
                                                             "addOther|multOther|divOther|setOther|maxOther|minOther"
                                                             "|"
                                                             "addConst|multConst|divConst|setConst|maxConst|minConst"
                                                             ") [A-Z][_A-Z0-9]+)");

inline EffectHolderFileParser::EffectHolderFileParser(const Groups& groups) noexcept : groups(groups) {}

} // namespace dnd

#endif // EFFECT_HOLDER_FILE_PARSER_HPP_
