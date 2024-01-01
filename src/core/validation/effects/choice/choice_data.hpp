#ifndef CHOICE_DATA_HPP_
#define CHOICE_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <string>
#include <vector>

#include <core/errors/errors.hpp>

namespace dnd {

class Content;

class ChoiceData {
public:
    std::strong_ordering operator<=>(const ChoiceData&) const noexcept = default;

    std::string attribute_name;
    int amount;
    std::vector<std::string> group_names;
    std::vector<std::string> explicit_choices;
};

Errors validate_choice_for_content(const ChoiceData& data, const Content& content);

} // namespace dnd

#endif // CHOICE_DATA_HPP_
