#include <dnd_config.hpp>

#include "choice.hpp"

#include <cassert>
#include <memory>
#include <regex>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <fmt/format.h>
#include <tl/expected.hpp>

#include <core/attribute_names.hpp>
#include <core/basic_mechanics/abilities.hpp>
#include <core/basic_mechanics/magic_schools.hpp>
#include <core/basic_mechanics/skills.hpp>
#include <core/content.hpp>
#include <core/data_result.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/effects/choice/choice_rules.hpp>
#include <core/models/spell/spell_type.hpp>
#include <core/searching/content_filters/content_filter.hpp>
#include <core/searching/content_filters/selection_filter.hpp>
#include <core/searching/content_filters/spell/spell_filter.hpp>
#include <core/validation/effects/choice/choice_validation.hpp>

namespace dnd {

static constexpr const char* cantrip_filter_regex_cstr = "(([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
                                                         "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation) )?"
                                                         "(([a-zA-Z][a-z]*) )?[cC]antrips";

static std::unique_ptr<ContentFilter> create_cantrip_filter(const Content& content, const std::string& group_name) {
    SpellFilter cantrip_filter(content);
    cantrip_filter.level_filter.set(NumberFilterType::EQUAL, 0);
    static const std::regex cantrip_filter_regex(cantrip_filter_regex_cstr);
    std::smatch match;
    if (!std::regex_match(group_name, match, cantrip_filter_regex)) {
        throw invalid_data("Cannot create choice filter from invalid group name.");
    }
    const std::string spell_magic_school_name = match[2].str();
    if (!spell_magic_school_name.empty()) {
        tl::expected<MagicSchool, RuntimeError> magic_school_result = magic_school_from_string(spell_magic_school_name);
        assert(magic_school_result.has_value());
        MagicSchool magic_school = magic_school_result.value();
        cantrip_filter.magic_school_filter.set(SelectionFilterType::IS_IN, {magic_school});
    }
    const std::string spell_class_name = match[4].str();
    if (!spell_class_name.empty()) {
        cantrip_filter.classes_filter.set(SelectionFilterType::IS_IN, {spell_class_name});
    }
    return std::make_unique<SpellFilter>(std::move(cantrip_filter));
}

static constexpr const char* spell_filter_regex_cstr = "((1st|2nd|3rd|[4-9]th)-level )?"
                                                       "(([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
                                                       "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation) )?"
                                                       "(([a-zA-Z][a-z]*) )?[sS]pells";

static std::unique_ptr<ContentFilter> create_spell_filter(const Content& content, const std::string& group_name) {
    SpellFilter spell_filter(content);
    static const std::regex spell_filter_regex(spell_filter_regex_cstr);
    std::smatch match;
    if (!std::regex_match(group_name, match, spell_filter_regex)) {
        throw invalid_data("Cannot create choice filter from invalid group name.");
    }
    const std::string spell_level = match[2].str();
    if (spell_level.empty()) {
        spell_filter.level_filter.set(NumberFilterType::GREATER_THAN, 0);
    } else if (spell_level == "1st") {
        spell_filter.level_filter.set(NumberFilterType::EQUAL, 1);
    } else if (spell_level == "2nd") {
        spell_filter.level_filter.set(NumberFilterType::EQUAL, 2);
    } else if (spell_level == "3rd") {
        spell_filter.level_filter.set(NumberFilterType::EQUAL, 3);
    } else {
        spell_filter.level_filter.set(NumberFilterType::EQUAL, spell_level[0] - '0');
    }
    const std::string spell_magic_school_name = match[4].str();
    if (!spell_magic_school_name.empty()) {
        tl::expected<MagicSchool, RuntimeError> magic_school_result = magic_school_from_string(spell_magic_school_name);
        assert(magic_school_result.has_value());
        MagicSchool magic_school = magic_school_result.value();
        spell_filter.magic_school_filter.set(SelectionFilterType::IS_IN, {magic_school});
    }
    const std::string spell_class_name = match[6].str();
    if (!spell_class_name.empty()) {
        spell_filter.classes_filter.set(SelectionFilterType::IS_IN, {spell_class_name});
    }
    return std::make_unique<SpellFilter>(std::move(spell_filter));
}

static std::vector<std::unique_ptr<ContentFilter>> spell_filters(const Content& content, Choice::Data& data) {
    std::vector<std::unique_ptr<ContentFilter>> filters;
    if (!data.explicit_choices.empty()) {
        SpellFilter spell_filter(content);
        SelectionFilter<std::string>& selection_filter = spell_filter.name_filter.emplace<1>();
        selection_filter.set(SelectionFilterType::IS_IN, data.explicit_choices);
        filters.push_back(std::make_unique<SpellFilter>(std::move(spell_filter)));
    }
    for (const std::string& group_name : data.group_names) {
        SpellFilter spell_filter(content);
        if (data.attribute_name == "cantrips_free") {
            filters.push_back(create_cantrip_filter(content, group_name));
        } else {
            filters.push_back(create_spell_filter(content, group_name));
        }
    }
    return filters;
}

CreateResult<Choice> Choice::create_for(Data&& data, const Content& content) {
    Errors errors = validate_choice_for_content(data, content);
    if (!errors.ok()) {
        return InvalidCreate<Choice>(std::move(data), std::move(errors));
    }

    std::vector<std::string> group_names = std::move(data.group_names);
    if (group_names.empty() && data.explicit_choices.empty()) {
        group_names.push_back(data.attribute_name);
    }

    std::optional<ChoiceType> type_optional = choice_type_for_attribute_name(data.attribute_name);
    if (!type_optional.has_value()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format("Choice has invalid attribute name '{}'", data.attribute_name)
        );
        return InvalidCreate<Choice>(std::move(data), std::move(errors));
    }
    ChoiceType type = type_optional.value();
    std::vector<std::unique_ptr<ContentFilter>> filters;
    switch (type) { // TODO: complete this switch
        case ChoiceType::ABILITY:
            break;
        case ChoiceType::SKILL:
            break;
        case ChoiceType::STRING:
            break;
        case ChoiceType::STAT_CHANGE:
            break;
        case ChoiceType::ITEM:
            break;
        case ChoiceType::SPELL:
            filters = spell_filters(content, data);
            break;
        case ChoiceType::CHOOSABLE:
            break;
    }
    return ValidCreate(Choice(
        type, std::move(filters), std::move(data.attribute_name), data.amount, std::move(group_names),
        std::move(data.explicit_choices)
    ));
}

