#ifndef CONTENT_PARSER_HPP_
#define CONTENT_PARSER_HPP_

#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "controllers/content.hpp"
#include "models/character.hpp"
#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/spell.hpp"
#include "parsing/parsing_types.hpp"

namespace dnd {

class ContentParser {
private:
    static const std::unordered_map<ParsingType, std::string> subdir_names;
    std::unordered_map<std::string, std::shared_ptr<const Spell>> parsed_spells;
    std::unordered_map<std::string, std::shared_ptr<const Character>> parsed_characters;
    std::unordered_map<std::string, std::shared_ptr<const CharacterClass>> parsed_character_classes;
    std::unordered_map<std::string, std::shared_ptr<const CharacterSubclass>> parsed_character_subclasses;
    std::unordered_map<std::string, std::shared_ptr<const CharacterRace>> parsed_character_races;
    std::unordered_map<std::string, std::shared_ptr<const CharacterSubrace>> parsed_character_subraces;
    void parseType(
        const dnd::ParsingType parsing_type, const std::vector<std::filesystem::directory_entry>& dirs_to_parse
    );
    // Content& content;
    // const std::unique_ptr<const nlohmann::json> openJSON(const std::filesystem::directory_entry& file);
    // void parseSpells(const std::filesystem::path& directory);
    // void parseCharacters(const std::filesystem::path& directory);
    // void parseCharacterClasses(const std::filesystem::path& directory);
    // void parseCharacterSubclasses(const std::filesystem::path& directory);
    // void parseCharacterRaces(const std::filesystem::path& directory);
    // void parseCharacterSubraces(const std::filesystem::path& directory);
    // void validateCharacterSubclasses() const;
    // void validateCharacterSubraces() const;
public:
    Content parse(const std::filesystem::path& content_path, const std::string& campaign_dir_name);
};

} // namespace dnd

#endif // CONTENT_PARSER_HPP_
