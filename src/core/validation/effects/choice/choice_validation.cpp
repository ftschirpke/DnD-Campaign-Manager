#include <dnd_config.hpp>

#include "choice_validation.hpp"

#include <cassert>
#include <optional>
#include <regex>
#include <string>
#include <vector>

#include <fmt/format.h>

#include <core/basic_mechanics/abilities.hpp>
#include <core/basic_mechanics/skills.hpp>
#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/runtime_error.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/effects/choice/choice.hpp>
#include <core/models/effects/choice/choice_rules.hpp>
#include <core/utils/char_manipulation.hpp>
#include <core/validation/effects/stat_change/stat_change_validation.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

static Errors validate_ability_choice(const Choice::Data& data) {
    Errors errors;
    for (const std::string& explicit_choice : data.explicit_choices) {
        if (explicit_choice.empty()) {
            continue;
        }
        if (!is_ability(explicit_choice)) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                fmt::format(
                    "Choice for '{}' cannot have '{}' as an option since that is not an ability.", data.attribute_name,
                    explicit_choice
                )
            );
        }
    }
    if (!data.group_names.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format(
                "Choice for '{}' implies the selection of an ability which prohibits group names.", data.attribute_name
            )
        );
    }
    return errors;
}

static Errors validate_skill_choice(const Choice::Data& data) {
    Errors errors;
    for (const std::string& explicit_choice : data.explicit_choices) {
        if (explicit_choice.empty()) {
            continue;
        }
        if (!is_skill(explicit_choice)) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                fmt::format(
                    "Choice for '{}' cannot have '{}' as an option since that is not a skill.", data.attribute_name,
                    explicit_choice
                )
            );
        }
    }
    if (!data.group_names.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format(
                "Choice for '{}' implies the selection of a skill which prohibits group names.", data.attribute_name
            )
        );
    }
    return errors;
}

static Errors validate_stat_change_choice(const Choice::Data& data) {
    Errors errors;
    StatChange::Data stat_change_validation;
    for (const std::string& explicit_choice : data.explicit_choices) {
        if (explicit_choice.empty()) {
            continue;
        }
        stat_change_validation.stat_change_str = explicit_choice;
        errors += validate_stat_change(stat_change_validation);
    }
    if (!data.group_names.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format(
                "Choice for '{}' implies the selection of a stat change which prohibits group names.",
                data.attribute_name
            )
        );
    }
    return errors;
}

Errors validate_choice_raw(const Choice::Data& data) {
    Errors errors;
    if (data.amount <= 0) {
        errors.add_validation_error(ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Choice has non-positive amount");
    }
    if (data.attribute_name.empty()) {
        errors.add_validation_error(ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Choice has empty attribute name");
        return errors;
    }
    std::optional<ChoiceType> type_optional = choice_type_for_attribute_name(data.attribute_name);
    if (!type_optional.has_value()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format("Choice has invalid attribute name '{}'", data.attribute_name)
        );
        return errors;
    }
    ChoiceType type = type_optional.value();

    for (const std::string& explicit_choice : data.explicit_choices) {
        if (explicit_choice.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                fmt::format("Choice for '{}' cannot have an empty string as an option.", data.attribute_name)
            );
        }
    }
    for (const std::string& group_name : data.group_names) {
        if (group_name.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                fmt::format("Choice for '{}' cannot have an empty string as a group name.", data.attribute_name)
            );
        }
    }

    switch (type) {
        case ChoiceType::ABILITY:
            errors += validate_ability_choice(data);
            return errors;
        case ChoiceType::SKILL:
            errors += validate_skill_choice(data);
            return errors;
        case ChoiceType::STAT_CHANGE:
            errors += validate_stat_change_choice(data);
            return errors;
        case ChoiceType::STRING:
        case ChoiceType::ITEM:
        case ChoiceType::SPELL:
        case ChoiceType::CHOOSABLE:
            return errors;
    }
    assert(false);
    return errors;
}