const std::string& Choice::get_attribute_name() const { return attribute_name; }

int Choice::get_amount() const { return amount; }

std::set<std::string> Choice::possible_values(const Content& content) const {
    std::set<std::string> possible_values;
    switch (type) { // TODO: implement correct filters
        case ChoiceType::ABILITY:
            for (const char* ability : attributes::ABILITIES) {
                possible_values.emplace(ability);
            }
            return possible_values;
        case ChoiceType::SKILL:
            for (const SkillInfo& skill_info : get_all_skill_infos()) {
                possible_values.emplace(skill_info.config_name);
            };
            return possible_values;
        case ChoiceType::STRING:
            for (const std::string& group_name : group_names) {
                for (const std::string& group_member : content.get_groups().get_group(group_name)) {
                    possible_values.emplace(group_member);
                }
            };
            [[fallthrough]];
        case ChoiceType::STAT_CHANGE:
            for (const std::string& explicit_choice : explicit_choices) {
                possible_values.emplace(explicit_choice);
            };
            return possible_values;
        case ChoiceType::ITEM:
            for (const auto& item : content.get_all_items()) {
                possible_values.emplace(item.get_name());
            };
            return possible_values;
        case ChoiceType::SPELL:
            // TODO: use the spell filters instead of this
            for (const auto& spell : content.get_all_spells()) {
                possible_values.emplace(spell.get_name());
            };
            return possible_values;
        case ChoiceType::CHOOSABLE:
            for (const auto& choosable : content.get_all_choosables()) {
                possible_values.emplace(choosable.get_name());
            };
            return possible_values;
    }
    std::unreachable();
}

Choice::Choice(
    ChoiceType type, std::vector<std::unique_ptr<ContentFilter>>&& filters, std::string&& attribute_name, int amount,
    std::vector<std::string>&& group_names, std::vector<std::string>&& explicit_choices
)
    : type(type), attribute_name(std::move(attribute_name)), amount(amount), group_names(std::move(group_names)),
      explicit_choices(std::move(explicit_choices)), filters(std::move(filters)) {}

} // namespace dnd
