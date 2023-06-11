#include <dnd_config.hpp>

#include "effect_factory.hpp"

#include <algorithm>
#include <cassert>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/effect_holder/effect/effect.hpp>
#include <core/models/effect_holder/effect/identifier_effect.hpp>
#include <core/models/effect_holder/effect/literal_effect.hpp>
#include <core/validation/effect_holder/effect/effect_data.hpp>

std::unique_ptr<dnd::Effect> dnd::create_effect(dnd::EffectData&& effect_data) {
    if (!effect_data.validate().ok()) {
        throw invalid_data("Cannot create effect from invalid data.");
    }
    std::string::const_iterator it = std::find(effect_data.effect_str.cbegin(), effect_data.effect_str.cend(), ' ');
    const std::string_view affected_attribute = std::string_view(effect_data.effect_str.cbegin(), it);
    std::string::const_iterator last_it = ++it;
    it = std::find(it, effect_data.effect_str.cend(), ' ');

    const std::string_view effect_time_str = std::string_view(last_it, it);
    dnd::EffectTime effect_time = static_cast<dnd::EffectTime>(-1);
    if (effect_time_str == "earliest") {
        effect_time = dnd::EffectTime::EARLIEST;
    } else if (effect_time_str == "early") {
        effect_time = dnd::EffectTime::EARLY;
    } else if (effect_time_str == "normal") {
        effect_time = dnd::EffectTime::NORMAL;
    } else if (effect_time_str == "late") {
        effect_time = dnd::EffectTime::LATE;
    } else if (effect_time_str == "latest") {
        effect_time = dnd::EffectTime::LATEST;
    }
    assert(effect_time != static_cast<dnd::EffectTime>(-1));

    last_it = ++it;
    it = std::find(it, effect_data.effect_str.cend(), ' ');
    const std::string_view operator_name = std::string_view(last_it, it);
    const std::string_view value = std::string_view(++it, effect_data.effect_str.cend());

    if (value[0] >= 'A' && value[0] <= 'Z') {
        return std::make_unique<IdentifierEffect>(affected_attribute, effect_time, operator_name, value);
    } else if (value == "true" || value == "false") {
        return std::make_unique<LiteralEffect>(affected_attribute, effect_time, operator_name, value == "true");
    } else if (value.find('.') != std::string::npos) {
        return std::make_unique<LiteralEffect>(affected_attribute, effect_time, operator_name, std::stof(value.data()));
    } else {
        return std::make_unique<LiteralEffect>(affected_attribute, effect_time, operator_name, std::stoi(value.data()));
    }
}
