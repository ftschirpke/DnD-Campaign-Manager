#ifndef CHOOSABLE_HPP_
#define CHOOSABLE_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <core/models/effect_holder/condition/condition.hpp>
#include <core/models/feature/feature.hpp>
#include <core/validation/feature/choosable_data.hpp>

namespace dnd {

class Content;
class ContentVisitor;

/**
 * @brief A class representing anything that can be chosen from a list of options that is feature-like
 * like feats, fighting styles, eldritch invocations, etc.
 */
class Choosable : public Feature {
public:
    /**
     * @brief Constructs a choosable feature from the given data and content
     * @param data the data to construct the choosable feature from
     * @param content the content to use for the construction
     * @return the constructed choosable feature
     * @throws dnd::invalid_data if the data is invalid or is incompatible with the given content
     */
    static Choosable create(ChoosableData&& data, const Content& content);

    Choosable(const Choosable&) = delete;
    Choosable& operator=(const Choosable&) = delete;
    Choosable(Choosable&&) = default;
    Choosable& operator=(Choosable&&) = default;

    const std::string& get_type() const noexcept;
    const std::vector<std::unique_ptr<Condition>>& get_prerequisites() const noexcept;

    /**
     * @brief Accepts a visitor
     * @param visitor reference to the visitor
     */
    virtual void accept(ContentVisitor& visitor) const override final;
private:
    /**
     * @brief Constructs a choosable feature
     * @param name the name of the feature
     * @param source_path the path to the source file of the feature
     * @param prerequisites the prerequisites needed before picking the feature is possible
     * @param main_part the main part of the feature
     * @param other_parts the other parts of the feature
     */
    Choosable(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path, std::string&& type,
        std::vector<std::unique_ptr<Condition>>&& prerequisites, EffectHolder&& main_part,
        std::vector<EffectHolder>&& other_parts = {}
    ) noexcept;

    std::string type;
    std::vector<std::unique_ptr<Condition>> prerequisites;
};

} // namespace dnd

#endif // CHOOSABLE_HPP_
