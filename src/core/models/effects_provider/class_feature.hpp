#ifndef CLASS_FEATURE_HPP_
#define CLASS_FEATURE_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <filesystem>
#include <map>
#include <string>

#include <core/models/effects/effects.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/source_info.hpp>
#include <core/text/text.hpp>
#include <core/validation/effects_provider/feature_validation.hpp>

namespace dnd {

class Content;
class ContentVisitor;

/**
 * @brief A class representing a feature provided by a class or a subclass
 */
class ClassFeature : public Feature {
public:
    struct Data;

    static CreateResult<ClassFeature> create_for(Data&& data, const Content& content);

    ClassFeature(const ClassFeature&) = delete;
    ClassFeature& operator=(const ClassFeature&) = delete;
    ClassFeature(ClassFeature&&) noexcept = default;
    ClassFeature& operator=(ClassFeature&&) noexcept = default;

    int get_level() const;
    const std::map<int, Effects>& get_higher_level_effects() const;

    const std::string& get_class_name() const;
    const std::string& get_class_source_name() const;
private:
    ClassFeature(
        std::string&& name, Text&& description, std::filesystem::path&& source_path, std::string&& source_name,
        std::string&& key, int level, Effects&& main_effects, std::map<int, Effects>&& higher_level_parts,
        std::string&& class_name, std::string&& class_source_name
    );

    int level;
    std::map<int, Effects> higher_level_effects; // careful when changing the type here, some code relies on order
    std::string class_name;
    std::string class_source_name;
};

struct ClassFeature::Data : public Feature::Data {
    static std::string key(
        const std::string& name, const std::string& source_name, const std::string& class_name,
        const std::string& class_source_name, int level
    );

    std::strong_ordering operator<=>(const Data&) const = default;
    std::string get_key() const override final;

    int level;
    std::map<int, Effects::Data> higher_level_effects_data;
    std::string class_name;
    std::string class_source_name;
};

} // namespace dnd

#endif // CLASS_FEATURE_HPP_
