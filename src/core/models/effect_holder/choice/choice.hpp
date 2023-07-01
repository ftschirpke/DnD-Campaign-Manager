#ifndef CHOICE_HPP_
#define CHOICE_HPP_

#include <dnd_config.hpp>

#include <memory>
#include <set>
#include <string>
#include <vector>

#include <core/content_filters/content_filter.hpp>
#include <core/errors/errors.hpp>
#include <core/models/effect_holder/choice/choice_rules.hpp>
#include <core/validation/effect_holder/choice/choice_data.hpp>

namespace dnd {

class Content;
class EffectHolderData;

class Choice {
public:
    /**
     * @brief Constructs a choice from the given data and content
     * @param data the data to construct the choice from
     * @param content the content to use for the construction
     * @return the constructed choice
     * @throws dnd::invalid_data if the data is invalid or is incompatible with the given content
     */
    static Choice create(ChoiceData&& data, const Content& content);

    const std::string& get_attribute_name() const noexcept;
    int get_amount() const noexcept;

    /**
     * @brief Returns the possible values that could be selected for the given content
     * @param content the content to use for the selection
     * @return the possible values that could be selected for this choice as strings
     */
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
