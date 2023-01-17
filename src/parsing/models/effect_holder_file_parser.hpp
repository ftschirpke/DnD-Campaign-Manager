#ifndef EFFECT_HOLDER_FILE_PARSER_HPP_
#define EFFECT_HOLDER_FILE_PARSER_HPP_

#include <regex>
#include <string>

#include <nlohmann/json.hpp>

#include "models/effect_holder/effect_holder.hpp"
#include "parsing/content_file_parser.hpp"

namespace dnd {

class EffectHolderFileParser : public ContentFileParser {
public:
    EffectHolderFileParser() noexcept = default;
protected:
    static const std::regex activation_regex, effect_regex;
    void parseAndAddEffect(const std::string& effect_str, EffectHolder& effect_holder) const;
    void parseAndAddActivation(const std::string& activation_str, EffectHolder& effect_holder) const;
    EffectHolder createEffectHolder(const std::string& name, const nlohmann::json& effect_holder_json) const;
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

} // namespace dnd

#endif // EFFECT_HOLDER_FILE_PARSER_HPP_
