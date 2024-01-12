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

namespace dnd {

class Content;

class Choice {
public:
    struct Data;

    static CreateResult<Choice> create_for(Data&& data, const Content& content);

    const std::string& get_attribute_name() const;
    int get_amount() const;

    std::set<std::string> possible_values(const Content& content) const;
private:
    Choice(
        ChoiceType type, std::vector<std::unique_ptr<ContentFilter>>&& filters, std::string&& attribute_name,
        int amount, std::vector<std::string>&& group_names, std::vector<std::string>&& explicit_choices
    );

    ChoiceType type;
    std::string attribute_name;
    int amount;
    std::vector<std::string> group_names;
    std::vector<std::string> explicit_choices;
    std::vector<std::unique_ptr<ContentFilter>> filters;
};

struct Choice::Data {
    std::strong_ordering operator<=>(const Data&) const = default;

    std::string attribute_name;
    int amount;
    std::vector<std::string> group_names;
    std::vector<std::string> explicit_choices;
};

} // namespace dnd

#endif // CHOICE_HPP_
