#include <dnd_config.hpp>

#include "core/validation/effects_provider/class_feature_data.hpp"
#include "minimal_testing_content.hpp"

#include <cassert>
#include <string>
#include <utility>

#include <core/content.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/models/character_subrace/character_subrace.hpp>
#include <core/models/spell/spell.hpp>
#include <core/validation/character/character_data.hpp>
#include <core/validation/character_class/character_class_data.hpp>
#include <core/validation/character_race/character_race_data.hpp>
#include <core/validation/spell/spell_data.hpp>
#include <testcore/validation/validation_data_mock.hpp>

static void add_groups(dnd::Content& content) {
    content.set_subgroups("weapons", {"simple weapons", "martial weapons"});
    content.set_subgroups("simple weapons", {"simple melee weapons", "simple ranged weapons"});
    content.set_subgroups("martial weapons", {"martial melee weapons", "martial ranged weapons"});
    content.add_group_members("simple melee weapons", {"dagger", "spear"});
    content.add_group_members("simple ranged weapons", {"shortbow", "sling"});
    content.add_group_members("martial melee weapons", {"longsword", "shortsword"});
    content.add_group_members("martial ranged weapons", {"longbow", "heavy crossbow"});
    content.set_subgroups("armor", {"light armor", "medium armor", "heavy armor"});
    content.add_group_members("light armor", {"padded", "leather"});
    content.add_group_members("medium armor", {"hide", "chain shirt"});
    content.add_group_members("heavy armor", {"ring mail", "chain mail"});
    content.add_group_members("languages", {"Common", "Dwarvish", "Elvish"});
    content.add_group_members("tools", {"thieves' tools", "disguise kit"});
    content.add_group_members("senses", {"Darkvision", "Blindsight", "Truesight"});
    content.add_group_members("conditions", {"blinded", "charmed", "deafened"});
    content.add_group_members("damage types", {"bludgeoning", "piercing", "slashing"});
}

static void add_spells(dnd::Content& content) {
    dnd::SpellData spell;
    dndtest::set_valid_mock_values(spell, "Dancing Lights");
    spell.components_data.str = "V, S, M (a bit of phosphorus or wychwood, or a glowworm)";
    spell.type_data.str = "Evocation cantrip";
    spell.casting_time = "1 action";
    spell.range = "120 feet";
    spell.duration = "Concentration, up to 1 minute";
    spell.classes = {"Bard", "Sorcerer", "Wizard"};
    assert(spell.validate().ok());
    assert(spell.validate_relations(content).ok());
    content.add_spell(dnd::Spell::create(std::move(spell)));

    dnd::SpellData spell2;
    dndtest::set_valid_mock_values(spell2, "Fireball");
    spell2.components_data.str = "V, S, M (a tiny ball of bat guano and sulfur)";
    spell2.type_data.str = "3rd-level Evocation";
    spell2.casting_time = "1 action";
    spell2.range = "150 feet";
    spell2.duration = "Instantaneous";
    spell2.classes = {"Sorcerer", "Wizard"};
    assert(spell2.validate().ok());
    assert(spell2.validate_relations(content).ok());
    content.add_spell(dnd::Spell::create(std::move(spell2)));

    dnd::SpellData spell3;
    dndtest::set_valid_mock_values(spell3, "Cure Wounds");
    spell3.components_data.str = "V, S";
    spell3.type_data.str = "1st-level Evocation";
    spell3.casting_time = "1 action";
    spell3.range = "Touch";
    spell3.duration = "Instantaneous";
    spell3.classes = {"Bard", "Cleric", "Druid", "Paladin", "Ranger"};
    assert(spell3.validate().ok());
    assert(spell3.validate_relations(content).ok());
    content.add_spell(dnd::Spell::create(std::move(spell3)));
}

static void add_classes(dnd::Content& content) {
    dnd::CharacterClassData class_data;
    dndtest::set_valid_mock_values(class_data, "Wizard");
    class_data.spellcasting_data.is_spellcaster = true;
    class_data.spellcasting_data.ability = "INT";
    class_data.spellcasting_data.is_spells_known_type = true;
    dnd::ClassFeatureData& feature_data = class_data.features_data.emplace_back(&class_data);
    dndtest::set_valid_mock_values(feature_data, "Example Class Feature");
    class_data.subclass_feature_name = "Example Class Feature";
    class_data.hit_dice_data.str = "d6";
    class_data.important_levels_data.feat_levels = {4, 8, 12, 16, 19};
    assert(class_data.validate().ok());
    assert(class_data.validate_relations(content).ok());
    content.add_character_class(dnd::CharacterClass::create(std::move(class_data), content));

    dnd::CharacterClassData class_data2;
    dndtest::set_valid_mock_values(class_data2, "Rogue");
    class_data2.spellcasting_data.is_spellcaster = false;
    dnd::ClassFeatureData& feature_data2 = class_data2.features_data.emplace_back(&class_data2);
    dndtest::set_valid_mock_values(feature_data2, "Example Class Feature 2");
    class_data2.subclass_feature_name = "Example Class Feature 2";
    class_data2.hit_dice_data.str = "d8";
    class_data2.important_levels_data.feat_levels = {4, 8, 12, 16, 19};
    assert(class_data2.validate().ok());
    assert(class_data2.validate_relations(content).ok());
    content.add_character_class(dnd::CharacterClass::create(std::move(class_data2), content));

    dnd::CharacterSubclassData subclass_data;
    dndtest::set_valid_mock_values(subclass_data, "Abjuration Wizard");
    subclass_data.spellcasting_data.is_spellcaster = false;
    dnd::ClassFeatureData& feature_data3 = subclass_data.features_data.emplace_back(&subclass_data);
    dndtest::set_valid_mock_values(feature_data3, "Example Subclass Feature");
    subclass_data.class_name = "Wizard";
    assert(subclass_data.validate().ok());
    assert(subclass_data.validate_relations(content).ok());
    content.add_character_subclass(dnd::CharacterSubclass::create(std::move(subclass_data), content));

    dnd::CharacterSubclassData subclass_data2;
    dndtest::set_valid_mock_values(subclass_data2, "Assassin");
    subclass_data2.spellcasting_data.is_spellcaster = false;
    dnd::ClassFeatureData& feature_data4 = subclass_data2.features_data.emplace_back(&subclass_data2);
    dndtest::set_valid_mock_values(feature_data4, "Example Subclass Feature 2");
    subclass_data2.class_name = "Rogue";
    assert(subclass_data2.validate().ok());
    assert(subclass_data2.validate_relations(content).ok());
    content.add_character_subclass(dnd::CharacterSubclass::create(std::move(subclass_data2), content));
}

