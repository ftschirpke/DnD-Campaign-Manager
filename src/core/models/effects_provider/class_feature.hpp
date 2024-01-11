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
#include <core/validation/effects_provider/feature_validation.hpp>

namespace dnd {

class Content;
class ContentVisitor;

/**
 * @brief A class representing a feature provided by a class or a subclass
 */
class ClassFeature : public Feature {
public:
    class Data;

    static CreateResult<ClassFeature> create_for(Data&& data, const Content& content);

    ClassFeature(const ClassFeature&) = delete;
    ClassFeature& operator=(const ClassFeature&) = delete;
    ClassFeature(ClassFeature&&) = default;
    ClassFeature& operator=(ClassFeature&&) = default;

    int get_level() const noexcept;
    const std::map<int, Effects>& get_higher_level_effects() const noexcept;

    /**
     * @brief Accepts a visitor
     * @param visitor reference to the visitor
     */
    virtual void accept_visitor(ContentVisitor& visitor) const override;
private:
    ClassFeature(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path, int level,
        Effects&& main_effects, std::map<int, Effects>&& higher_level_parts = {}
    ) noexcept;

    int level;
    std::map<int, Effects> higher_level_effects; // careful when changing the type here, some code relies on order
};

class ClassFeature::Data : public Feature::Data {
public:
    std::strong_ordering operator<=>(const Data&) const noexcept = default;

    int level;
    std::map<int, Effects::Data> higher_level_effects_data;
};

} // namespace dnd

#endif // CLASS_FEATURE_HPP_
