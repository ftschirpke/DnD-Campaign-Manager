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
    struct Data;

    static CreateResult<ClassFeature> create_for(Data&& data, const Content& content);
    static std::string key(
        const std::string& name, const std::string& source_name, const std::filesystem::path& source_path, int level
    );

    ClassFeature(const ClassFeature&) = delete;
    ClassFeature& operator=(const ClassFeature&) = delete;
    ClassFeature(ClassFeature&&) noexcept = default;
    ClassFeature& operator=(ClassFeature&&) noexcept = default;

    int get_level() const;
    const std::map<int, Effects>& get_higher_level_effects() const;

    std::string get_key() const override;

    /**
     * @brief Accepts a visitor
     * @param visitor reference to the visitor
     */
    virtual void accept_visitor(ContentVisitor& visitor) const override;
private:
    ClassFeature(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path, std::string&& source_name,
        int level, Effects&& main_effects, std::map<int, Effects>&& higher_level_parts = {}
    );

    int level;
    std::map<int, Effects> higher_level_effects; // careful when changing the type here, some code relies on order
};

struct ClassFeature::Data : public Feature::Data {
    static std::string key(
        const std::string& name, const std::string& source_name, const std::filesystem::path& source_path, int level
    );

    std::strong_ordering operator<=>(const Data&) const = default;
    std::string get_key() const override;

    int level;
    std::map<int, Effects::Data> higher_level_effects_data;
};

} // namespace dnd

#endif // CLASS_FEATURE_HPP_
