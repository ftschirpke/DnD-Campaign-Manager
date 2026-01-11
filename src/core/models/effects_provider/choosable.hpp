#ifndef CHOOSABLE_HPP_
#define CHOOSABLE_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <core/models/effects/condition/condition.hpp>
#include <core/models/effects_provider/effects_provider.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/text/text.hpp>

namespace dnd {

class Content;
class ContentVisitor;

/**
 * @brief A class representing anything that can be chosen from a list of options that is feature-like
 * like feats, fighting styles, eldritch invocations, etc.
 */
class Choosable : public ContentPiece, public EffectsProvider {
public:
    struct Data;

    static CreateResult<Choosable> create_for(Data&& data, const Content& content);

    Choosable(const Choosable&) = delete;
    Choosable& operator=(const Choosable&) = delete;
    Choosable(Choosable&&) noexcept = default;
    Choosable& operator=(Choosable&&) noexcept = default;

    const std::string& get_name() const override;
    const Text& get_description() const override;
    const SourceInfo& get_source_info() const override;
    const std::string& get_key() const override;
    const Effects& get_main_effects() const override;
    const std::string& get_type() const;
    const std::vector<std::unique_ptr<Condition>>& get_prerequisites() const;

    virtual void accept_visitor(ContentVisitor& visitor) const override final;
private:
    Choosable(
        std::string&& name, Text&& description, std::filesystem::path&& source_path, std::string&& source_name,
        std::string&& key, std::string&& type, std::vector<std::unique_ptr<Condition>>&& prerequisites,
        Effects&& main_effects
    );

    std::string name;
    Text description;
    SourceInfo source_info;
    std::string key;
    Effects main_effects;
    std::string type;
    std::vector<std::unique_ptr<Condition>> prerequisites;
};

struct Choosable::Data : public Feature::Data {
    std::strong_ordering operator<=>(const Data&) const = default;

    std::string type;
    std::vector<Condition::Data> prerequisites_data;
};

} // namespace dnd

#endif // CHOOSABLE_HPP_
