#ifndef CHOICE_HPP_
#define CHOICE_HPP_

#include <dnd_config.hpp>

#include <string>
#include <string_view>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/validation/effect_holder/choice/choice_data.hpp>

namespace dnd {

class ContentHolder;
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
    static Choice create(ChoiceData&& data, const ContentHolder& content);

    const std::string& get_attribute_name() const noexcept;
    int get_amount() const noexcept;

    /**
     * @brief Returns the possible values that could be selected for the given content
     * @param content the content to use for the selection
     * @return the possible values that could be selected for this choice as strings
     */
    std::vector<std::string_view> possible_values(const ContentHolder& content) const;
private:
    Choice(
        std::string&& attribute_name, int amount, std::vector<std::string>&& group_names,
        std::vector<std::string>&& explicit_choices
    ) noexcept;

    std::string attribute_name;
    int amount;
    std::vector<std::string> group_names;
    std::vector<std::string> explicit_choices;
};

} // namespace dnd

#endif // CHOICE_HPP_