static void add_races(dnd::Content& content) {
    dnd::CharacterRaceData race_data1;
    dndtest::set_valid_mock_values(race_data1, "Dwarf");
    dnd::FeatureData& feature_data1 = race_data1.features_data.emplace_back(&race_data1);
    dndtest::set_valid_mock_values(feature_data1, "Example Race Feature");
    race_data1.subraces = true;
    assert(race_data1.validate().ok());
    assert(race_data1.validate_relations(content).ok());
    content.add_character_race(dnd::CharacterRace::create(std::move(race_data1), content));

    dnd::CharacterRaceData race_data2;
    dndtest::set_valid_mock_values(race_data2, "Human");
    dnd::FeatureData& feature_data2 = race_data2.features_data.emplace_back(&race_data2);
    dndtest::set_valid_mock_values(feature_data2, "Example Race Feature 2");
    race_data2.subraces = false;
    assert(race_data2.validate().ok());
    assert(race_data2.validate_relations(content).ok());
    content.add_character_race(dnd::CharacterRace::create(std::move(race_data2), content));

    dnd::CharacterRaceData race_data3;
    dndtest::set_valid_mock_values(race_data3, "Elf");
    dnd::FeatureData& feature_data3 = race_data3.features_data.emplace_back(&race_data3);
    dndtest::set_valid_mock_values(feature_data3, "Example Race Feature 3");
    race_data3.subraces = true;
    assert(race_data3.validate().ok());
    assert(race_data3.validate_relations(content).ok());
    content.add_character_race(dnd::CharacterRace::create(std::move(race_data3), content));

    dnd::CharacterSubraceData subrace_data1;
    dndtest::set_valid_mock_values(subrace_data1, "Hill Dwarf");
    dnd::FeatureData& feature_data4 = subrace_data1.features_data.emplace_back(&subrace_data1);
    dndtest::set_valid_mock_values(feature_data4, "Example Subrace Feature");
    subrace_data1.race_name = "Dwarf";
    assert(subrace_data1.validate().ok());
    assert(subrace_data1.validate_relations(content).ok());
    content.add_character_subrace(dnd::CharacterSubrace::create(std::move(subrace_data1), content));

    dnd::CharacterSubraceData subrace_data2;
    dndtest::set_valid_mock_values(subrace_data2, "High Elf");
    dnd::FeatureData& feature_data5 = subrace_data2.features_data.emplace_back(&subrace_data2);
    dndtest::set_valid_mock_values(feature_data5, "Example Subrace Feature 2");
    subrace_data2.race_name = "Elf";
    assert(subrace_data2.validate().ok());
    assert(subrace_data2.validate_relations(content).ok());
    content.add_character_subrace(dnd::CharacterSubrace::create(std::move(subrace_data2), content));
}

static void add_characters(dnd::Content& content) {
    DND_UNUSED(content);
    dnd::CharacterData character_data;
    dndtest::set_valid_mock_values(character_data, "Example Character");
    dnd::FeatureData& feature_data = character_data.features_data.emplace_back(&character_data);
    dndtest::set_valid_mock_values(feature_data, "Example Character Feature");
    character_data.base_ability_scores_data.ability_scores = {10, 8, 12, 15, 13, 14};
    character_data.character_basis_data.race_name = "Dwarf";
    character_data.character_basis_data.subrace_name = "Hill Dwarf";
    character_data.character_basis_data.class_name = "Wizard";
    character_data.character_basis_data.subclass_name = "Abjuration Wizard";
    character_data.progression_data.level = 4;
    character_data.progression_data.xp = 4200;
    character_data.progression_data.hit_dice_rolls = {6, 4, 2, 5};
    assert(character_data.validate().ok());
    assert(character_data.validate_relations(content).ok());
    content.add_character(dnd::Character::create(std::move(character_data), content));
}

dnd::Content dndtest::minimal_testing_content() {
    dnd::Content content;

    add_groups(content);
    add_spells(content);
    add_classes(content);
    add_races(content);
    add_characters(content);

    return content;
}
