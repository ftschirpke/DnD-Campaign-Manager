#ifndef CHARACTER_SUBRACE_HPP_
#define CHARACTER_SUBRACE_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>
#include <vector>

#include <core/models/character_race/character_race.hpp>
#include <core/models/feature_holder/feature_holder.hpp>
#include <core/validation/character_subrace/character_subrace_data.hpp>

namespace dnd {

class Content;
class ContentVisitor;

class CharacterSubrace : public FeatureHolder {
public:
    /**
     * @brief Constructs a character subrace from the given data and content
     * @param data the data to construct the character subrace from
     * @param content the content to use for the construction
     * @return the constructed character subrace
     * @throws dnd::invalid_data if the given data is invalid or is incompatible with the given content
     */
    static CharacterSubrace create(CharacterSubraceData&& data, const Content& content);

    CharacterSubrace(const CharacterSubrace&) = delete;
    CharacterSubrace& operator=(const CharacterSubrace&) = delete;
    CharacterSubrace(CharacterSubrace&&) = default;
    CharacterSubrace& operator=(CharacterSubrace&&) = default;

    const CharacterRace* get_race() const noexcept;

    /**
     * @brief Accepts a visitor
     * @param visitor reference to the visitor
     */
    virtual void accept(ContentVisitor& visitor) const override final;
private:
    CharacterSubrace(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path,
        std::vector<Feature>&& features, const CharacterRace* race
    ) noexcept;

    const CharacterRace* race;
};

} // namespace dnd

#endif // CHARACTER_SUBRACE_HPP_
