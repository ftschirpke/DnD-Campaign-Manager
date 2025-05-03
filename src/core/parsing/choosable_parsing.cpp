#include <dnd_config.hpp>

#include "choosable_parsing.hpp"

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/parsing/parser.hpp>

namespace dnd {

static WithErrors<Choosable::Data> parse_choosable(const nlohmann::json& obj, const std::filesystem::path& filepath) {
    WithErrors<Choosable::Data> result;
    Choosable::Data& choosable_data = result.value;
    Errors& errors = result.errors;

    choosable_data.source_path = filepath;
    errors += parse_required_attribute_into(obj, "name", choosable_data.name, filepath);
    errors += parse_required_attribute_into(obj, "source", choosable_data.source_name, filepath);
    errors += write_formatted_text_into(obj, choosable_data.description, filepath);

    // TODO: implement functional choosables
    // choosable_data.main_effects_data;
    // choosable_data.prerequisites_data;

    return result;
}

WithErrors<Choosable::Data> parse_feat(const nlohmann::json& obj, const std::filesystem::path& filepath) {
    WithErrors<Choosable::Data> feat_data = parse_choosable(obj, filepath);
    feat_data.value.type = "feat";
    return feat_data;
}

} // namespace dnd
