#include <dnd_config.hpp>

#include "choosable_parsing.hpp"

#include <filesystem>

#include <nlohmann/json.hpp>
#include <tl/expected.hpp>

#include <core/errors/errors.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/parsing/choice_parsing.hpp>
#include <core/parsing/parser.hpp>

namespace dnd {

static WithErrors<Choosable::Data> parse_choosable(const nlohmann::json& obj, const std::filesystem::path& filepath) {
    WithErrors<Choosable::Data> result;
    Choosable::Data& choosable_data = result.value;
    Errors& errors = result.errors;

    choosable_data.source_path = filepath;
    errors += parse_required_attribute_into(obj, "name", choosable_data.name, filepath);
    errors += parse_required_attribute_into(obj, "source", choosable_data.source_name, filepath);

    // TODO: implement functional choosables
    // choosable_data.main_effects_data;
    // choosable_data.prerequisites_data;

    choosable_data.description = Text{};

    std::optional<Error> error;

    bool skip_first_description_entry = false;
    if (obj.contains("entries") && obj["entries"].is_array() && !obj["entries"].empty()
        && obj["entries"][0].is_string()) {
        std::string first_string = obj["entries"][0].get<std::string>();
        if (first_string.find("following") != std::string::npos || first_string.ends_with("benefits:")) {
            Paragraph first_paragaph{};
            error = parse_paragraph(std::move(first_string), first_paragaph, filepath);
            if (!error.has_value()) {
                choosable_data.description.parts.push_back(first_paragaph);
                skip_first_description_entry = true;
            }
        }
    }

    if (obj.contains("ability")) {
        error = check_required_attribute(obj, "ability", filepath, JsonType::ARRAY);
        if (error.has_value()) {
            errors += error.value();
        } else {
            std::vector<std::string> direct;
            std::vector<std::pair<ParsedChoice, int>> choices;
            const nlohmann::json& ability = obj["ability"];
            for (const nlohmann::json& entry : ability) {
                if (entry.is_string()) {
                    direct.emplace_back(entry.get<std::string>());
                    continue;
                }

                for (const auto& [key, value] : entry.items()) {
                    if (!is_ability(key)) {
                        continue;
                    }
                    int amount;
                    error = parse_required_attribute_into(entry, key.c_str(), amount, filepath);
                    for (int i = 0; i < amount; ++i) {
                        direct.push_back(key);
                    }
                }

                if (entry.contains("choose")) {
                    error = check_required_attribute(entry, "choose", filepath, JsonType::OBJECT);
                    if (error.has_value()) {
                        errors += error.value();
                        continue;
                    }
                    tl::expected<ParsedChoice, Error> parsed_choice = parse_choice(entry["choose"], filepath);

                    int max_score = 20;
                    error = parse_optional_attribute_into(entry, "max", max_score, filepath);
                    if (error.has_value()) {
                        errors += error.value();
                    }

                    if (parsed_choice.has_value()) {
                        choices.emplace_back(parsed_choice.value(), max_score);
                    } else {
                        errors += parsed_choice.error();
                    }
                }
            }

            if (!direct.empty()) {
                Paragraph direct_paragraph{};
                direct_paragraph.parts.push_back(SimpleText{
                    .str = "Ability Score Increase. ",
                    .bold = true,
                    .italic = false,
                });
                direct_paragraph.parts.push_back(SimpleText{
                    .str = fmt::format("Increase your {} by 1, to a maximum of 20.", and_of_strings(direct)),
                    .bold = false,
                    .italic = false,
                });
                choosable_data.description.parts.push_back(direct_paragraph);
            }

            for (auto& [choice, max_score] : choices) {
                Paragraph choice_paragraph{};
                choice_paragraph.parts.push_back(SimpleText{
                    .str = "Ability Score Increase. ",
                    .bold = true,
                    .italic = false,
                });
                choice_paragraph.parts.push_back(SimpleText{
                    .str = fmt::format(
                        "Increase your {} by {}, to a maximum of {}.", or_of_strings(choice.options), choice.amount,
                        max_score
                    ),
                    .bold = false,
                    .italic = false,
                });
                choosable_data.description.parts.push_back(choice_paragraph);
            }
        }
    }

    errors += write_formatted_text_into(obj, choosable_data.description, filepath, skip_first_description_entry);

    return result;
}

WithErrors<Choosable::Data> parse_feat(const nlohmann::json& obj, const std::filesystem::path& filepath) {
    WithErrors<Choosable::Data> feat_data = parse_choosable(obj, filepath);
    feat_data.value.type = "feat";
    return feat_data;
}

} // namespace dnd
