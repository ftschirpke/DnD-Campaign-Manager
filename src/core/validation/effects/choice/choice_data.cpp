#include <dnd_config.hpp>

#include "choice_data.hpp"

#include <regex>
#include <string>
#include <vector>

#include <fmt/format.h>

#include <core/basic_mechanics/abilities.hpp>
#include <core/basic_mechanics/skills.hpp>
#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/effects/choice/choice.hpp>
#include <core/models/effects/choice/choice_rules.hpp>
#include <core/utils/char_manipulation.hpp>
#include <core/validation/effects/stat_change/stat_change_data.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

dnd::ChoiceData::ChoiceData(const ValidationData* parent) noexcept : ValidationSubdata(parent) {}

static dnd::Errors validate_ability_choice(const dnd::ChoiceData& data, const dnd::ValidationData* parent) {
    dnd::Errors errors;
    for (const std::string& explicit_choice : data.explicit_choices) {
        if (explicit_choice.empty()) {
            continue;
        }
        if (!dnd::is_ability(explicit_choice)) {
            errors.add_validation_error(
                dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
                fmt::format(
                    "Choice for '{}' cannot have '{}' as an option since that is not an ability.", data.attribute_name,
                    explicit_choice
                )
            );
        }
    }
    if (!data.group_names.empty()) {
        errors.add_validation_error(
            dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
            fmt::format(
                "Choice for '{}' implies the selection of an ability which prohibits group names.", data.attribute_name
            )
        );
    }
    return errors;
}

static dnd::Errors validate_skill_choice(const dnd::ChoiceData& data, const dnd::ValidationData* parent) {
    dnd::Errors errors;
    for (const std::string& explicit_choice : data.explicit_choices) {
        if (explicit_choice.empty()) {
            continue;
        }
        if (!dnd::is_skill(explicit_choice)) {
            errors.add_validation_error(
                dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
                fmt::format(
                    "Choice for '{}' cannot have '{}' as an option since that is not a skill.", data.attribute_name,
                    explicit_choice
                )
            );
        }
    }
    if (!data.group_names.empty()) {
        errors.add_validation_error(
            dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
            fmt::format(
                "Choice for '{}' implies the selection of a skill which prohibits group names.", data.attribute_name
            )
        );
    }
    return errors;
}

static dnd::Errors validate_stat_change_choice(const dnd::ChoiceData& data, const dnd::ValidationData* parent) {
    dnd::Errors errors;
    dnd::StatChangeData stat_change_data(parent);
    for (const std::string& explicit_choice : data.explicit_choices) {
        if (explicit_choice.empty()) {
            continue;
        }
        stat_change_data.stat_change_str = explicit_choice;
        errors += stat_change_data.validate();
    }
    if (!data.group_names.empty()) {
        errors.add_validation_error(
            dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
            fmt::format(
                "Choice for '{}' implies the selection of a stat change which prohibits group names.",
                data.attribute_name
            )
        );
    }
    return errors;
}

dnd::Errors dnd::ChoiceData::validate() const {
    Errors errors;
    if (amount <= 0) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent, "Choice has non-positive amount"
        );
    }
    if (attribute_name.empty()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent, "Choice has emtpy attribute name"
        );
        return errors;
    } else if (!is_valid_choice_attribute_name(attribute_name)) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
            fmt::format("Choice has invalid attribute name '{}'", attribute_name)
        );
        return errors;
    }

    for (const std::string& explicit_choice : explicit_choices) {
        if (explicit_choice.empty()) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
                fmt::format("Choice for '{}' cannot have an empty string as an option.", attribute_name)
            );
        }
    }
    for (const std::string& group_name : group_names) {
        if (group_name.empty()) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
                fmt::format("Choice for '{}' cannot have an empty string as a group name.", attribute_name)
            );
        }
    }

    ChoiceType type = choice_type_for_attribute_name(attribute_name);
    switch (type) {
        case ChoiceType::ABILITY:
            errors += validate_ability_choice(*this, parent);
            break;
        case ChoiceType::SKILL:
            errors += validate_skill_choice(*this, parent);
            break;
        case ChoiceType::STAT_CHANGE:
            errors += validate_stat_change_choice(*this, parent);
            break;
        default:
            break;
    }
    return errors;
}

static dnd::Errors validate_relations_string_choice(
    const dnd::ChoiceData& data, const dnd::ValidationData* parent, const dnd::Content& content
) {
    dnd::Errors errors;
    if (!dnd::attribute_name_implies_group(data.attribute_name)) {
        errors.add_validation_error(
            dnd::ValidationErrorCode::INCONSISTENT_ATTRIBUTES, parent,
            fmt::format(
                "Choice for '{}' implies string selection but no string group which is contradictory.",
                data.attribute_name
            )
        );
        return errors;
    }
    const std::string group_name = dnd::group_name_for_attribute_name(data.attribute_name);
    for (const std::string& explicit_choice : data.explicit_choices) {
        if (explicit_choice.empty()) {
            continue;
        }
        if (!content.get_groups().is_part_of_group(explicit_choice, group_name)) {
            errors.add_validation_error(
                dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
                fmt::format(
                    "Choice for '{}' cannot have '{}' as an option since that is part of the group '{}'.",
                    data.attribute_name, explicit_choice, group_name
                )
            );
        }
    }
    for (const std::string& subgroup_name : data.group_names) {
        if (subgroup_name.empty()) {
            continue;
        }
        if (!content.get_groups().is_subgroup(subgroup_name, group_name)) {
            errors.add_validation_error(
                dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
                fmt::format(
                    "Choice for '{}' cannot have '{}' as a group name since that is not a subgroup of '{}'.",
                    data.attribute_name, subgroup_name, group_name
                )
            );
        }
    }
    return errors;
}

