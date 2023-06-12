#ifndef FEATURE_HPP_
#define FEATURE_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>
#include <vector>

#include <core/models/content_piece.hpp>
#include <core/models/effect_holder/effect_holder.hpp>
#include <core/validation/feature/feature_data.hpp>

namespace dnd {

/**
 * @brief A class representing a feature provided by classes, races, subclasses and subraces
 */
class Feature : public ContentPiece {
public:
    /**
     * @brief Constructs a feature from the given data and content
     * @param data the data to construct the feature from
     * @param content the content to use for the construction
     * @return the constructed feature
     * @throws dnd::invalid_data if the given data is invalid or is incompatible with the given content
     */
    static Feature create(FeatureData&& data, const ContentHolder* content);

    Feature(const Feature&) = delete;
    Feature& operator=(const Feature&) = delete;
    Feature(Feature&&) = default;
    Feature& operator=(Feature&&) = default;

    const dnd::EffectHolder& get_main_part() const noexcept;
    const std::vector<dnd::EffectHolder>& get_other_parts() const noexcept;

    /**
     * @brief Accepts a visitor
     * @param visitor pointer to the visitor
     */
    virtual void accept(ContentVisitor* visitor) const override final;
private:
    /**
     * @brief Constructs a feature
     * @param name the name of the feature
     * @param source_path the path to the source file of the feature
     * @param main_part the main part of the feature
     * @param other_parts the other parts of the feature
     */
    Feature(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path, EffectHolder&& main_part,
        std::vector<EffectHolder>&& other_parts = {}
    ) noexcept;

    EffectHolder main_part;
    std::vector<EffectHolder> other_parts;
};

} // namespace dnd

#endif // FEATURE_HPP_