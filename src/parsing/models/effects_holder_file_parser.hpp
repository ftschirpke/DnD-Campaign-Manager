#ifndef EFFECTS_HOLDER_FILE_PARSER_HPP_
#define EFFECTS_HOLDER_FILE_PARSER_HPP_

#include <string>

#include <nlohmann/json.hpp>

#include "models/effects_holder/effects_holder.hpp"
#include "parsing/content_file_parser.hpp"

namespace dnd {

class EffectsHolderFileParser : public ContentFileParser {
public:
    EffectsHolderFileParser() noexcept = default;
protected:
    void parseAndAddEffect(const std::string& effect_str, EffectsHolder& effects_holder) const;
    void parseAndAddActivation(const std::string& activation_str, EffectsHolder& effects_holder) const;
    EffectsHolder createEffectsHolder(const std::string& name, const nlohmann::json& effect_holder_json) const;
};

} // namespace dnd

#endif // EFFECTS_HOLDER_FILE_PARSER_HPP_
