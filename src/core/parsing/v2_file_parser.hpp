#ifndef V2_FILE_PARSER_HPP_
#define V2_FILE_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <map>

#include <core/errors/errors.hpp>
#include <core/models/character/character.hpp>
#include <core/models/class/class.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/species/species.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/parsing/file_parser.hpp>

namespace dnd {

#define X_PARSE_TYPES                                                                                                  \
    X(action), X(adventure), X(artObjects), X(background), X(backgroundFluff), X(baseitem), X(book), X(boon), X(card), \
        X(character), X(charoption), X(charoptionFluff), X(class), X(classFeature), X(classFluff), X(condition),       \
        X(conditionFluff), X(cr), X(cult), X(data), X(deck), X(deity), X(disease), X(dragon), X(dragonMundaneItems),   \
        X(encounter), X(facility), X(facilityFluff), X(feat), X(featFluff), X(gems), X(hazard), X(hazardFluff),        \
        X(hoard), X(individual), X(item), X(itemEntry), X(itemFluff), X(itemGroup), X(itemMastery), X(itemProperty),   \
        X(itemType), X(itemTypeAdditionalEntries), X(language), X(languageFluff), X(languageScript),                   \
        X(legendaryGroup), X(lifeBackground), X(lifeClass), X(lifeTrinket), X(magicItems), X(magicvariant),            \
        X(makebrewCreatureAction), X(makebrewCreatureTrait), X(monster), X(monsterFluff), X(monsterTemplate),          \
        X(monsterfeatures), X(name), X(object), X(objectFluff), X(optionalfeature), X(optionalfeatureFluff),           \
        X(psionic), X(race), X(raceFeature), X(raceFluff), X(recipe), X(recipeFluff), X(reducedItemProperty),          \
        X(reducedItemType), X(reward), X(rewardFluff), X(sense), X(skill), X(spell), X(spellFluff), X(status),         \
        X(subclass), X(subclassFeature), X(subclassFluff), X(subrace), X(table), X(tableGroup), X(trap), X(trapFluff), \
        X(variantrule), X(vehicle), X(vehicleFluff), X(vehicleUpgrade),

enum class ParseType {
#define X(enum) enum##_type
    X_PARSE_TYPES
#undef X
};

#define X(str) #str
constexpr std::array<const char*, 90> parse_types = {X_PARSE_TYPES};
#undef X

class Content;

class V2FileParser : public FileParser {
public:
    struct Data {
        std::map<std::string, Class::Data> class_data;
        std::map<std::string, Subclass::Data> subclass_data;
        std::map<std::string, Species::Data> species_data;
        std::map<std::string, Subspecies::Data> subspecies_data;
        std::map<std::string, Character::Data> character_data;
        std::map<std::string, Choosable::Data> choosable_data;
    };
    explicit V2FileParser(const std::filesystem::path& filepath);
    virtual Errors parse();
    virtual void save_result(Content& content);
private:
    Errors parse_object(const nlohmann::ordered_json& obj, ParseType parse_type);

    Data parsed_data;
};


} // namespace dnd

#endif // V2_FILE_PARSER_HPP_