static dnd::Errors validate_relations_item_choice(
    const dnd::ChoiceData& data, const dnd::ValidationData* parent, const dnd::Content& content
) {
    dnd::Errors errors;
    for (const std::string& explicit_choice : data.explicit_choices) {
        if (explicit_choice.empty()) {
            continue;
        }
        if (!content.get_items().contains(explicit_choice)) {
            errors.add_validation_error(
                dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
                fmt::format(
                    "Choice for '{}' cannot have '{}' as an option since that is not an item.", data.attribute_name,
                    explicit_choice
                )
            );
        }
    }
    for (const std::string& group_name : data.group_names) {
        if (group_name.empty()) {
            continue;
        }
        errors.add_validation_error(
            dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
            fmt::format(
                "Choice for '{}' cannot have '{}' as a group name since that is not a name specifying items.",
                data.attribute_name, group_name
            )
        );
    }
    return errors;
}

static constexpr const char* cantrip_filter_regex_cstr = "(([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
                                                         "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation) )?"
                                                         "(([a-zA-Z][a-z]*) )?[cC]antrips";

static constexpr const char* spell_filter_regex_cstr = "((1st|2nd|3rd|[4-9]th)-level )?"
                                                       "(([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
                                                       "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation) )?"
                                                       "(([a-zA-Z][a-z]*) )?[sS]pells";

static dnd::Errors validate_relations_spell_choice(
    const dnd::ChoiceData& data, const dnd::ValidationData* parent, const dnd::Content& content
) {
    dnd::Errors errors;
    for (const std::string& explicit_choice : data.explicit_choices) {
        if (explicit_choice.empty()) {
            continue;
        }
        if (!content.get_spells().contains(explicit_choice)) {
            errors.add_validation_error(
                dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
                fmt::format(
                    "Choice for '{}' cannot have '{}' as an option since that is not a spell.", data.attribute_name,
                    explicit_choice
                )
            );
        }
    }
    for (const std::string& group_name : data.group_names) {
        if (group_name.empty()) {
            continue;
        }
        if (data.attribute_name == "cantrips_free") {
            static const std::regex cantrip_filter_regex(cantrip_filter_regex_cstr);
            std::smatch match_pieces;
            if (std::regex_match(group_name, match_pieces, cantrip_filter_regex)) {
                std::string class_name = match_pieces[4];
                if (!class_name.empty() && !content.get_classes().contains(class_name)) {
                    errors.add_validation_error(
                        dnd::ValidationErrorCode::RELATION_NOT_FOUND, parent,
                        fmt::format(
                            "Choice for '{}' cannot have '{}' as a group name because '{}' is not a character class.",
                            data.attribute_name, group_name, class_name
                        )
                    );
                }
            } else {
                errors.add_validation_error(
                    dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
                    fmt::format(
                        "Choice for '{}' cannot have '{}' as a group name because that is not a name specifying "
                        "cantrips.",
                        data.attribute_name, group_name
                    )
                );
            }
        } else {
            static const std::regex spell_filter_regex(spell_filter_regex_cstr);
            std::smatch match_pieces;
            if (std::regex_match(group_name, match_pieces, spell_filter_regex)) {
                std::string class_name = match_pieces[6];
                if (!class_name.empty() && !content.get_classes().contains(class_name)) {
                    errors.add_validation_error(
                        dnd::ValidationErrorCode::RELATION_NOT_FOUND, parent,
                        fmt::format(
                            "Choice for '{}' cannot have '{}' as a group name because '{}' is not a character class.",
                            data.attribute_name, group_name, class_name
                        )
                    );
                }
            } else {
                errors.add_validation_error(
                    dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
                    fmt::format(
                        "Choice for '{}' cannot have '{}' as a group name because that is not a name specifying "
                        "spells.",
                        data.attribute_name, group_name
                    )
                );
            }
        }
    }
    return errors;
}

static dnd::Errors validate_relations_choosable_choice(
    const dnd::ChoiceData& data, const dnd::ValidationData* parent, const dnd::Content& content
) {
    dnd::Errors errors;
    for (const std::string& explicit_choice : data.explicit_choices) {
        if (explicit_choice.empty()) {
            continue;
        }
        if (!content.get_choosables().contains(explicit_choice)) {
            errors.add_validation_error(
                dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
                fmt::format(
                    "Choice for '{}' cannot have '{}' as an option since that is not an choosable feature.",
                    data.attribute_name, explicit_choice
                )
            );
        }
    }
    for (const std::string& group_name : data.group_names) {
        if (group_name.empty()) {
            continue;
        }
        errors.add_validation_error(
            dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
            fmt::format(
                "Choice for '{}' cannot have '{}' as a group name since that is not a name specifying "
                "choosable features.",
                data.attribute_name, group_name
            )
        );
    }
    return errors;
}

dnd::Errors dnd::ChoiceData::validate_relations(const Content& content) const {
    Errors errors;
    if (!is_valid_choice_attribute_name(attribute_name)) {
        return errors;
    }

    ChoiceType type = choice_type_for_attribute_name(attribute_name);
    switch (type) {
        case ChoiceType::STRING:
            errors += validate_relations_string_choice(*this, parent, content);
            break;
        case ChoiceType::ITEM:
            errors += validate_relations_item_choice(*this, parent, content);
            break;
        case ChoiceType::SPELL:
            errors += validate_relations_spell_choice(*this, parent, content);
            break;
        case ChoiceType::CHOOSABLE:
            errors += validate_relations_choosable_choice(*this, parent, content);
            break;
        default:
            break;
    }

    return errors;
}
