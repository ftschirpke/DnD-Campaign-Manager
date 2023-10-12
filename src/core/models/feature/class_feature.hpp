#ifndef CLASS_FEATURE_HPP_
#define CLASS_FEATURE_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <functional>
#include <string>
#include <vector>

#include <core/models/effect_holder/effect_holder.hpp>
#include <core/models/feature/feature.hpp>
#include <core/models/source_info.hpp>
#include <core/validation/feature/feature_data.hpp>

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

    int get_level() const noexcept;
    const std::vector<dnd::EffectHolder>& get_higher_level_parts() const noexcept;

    /**
     * @brief Accepts a visitor
     * @param visitor reference to the visitor
     */
    virtual void accept(ContentVisitor& visitor) const override;
protected:
    /**
     * @brief Constructs a feature
     * @param name the name of the feature
     * @param source_path the path to the source file of the feature
     * @param main_part the main part of the feature
     * @param higher_level_parts the higher level parts of the feature
     */
    ClassFeature(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path, int level,
        EffectHolder&& main_part, std::vector<EffectHolder>&& higher_level_parts = {}
    ) noexcept;
private:
    int level;
    std::vector<EffectHolder> higher_level_parts;
};

} // namespace dnd

#endif // CLASS_FEATURE_HPP_
