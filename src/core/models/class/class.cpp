#include <dnd_config.hpp>

#include "class.hpp"

#include <algorithm>
#include <cassert>
#include <filesystem>
#include <memory>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include <core/basic_mechanics/dice.hpp>
#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/class/important_levels.hpp>
#include <core/models/effects_provider/class_feature.hpp>
#include <core/models/source_info.hpp>
#include <core/models/spellcasting/spellcasting_factory.hpp>
#include <core/utils/data_result.hpp>
#include <core/validation/class/class_data.hpp>
#include <core/validation/effects/condition/condition_data.hpp>
#include <core/validation/effects/effects_data.hpp>
#include <core/validation/effects_provider/feature_data.hpp>
#include <core/visitors/content/content_visitor.hpp>

#include <iostream>
namespace dnd {

static constexpr const char* level_activation_regex_cstr = "CLASS_LEVEL >= [123456789]\\d?";

static int determine_subclass_level(const Feature::Data& subclass_feature_data) {
    static const std::regex level_activation_regex(level_activation_regex_cstr);
    const std::vector<Condition::Data>& activation_conditions = subclass_feature_data.main_effects_data
                                                                    .activation_conditions_data;
    for (const Condition::Data& condition_data : activation_conditions) {
        if (std::regex_match(condition_data.condition_str, level_activation_regex)) {
            return std::stoi(condition_data.condition_str.substr(15));
        }
    }
    return 1;
}

CreateResult<Class> Class::create_for(Data&& data, const Content& content) {
    Errors errors = validate_class_nonrecursively_for_content(data, content);
    if (!errors.ok()) {
        return InvalidCreate<Class>(std::move(data), std::move(errors));
    }

    int subclass_level = -1;
    OptCRef<ClassFeature> subclass_feature = std::nullopt;

    std::vector<ClassFeature> features;
    features.reserve(data.features_data.size());
    for (ClassFeature::Data& feature_data : data.features_data) {
        CreateResult<ClassFeature> feature_result = ClassFeature::create_for(std::move(feature_data), content);
        if (!feature_result.is_valid()) {
            auto [_, sub_errors] = feature_result.data_and_errors();
            return InvalidCreate<Class>(std::move(data), std::move(sub_errors));
        }
        ClassFeature feature = feature_result.value();
        if (feature.get_name() == data.subclass_feature_name) {
            subclass_level = determine_subclass_level(feature_data);
            features.emplace_back(std::move(feature));
            subclass_feature = features.back();
        } else {
            features.emplace_back(std::move(feature));
        }
    }
    assert(subclass_level != -1);
    assert(subclass_feature.has_value());

    tl::expected<Dice, Errors> hit_dice_result = Dice::from_string(data.hit_dice_str);
    if (!hit_dice_result.has_value()) {
        Errors sub_errors;
        for (const Error& error : hit_dice_result.error().get_errors()) {
            std::string error_message = std::visit([](const auto& error) { return error.get_error_message(); }, error);
            sub_errors.add_validation_error(ValidationError::Code::INVALID_ATTRIBUTE_VALUE, std::move(error_message));
        }
        return InvalidCreate<Class>(std::move(data), std::move(sub_errors));
    }
    Dice hit_dice = hit_dice_result.value();

    CreateResult<ImportantLevels> important_levels_result = ImportantLevels::create(
        std::move(data.important_levels_data), subclass_level
    );
    if (!important_levels_result.is_valid()) {
        auto [_, sub_errors] = important_levels_result.data_and_errors();
        return InvalidCreate<Class>(std::move(data), std::move(sub_errors));
    }
    ImportantLevels important_levels = important_levels_result.value();

    FactoryResult<Spellcasting> spellcasting_result = create_spellcasting(std::move(data.spellcasting_data));
    if (!spellcasting_result.is_valid()) {
        auto [_, sub_errors] = spellcasting_result.data_and_errors();
        return InvalidCreate<Class>(std::move(data), std::move(sub_errors));
    }
    std::unique_ptr<Spellcasting> spellcasting = spellcasting_result.value();

    return ValidCreate(Class(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(features),
        subclass_feature, std::move(hit_dice), std::move(important_levels), std::move(spellcasting)
    ));
}

const std::string& Class::get_name() const noexcept { return name; }

const std::string& Class::get_description() const noexcept { return description; }

const SourceInfo& Class::get_source_info() const noexcept { return source_info; }

const std::vector<ClassFeature>& Class::get_features() const noexcept { return features; }

bool Class::has_spellcasting() const noexcept { return spellcasting != nullptr; }

const Spellcasting* Class::get_spellcasting() const noexcept { return spellcasting.get(); }

OptCRef<ClassFeature> Class::get_subclass_feature() const noexcept { return subclass_feature; }

const Dice& Class::get_hit_dice() const noexcept { return hit_dice; }

const ImportantLevels& Class::get_important_levels() const noexcept { return important_levels; }

void Class::accept_visitor(ContentVisitor& visitor) const { visitor(*this); }

Class::Class(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path,
    std::vector<ClassFeature>&& features, OptCRef<ClassFeature> subclass_feature, Dice hit_dice,
    ImportantLevels&& important_levels, std::unique_ptr<Spellcasting>&& spellcasting
) noexcept
    : name(std::move(name)), description(std::move(description)), source_info(std::move(source_path)),
      features(std::move(features)), spellcasting(std::move(spellcasting)), subclass_feature(subclass_feature),
      hit_dice(std::move(hit_dice)), important_levels(std::move(important_levels)) {}

} // namespace dnd
