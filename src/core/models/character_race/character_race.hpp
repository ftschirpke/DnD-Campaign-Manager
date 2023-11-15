#ifndef CHARACTER_RACE_HPP_
#define CHARACTER_RACE_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>
#include <vector>

#include <core/models/content_piece.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/source_info.hpp>
#include <core/validation/character_race/character_race_data.hpp>

namespace dnd {

class Content;
class ContentVisitor;

class CharacterRace : public ContentPiece {
public:
    /**
     * @brief Constructs a character race from the given data and content
     * @param data the data to construct the character race from
     * @param content the content to use for the construction
     * @return the constructed character race
     * @throws dnd::invalid_data if the given data is invalid or is incompatible with the given content
     */
    static CharacterRace create(CharacterRaceData&& data, const Content& content);

    CharacterRace(const CharacterRace&) = delete;
    CharacterRace& operator=(const CharacterRace&) = delete;
    CharacterRace(CharacterRace&&) = default;
    CharacterRace& operator=(CharacterRace&&) = default;

    const std::string& get_name() const noexcept override;
    const std::string& get_description() const noexcept override;
    const SourceInfo& get_source_info() const noexcept override;
    const std::vector<Feature>& get_features() const noexcept;
    bool has_subraces() const noexcept;

    virtual void accept(ContentVisitor& visitor) const override final;
private:
    CharacterRace(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path,
        std::vector<Feature>&& features, bool has_subraces
    ) noexcept;

    std::string name;
    std::string description;
    SourceInfo source_info;
    std::vector<Feature> features;
    bool subraces;
};

} // namespace dnd

#endif // CHARACTER_RACE_HPP_