static Errors validate_relations_string_choice(const Choice::Data& data, const Content& content) {
    Errors errors;
    std::optional<std::string> group_name_optional = group_name_for_attribute_name(data.attribute_name);
    if (!group_name_optional.has_value()) {
        errors.add_validation_error(
            ValidationError::Code::INCONSISTENT_ATTRIBUTES,
            fmt::format(
                "Choice for '{}' implies string selection but no string group which is contradictory.",
                data.attribute_name
            )
        );
        return errors;
    }
    const std::string group_name = group_name_optional.value();
    for (const std::string& explicit_choice : data.explicit_choices) {
        if (explicit_choice.empty()) {
            continue;
        }
        if (!content.get_groups().is_member_of_group(explicit_choice, group_name)) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
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
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                fmt::format(
                    "Choice for '{}' cannot have '{}' as a group name since that is not a subgroup of '{}'.",
                    data.attribute_name, subgroup_name, group_name
                )
            );
        }
    }
    return errors;
}

static Errors validate_relations_item_choice(const Choice::Data& data, const Content& content) {
    Errors errors;
    for (const std::string& explicit_choice : data.explicit_choices) {
        if (explicit_choice.empty()) {
            continue;
        }
        if (!content.get_items().contains(explicit_choice)) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
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
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
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
                                                         "(([a-zA-Z][a-z|]*) )?[cC]antrips";

static constexpr const char* spell_filter_regex_cstr = "((1st|2nd|3rd|[4-9]th)-level )?"
                                                       "(([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
                                                       "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation) )?"
                                                       "(([a-zA-Z][a-z|]*) )?[sS]pells";

static Errors validate_relations_spell_choice(const Choice::Data& data, const Content& content) {
    Errors errors;
    for (const std::string& explicit_choice : data.explicit_choices) {
        if (explicit_choice.empty()) {
            continue;
        }
        if (!content.get_spells().contains(explicit_choice)) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
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
                        ValidationError::Code::RELATION_NOT_FOUND,
                        fmt::format(
                            "Choice for '{}' cannot have '{}' as a group name because '{}' is not a character class.",
                            data.attribute_name, group_name, class_name
                        )
                    );
                }
            } else {
                errors.add_validation_error(
                    ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
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
                        ValidationError::Code::RELATION_NOT_FOUND,
                        fmt::format(
                            "Choice for '{}' cannot have '{}' as a group name because '{}' is not a character class.",
                            data.attribute_name, group_name, class_name
                        )
                    );
                }
            } else {
                errors.add_validation_error(
                    ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
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

static Errors validate_relations_choosable_choice(const Choice::Data& data, const Content& content) {
    Errors errors;
    for (const std::string& explicit_choice : data.explicit_choices) {
        if (explicit_choice.empty()) {
            continue;
        }
        if (!content.get_choosables().contains(explicit_choice)) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
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
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format(
                "Choice for '{}' cannot have '{}' as a group name since that is not a name specifying "
                "choosable features.",
                data.attribute_name, group_name
            )
        );
    }
    return errors;
}

static Errors validate_choice_relations(const Choice::Data& data, const Content& content) {
    Errors errors;
    std::optional<ChoiceType> type_optional = choice_type_for_attribute_name(data.attribute_name);
    if (!type_optional.has_value()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format("Choice has invalid attribute name '{}'", data.attribute_name)
        );
        return errors;
    }
    ChoiceType type = type_optional.value();
    switch (type) {
        case ChoiceType::STRING:
            errors += validate_relations_string_choice(data, content);
            return errors;
        case ChoiceType::ITEM:
            errors += validate_relations_item_choice(data, content);
            return errors;
        case ChoiceType::SPELL:
            errors += validate_relations_spell_choice(data, content);
            return errors;
        case ChoiceType::CHOOSABLE:
            errors += validate_relations_choosable_choice(data, content);
            return errors;
        case ChoiceType::ABILITY:
        case ChoiceType::SKILL:
        case ChoiceType::STAT_CHANGE:
            return errors;
    }
    assert(false);
    return errors;
}

Errors validate_choice_for_content(const Choice::Data& data, const Content& content) {
    Errors errors = validate_choice_raw(data);
    errors += validate_choice_relations(data, content);
    return errors;
}

} // namespace dnd
