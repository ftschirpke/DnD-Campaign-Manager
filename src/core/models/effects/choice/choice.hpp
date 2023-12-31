#ifndef CHOICE_HPP_
#define CHOICE_HPP_

#include <dnd_config.hpp>

#include <memory>
#include <set>
#include <string>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/models/effects/choice/choice_rules.hpp>
#include <core/searching/content_filters/content_filter.hpp>
#include <core/utils/data_result.hpp>
#include <core/validation/effects/choice/choice_data.hpp>

namespace dnd {

class Content;
class EffectsData;

class Choice {
public:
    using Data = ChoiceData;

    static CreateResult<Choice> create_for(Data&& data, const Content& content);

    const std::string& get_attribute_name() const noexcept;
    int get_amount() const noexcept;

    std::set<std::string> possible_values(const Content& content) const;
private:
    Choice(
        ChoiceType type, std::vector<std::unique_ptr<ContentFilter>>&& filters, std::string&& attribute_name,
        int amount, std::vector<std::string>&& group_names, std::vector<std::string>&& explicit_choices
    ) noexcept;

    ChoiceType type;
    std::string attribute_name;
    int amount;
    std::vector<std::string> group_names;
    std::vector<std::string> explicit_choices;
    std::vector<std::unique_ptr<ContentFilter>> filters;
};

} // namespace dnd

#endif // CHOICE_HPP_
