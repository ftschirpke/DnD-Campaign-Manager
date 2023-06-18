#include <dnd_config.hpp>

#include "choice.hpp"

#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/validation/effect_holder/choice/choice_data.hpp>

dnd::Choice dnd::Choice::create(dnd::ChoiceData&& data, const dnd::ContentHolder& content) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create choice from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw invalid_data("Choice data is incompatible with the given content.");
    }

    std::vector<std::string> group_names = std::move(data.group_names);
    if (group_names.empty()) {
        if (data.group_name.empty()) {
            group_names.emplace_back(std::move(data.attribute_name));
        } else {
            group_names.emplace_back(std::move(data.group_name));
        }
    }

    return Choice(
        std::move(data.attribute_name), data.amount, std::move(group_names), std::move(data.explicit_choices)
    );
}

const std::string& dnd::Choice::get_attribute_name() const noexcept { return attribute_name; }

int dnd::Choice::get_amount() const noexcept { return amount; }

std::vector<std::string_view> dnd::Choice::possible_values(const dnd::ContentHolder& content) const {
    std::vector<std::string_view> possible_values;
    for (const auto& explicit_choice : explicit_choices) {
        possible_values.emplace_back(explicit_choice);
    }
    for (const std::string& group_name : group_names) {
        for (const std::string& group_member : content.groups.get_string_group(group_name)) {
            possible_values.emplace_back(group_member);
        }
    }
    return possible_values;
}
