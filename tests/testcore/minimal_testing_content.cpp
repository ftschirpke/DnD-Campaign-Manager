#include <dnd_config.hpp>

#include "minimal_testing_content.hpp"

#include <cassert>
#include <string>
#include <utility>

#include <core/attribute_names.hpp>
#include <core/content.hpp>
#include <core/models/class/class.hpp>
#include <core/models/species/species.hpp>
#include <core/models/spell/spell.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/validation/character/character_validation.hpp>
#include <core/validation/class/class_validation.hpp>
#include <core/validation/species/species_validation.hpp>
#include <core/validation/spell/spell_validation.hpp>
#include <core/validation/subclass/subclass_validation.hpp>
#include <core/validation/subspecies/subspecies_validation.hpp>
#include <testcore/validation/validation_data_mock.hpp>

namespace dnd::test {

static void add_groups(Content& content) {
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

static void add_spells(Content& content) {
    Spell::Data spell;
    set_valid_mock_values(spell, "Dancing Lights");
    spell.components_data.verbal = true;
    spell.components_data.somatic = true;
    spell.components_data.material_components = "a bit of phosphorus or wychwood, or a glowworm";
    spell.type_data.magic_school_char = 'V';
    spell.type_data.level = 0;
    spell.type_data.ritual = false;
    spell.casting_time = "1 action";
    spell.range = "120 feet";
    spell.duration = "Concentration, up to 1 minute";
    spell.classes = {"Bard", "Sorcerer", "Wizard"};
    assert(validate_spell_recursively(spell).ok());
    content.add_spell(Spell::create(std::move(spell)).value());

    Spell::Data spell2;
    set_valid_mock_values(spell2, "Fireball");
    spell2.components_data.verbal = true;
    spell2.components_data.somatic = true;
    spell2.components_data.material_components = "a tiny ball of bat guano and sulfur";
    spell2.type_data.magic_school_char = 'V';
    spell2.type_data.level = 2;
    spell2.type_data.ritual = false;
    spell2.casting_time = "1 action";
    spell2.range = "150 feet";
    spell2.duration = "Instantaneous";
    spell2.classes = {"Sorcerer", "Wizard"};
    assert(validate_spell_recursively(spell2).ok());
    content.add_spell(Spell::create(std::move(spell2)).value());

    Spell::Data spell3;
    set_valid_mock_values(spell3, "Cure Wounds");
    spell3.components_data.verbal = true;
    spell3.components_data.somatic = true;
    spell3.type_data.magic_school_char = 'V';
    spell3.type_data.level = 1;
    spell3.type_data.ritual = false;
    spell3.casting_time = "1 action";
    spell3.range = "Touch";
    spell3.duration = "Instantaneous";
    spell3.classes = {"Bard", "Cleric", "Druid", "Paladin", "Ranger"};
    assert(validate_spell_recursively(spell3).ok());
    content.add_spell(Spell::create(std::move(spell3)).value());
}

static void add_classes(Content& content) {
    Class::Data class_data{};
    set_valid_mock_values(class_data, "Wizard");
    class_data.spellcasting_data.is_spellcaster = true;
    class_data.spellcasting_data.ability = dnd::attributes::INTELLIGENCE;
    class_data.spellcasting_data.is_spells_known_type = true;
    ClassFeature::Data& feature_data = class_data.features_data.emplace_back();
    set_valid_mock_values(feature_data, "Example Class Feature");
    feature_data.class_name = "Wizard";
    feature_data.class_source_name = "dummy";
    feature_data.level = 1;
    class_data.subclass_feature_name = "Example Class Feature";
    class_data.hit_dice_str = "d6";
    class_data.important_levels_data.feat_levels = {4, 8, 12, 16, 19};
    assert(validate_class_recursively_for_content(class_data, content).ok());
    content.add_class(Class::create_for(std::move(class_data), content).value());

    Class::Data class_data2{};
    set_valid_mock_values(class_data2, "Rogue");
    class_data2.spellcasting_data.is_spellcaster = false;
    ClassFeature::Data& feature_data2 = class_data2.features_data.emplace_back();
    set_valid_mock_values(feature_data2, "Example Class Feature 2");
    feature_data2.class_name = "Rogue";
    feature_data2.class_source_name = "dummy";
    feature_data2.level = 20;
    class_data2.subclass_feature_name = "Example Class Feature 2";
    class_data2.hit_dice_str = "d8";
    class_data2.important_levels_data.feat_levels = {4, 8, 12, 16, 19};
    assert(validate_class_recursively_for_content(class_data2, content).ok());
    content.add_class(Class::create_for(std::move(class_data2), content).value());


    Subclass::Data subclass_data{};
    set_valid_mock_values(subclass_data, "Abjuration Wizard");
    subclass_data.short_name = "Abjuration";
    subclass_data.spellcasting_data.is_spellcaster = false;
    SubclassFeature::Data& feature_data3 = subclass_data.features_data.emplace_back();
    feature_data3.subclass_short_name = "Abjuration";
    feature_data3.subclass_source_name = "dummy";
    feature_data3.level = 2;
    set_valid_mock_values(feature_data3, "Example Subclass Feature");
    subclass_data.class_key = "Wizard##dummy";
    assert(validate_subclass_recursively_for_content(subclass_data, content).ok());
    content.add_subclass(Subclass::create_for(std::move(subclass_data), content).value());

    Subclass::Data subclass_data2{};
    set_valid_mock_values(subclass_data2, "Assassin");
    subclass_data2.short_name = "Assassin";
    subclass_data2.spellcasting_data.is_spellcaster = false;
    SubclassFeature::Data& feature_data4 = subclass_data2.features_data.emplace_back();
    feature_data4.subclass_short_name = "Assassin";
    feature_data4.subclass_source_name = "dummy";
    feature_data4.level = 3;
    set_valid_mock_values(feature_data4, "Example Subclass Feature 2");
    subclass_data2.class_key = "Rogue##dummy";
    assert(validate_subclass_recursively_for_content(subclass_data2, content).ok());
    content.add_subclass(Subclass::create_for(std::move(subclass_data2), content).value());
}

static void add_species(Content& content) {
    Species::Data species_data1;
    set_valid_mock_values(species_data1, "Dwarf");
    Feature::Data& feature_data1 = species_data1.features_data.emplace_back();
    set_valid_mock_values(feature_data1, "Example Species Feature");
    assert(validate_species_recursively_for_content(species_data1, content).ok());
    content.add_species(Species::create_for(std::move(species_data1), content).value());

    Species::Data species_data2;
    set_valid_mock_values(species_data2, "Human");
    Feature::Data& feature_data2 = species_data2.features_data.emplace_back();
    set_valid_mock_values(feature_data2, "Example Species Feature 2");
    assert(validate_species_recursively_for_content(species_data2, content).ok());
    content.add_species(Species::create_for(std::move(species_data2), content).value());

    Species::Data species_data3;
    set_valid_mock_values(species_data3, "Elf");
    Feature::Data& feature_data3 = species_data3.features_data.emplace_back();
    set_valid_mock_values(feature_data3, "Example Species Feature 3");
    assert(validate_species_recursively_for_content(species_data3, content).ok());
    content.add_species(Species::create_for(std::move(species_data3), content).value());

    Subspecies::Data subspecies_data1;
    set_valid_mock_values(subspecies_data1, "Hill Dwarf");
    Feature::Data& feature_data4 = subspecies_data1.features_data.emplace_back();
    set_valid_mock_values(feature_data4, "Example Subspecies Feature");
    subspecies_data1.species_key = "Dwarf##dummy";
    assert(validate_subspecies_recursively_for_content(subspecies_data1, content).ok());
    content.add_subspecies(Subspecies::create_for(std::move(subspecies_data1), content).value());

    Subspecies::Data subspecies_data2;
    set_valid_mock_values(subspecies_data2, "High Elf");
    Feature::Data& feature_data5 = subspecies_data2.features_data.emplace_back();
    set_valid_mock_values(feature_data5, "Example Subspecies Feature 2");
    subspecies_data2.species_key = "Elf##dummy";
    assert(validate_subspecies_recursively_for_content(subspecies_data2, content).ok());
    content.add_subspecies(Subspecies::create_for(std::move(subspecies_data2), content).value());
}

static void add_characters(Content& content) {
    DND_UNUSED(content);
    Character::Data character_data;
    set_valid_mock_values(character_data, "Example Character");
    Feature::Data& feature_data = character_data.features_data.emplace_back();
    set_valid_mock_values(feature_data, "Example Character Feature");
    character_data.base_ability_scores_data.strength = 10;
    character_data.base_ability_scores_data.dexterity = 8;
    character_data.base_ability_scores_data.constitution = 12;
    character_data.base_ability_scores_data.intelligence = 15;
    character_data.base_ability_scores_data.wisdom = 12;
    character_data.base_ability_scores_data.charisma = 14;
    character_data.feature_providers_data.species_key = "Dwarf##dummy";
    character_data.feature_providers_data.subspecies_key = "Hill Dwarf##dummy";
    character_data.feature_providers_data.class_key = "Wizard##dummy";
    character_data.feature_providers_data.subclass_key = "Abjuration##dummy";
    character_data.progression_data.level = 4;
    character_data.progression_data.xp = 4200;
    character_data.progression_data.hit_dice_rolls = {6, 4, 2, 5};
    assert(validate_character_recursively_for_content(character_data, content).ok());
    content.add_character(Character::create_for(std::move(character_data), content).value());
}

Content minimal_testing_content() {
    Content content;

    add_groups(content);
    add_spells(content);
    add_classes(content);
    add_species(content);
    add_characters(content);

    return content;
}

} // namespace dnd::test
