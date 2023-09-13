#include <dnd_config.hpp>

#include "minimal_testing_content.hpp"

#include <cassert>
#include <string>
#include <utility>

#include <core/content.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/models/spell/spell.hpp>
#include <core/validation/character_class/character_class_data.hpp>
#include <core/validation/character_race/character_race_data.hpp>
#include <core/validation/spell/spell_data.hpp>
#include <core/validation/validation_data_mock.hpp>

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
    auto& feature_data = class_data.features_data.emplace_back(&class_data);
    dndtest::set_valid_mock_values(feature_data, "Example Class Feature");
    class_data.subclass_feature_name = "Example Class Feature";
    class_data.hit_dice_data.str = "d6";
    class_data.important_levels_data.asi_levels = {4, 8, 12, 16, 19};
    assert(class_data.validate().ok());
    assert(class_data.validate_relations(content).ok());
    content.add_character_class(dnd::CharacterClass::create(std::move(class_data), content));
}

static void add_races(dnd::Content& content) {
    dnd::CharacterRaceData race_data;
    dndtest::set_valid_mock_values(race_data, "Dwarf");
    auto& feature_data = race_data.features_data.emplace_back(&race_data);
    dndtest::set_valid_mock_values(feature_data, "Example Race Feature");
    race_data.subraces = true;
    assert(race_data.validate().ok());
    assert(race_data.validate_relations(content).ok());
    content.add_character_race(dnd::CharacterRace::create(std::move(race_data), content));
}

dnd::Content dndtest::minimal_testing_content() {
    dnd::Content content;

    add_groups(content);
    add_spells(content);
    add_classes(content);
    add_races(content);

    return content;
}
