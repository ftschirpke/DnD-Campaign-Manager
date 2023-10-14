#ifndef CLASS_FEATURE_HPP_
#define CLASS_FEATURE_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <functional>
#include <map>
#include <string>
#include <vector>

#include <core/models/effects/effects.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/source_info.hpp>
#include <core/validation/effects_provider/feature_data.hpp>

namespace dnd {

class Content;
class ContentVisitor;

/**
 * @brief A class representing a feature provided by a class or a subclass
 */
class ClassFeature : public Feature {
public:
    /**
     * @brief Constructs a feature from the given data and content
     * @param data the data to construct the feature from
     * @param content the content to use for the construction
     * @return the constructed feature
     * @throws dnd::invalid_data if the given data is invalid or is incompatible with the given content
     */
    static ClassFeature create(FeatureData&& data, const Content& content);

    ClassFeature(const ClassFeature&) = delete;
    ClassFeature& operator=(const ClassFeature&) = delete;
    ClassFeature(ClassFeature&&) = default;
    ClassFeature& operator=(ClassFeature&&) = default;

    std::vector<const Effects*> get_all_effects() const override;
    int get_level() const noexcept;
    const std::map<int, dnd::Effects>& get_higher_level_effects() const noexcept;

    /**
     * @brief Accepts a visitor
     * @param visitor reference to the visitor
     */
    virtual void accept(ContentVisitor& visitor) const override;
protected:
    /**
     * @brief Constructs a feature
     * @param name the name of the feature
     * @param description the description of the feature
     * @param source_path the path to the source file of the feature
     * @param main_effects the main effects of the feature
     * @param higher_level_effects the higher level effects of the feature
     */
    ClassFeature(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path, int level,
        Effects&& main_effects, std::map<int, Effects>&& higher_level_parts = {}
    ) noexcept;
private:
    int level;
    std::map<int, Effects> higher_level_effects;
};

} // namespace dnd

#endif // CLASS_FEATURE_HPP_
