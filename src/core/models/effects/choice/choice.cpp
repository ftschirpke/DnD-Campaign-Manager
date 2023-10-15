#include <dnd_config.hpp>

#include "choice.hpp"

#include <memory>
#include <regex>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <core/basic_mechanics/abilities.hpp>
#include <core/basic_mechanics/skills.hpp>
#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/effects/choice/choice_rules.hpp>
#include <core/models/spell/spell_type.hpp>
#include <core/searching/content_filters/content_filter.hpp>
#include <core/searching/content_filters/spell/spell_filter.hpp>
#include <core/validation/effects/choice/choice_data.hpp>


static constexpr const char* cantrip_filter_regex_cstr = "(([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
                                                         "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation) )?"
                                                         "(([a-zA-Z][a-z]*) )?[cC]antrips";

static std::unique_ptr<dnd::ContentFilter> create_cantrip_filter(const std::string& group_name) {
    dnd::SpellFilter cantrip_filter;
    cantrip_filter.set_level_filter(dnd::NumberFilterType::EQUAL, 0);
    static const std::regex cantrip_filter_regex(cantrip_filter_regex_cstr);
    std::smatch match;
    if (!std::regex_match(group_name, match, cantrip_filter_regex)) {
        throw dnd::invalid_data("Cannot create choice filter from invalid group name.");
    }
    const std::string spell_level = match[2].str();
    if (!spell_level.empty()) {
        cantrip_filter.set_magic_school_filter(
            dnd::SelectionFilterType::IS_IN, {dnd::magic_school_from_name(spell_level)}
        );
    }
    const std::string spell_class_name = match[4].str();
    if (!spell_class_name.empty()) {
        cantrip_filter.set_classes_filter(dnd::SelectionFilterType::IS_IN, {spell_class_name});
    }
    return std::make_unique<dnd::SpellFilter>(std::move(cantrip_filter));
}

static constexpr const char* spell_filter_regex_cstr = "((1st|2nd|3rd|[4-9]th)-level )?"
                                                       "(([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
                                                       "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation) )?"
                                                       "(([a-zA-Z][a-z]*) )?[sS]pells";

static std::unique_ptr<dnd::ContentFilter> create_spell_filter(const std::string& group_name) {
    dnd::SpellFilter spell_filter;
    static const std::regex spell_filter_regex(spell_filter_regex_cstr);
    std::smatch match;
    if (!std::regex_match(group_name, match, spell_filter_regex)) {
        throw dnd::invalid_data("Cannot create choice filter from invalid group name.");
    }
    const std::string spell_level = match[2].str();
    if (spell_level.empty()) {
        spell_filter.set_level_filter(dnd::NumberFilterType::GREATER_THAN, 0);
    } else if (spell_level == "1st") {
        spell_filter.set_level_filter(dnd::NumberFilterType::EQUAL, 1);
    } else if (spell_level == "2nd") {
        spell_filter.set_level_filter(dnd::NumberFilterType::EQUAL, 2);
    } else if (spell_level == "3rd") {
        spell_filter.set_level_filter(dnd::NumberFilterType::EQUAL, 3);
    } else {
        spell_filter.set_level_filter(dnd::NumberFilterType::EQUAL, spell_level[0] - '0');
    }
    const std::string spell_school_name = match[4].str();
    if (!spell_school_name.empty()) {
        spell_filter.set_magic_school_filter(
            dnd::SelectionFilterType::IS_IN, {dnd::magic_school_from_name(spell_school_name)}
        );
    }
    const std::string spell_class_name = match[6].str();
    if (!spell_class_name.empty()) {
        spell_filter.set_classes_filter(dnd::SelectionFilterType::IS_IN, {spell_class_name});
    }
    return std::make_unique<dnd::SpellFilter>(std::move(spell_filter));
}

static std::vector<std::unique_ptr<dnd::ContentFilter>> spell_filters(dnd::ChoiceData& data) {
    std::vector<std::unique_ptr<dnd::ContentFilter>> filters;
    if (!data.explicit_choices.empty()) {
        dnd::SpellFilter spell_filter;
        spell_filter.set_name_selection_filter(dnd::SelectionFilterType::IS_IN, data.explicit_choices);
        filters.emplace_back(std::make_unique<dnd::SpellFilter>(std::move(spell_filter)));
    }
    for (const std::string& group_name : data.group_names) {
        dnd::SpellFilter spell_filter;
        if (data.attribute_name == "cantrips_free") {
            filters.emplace_back(create_cantrip_filter(group_name));
        } else {
            filters.emplace_back(create_spell_filter(group_name));
        }
    }
    return filters;
}

dnd::Choice dnd::Choice::create(dnd::ChoiceData&& data, const dnd::Content& content) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create choice from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw invalid_data("Choice data is incompatible with the given content.");
    }

    std::vector<std::string> group_names = std::move(data.group_names);
    if (group_names.empty() && data.explicit_choices.empty()) {
        group_names.push_back(data.attribute_name);
    }

    ChoiceType type = choice_type_for_attribute_name(data.attribute_name);
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
            filters = spell_filters(data);
            break;
        case ChoiceType::CHOOSABLE:
            break;
        default:
            throw invalid_data("Cannot create choice from invalid data.");
    }

    return Choice(
        type, std::move(filters), std::move(data.attribute_name), data.amount, std::move(group_names),
        std::move(data.explicit_choices)
    );
}

const std::string& dnd::Choice::get_attribute_name() const noexcept { return attribute_name; }

int dnd::Choice::get_amount() const noexcept { return amount; }

std::set<std::string> dnd::Choice::possible_values(const dnd::Content& content) const {
    std::set<std::string> possible_values;
    switch (type) {
        case ChoiceType::ABILITY:
            for (const auto& ability : ability_cstrings_inorder) {
                possible_values.emplace(ability);
            }
            break;
        case ChoiceType::SKILL:
            for (const auto& skill : get_skills()) {
                possible_values.emplace(skill);
            };
            break;
        case ChoiceType::STRING:
            for (const std::string& group_name : group_names) {
                for (const std::string& group_member : content.get_groups().get_group(group_name)) {
                    possible_values.emplace(group_member);
                }
            };
            [[fallthrough]];
        case ChoiceType::STAT_CHANGE:
            for (const auto& explicit_choice : explicit_choices) {
                possible_values.emplace(explicit_choice);
            };
            break;
        case ChoiceType::ITEM:
            for (const auto& [_, item] : content.get_items().get_all()) {
                possible_values.emplace(item.get_name());
            };
            break;
        case ChoiceType::SPELL:
            // TODO: use the spell filters instead of this
            for (const auto& [_, spell] : content.get_spells().get_all()) {
                possible_values.emplace(spell.get_name());
            };
            break;
        case ChoiceType::CHOOSABLE:
            for (const auto& [_, choosable] : content.get_choosables().get_all()) {
                possible_values.emplace(choosable.get_name());
            };
            break;
        default:
            break;
    }
    return possible_values;
}

dnd::Choice::Choice(
    ChoiceType type, std::vector<std::unique_ptr<ContentFilter>>&& filters, std::string&& attribute_name, int amount,
    std::vector<std::string>&& group_names, std::vector<std::string>&& explicit_choices
) noexcept
    : type(type), attribute_name(std::move(attribute_name)), amount(amount), group_names(std::move(group_names)),
      explicit_choices(std::move(explicit_choices)), filters(std::move(filters)) {}
