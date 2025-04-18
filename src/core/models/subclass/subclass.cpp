#include <dnd_config.hpp>

#include "subclass.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/effects_provider/class_feature.hpp>
#include <core/models/source_info.hpp>
#include <core/models/species/species.hpp>
#include <core/models/spellcasting/spellcasting_factory.hpp>
#include <core/validation/subclass/subclass_validation.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

CreateResult<Subclass> Subclass::create_for(Data&& data, const Content& content) {
    Errors errors = validate_subclass_nonrecursively_for_content(data, content);
    if (!errors.ok()) {
        return InvalidCreate<Subclass>(std::move(data), std::move(errors));
    }
    std::vector<ClassFeature> features;
    features.reserve(data.features_data.size());
    for (ClassFeature::Data& feature_data : data.features_data) {
        CreateResult<ClassFeature> feature_result = ClassFeature::create_for(std::move(feature_data), content);
        if (!feature_result.is_valid()) {
            auto [_, sub_errors] = feature_result.data_and_errors();
            return InvalidCreate<Subclass>(std::move(data), std::move(sub_errors));
        }
        features.push_back(feature_result.value());
    }
    CRef<Class> cls = content.get_classes().get(data.class_key).value();

    FactoryResult<Spellcasting> spellcasting_result = create_spellcasting(std::move(data.spellcasting_data));
    if (!spellcasting_result.is_valid()) {
        auto [_, sub_errors] = spellcasting_result.data_and_errors();
        return InvalidCreate<Subclass>(std::move(data), std::move(sub_errors));
    }
    std::unique_ptr<Spellcasting> spellcasting = spellcasting_result.value();

    return ValidCreate(Subclass(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(data.source_name),
        std::move(data.short_name), std::move(features), cls, std::move(spellcasting)
    ));
}

const std::string& Subclass::get_name() const { return name; }

const std::string& Subclass::get_description() const { return description; }

const SourceInfo& Subclass::get_source_info() const { return source_info; }

const std::string& Subclass::get_short_name() const { return short_name; }

const std::vector<ClassFeature>& Subclass::get_features() const { return features; }

bool Subclass::has_spellcasting() const { return spellcasting != nullptr; }

const Spellcasting* Subclass::get_spellcasting() const { return spellcasting.get(); }

CRef<Class> Subclass::get_class() const { return cls; }

void Subclass::accept_visitor(ContentVisitor& visitor) const { visitor(*this); }

std::string Subclass::get_key() const { return key(short_name, get_source_info().name); }

Subclass::Subclass(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, std::string&& source_name,
    std::string&& short_name, std::vector<ClassFeature>&& features, CRef<Class> cls,
    std::unique_ptr<Spellcasting>&& spellcasting
)
    : name(std::move(name)), description(std::move(description)),
      source_info({.path = std::move(source_path), .name = std::move(source_name)}), short_name(std::move(short_name)),
      features(std::move(features)), cls(cls), spellcasting(std::move(spellcasting)) {}

std::string Subclass::Data::get_key() const { return key(short_name, source_name); }

} // namespace dnd
