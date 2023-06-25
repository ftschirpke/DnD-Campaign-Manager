#include <dnd_config.hpp>

#include "minimal_testing_content.hpp"

#include <string>
#include <utility>

#include <core/content.hpp>
#include <core/models/spell/spell.hpp>
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
}

static void add_spells(dnd::Content& content) {
    dnd::SpellData spell;
    set_valid_mock_values(spell, "Dancing Lights");
    spell.components_data.str = "V, S, M (a bit of phosphorus or wychwood, or a glowworm)";
    spell.type_data.str = "Evocation cantrip";
    spell.casting_time = "1 action";
    spell.range = "120 feet";
    spell.duration = "Concentration, up to 1 minute";
    spell.classes = {"Bard", "Sorcerer", "Wizard"};
    content.add_spell(dnd::Spell::create(std::move(spell)));

    dnd::SpellData spell2;
    set_valid_mock_values(spell2, "Fireball");
    spell2.components_data.str = "V, S, M (a tiny ball of bat guano and sulfur)";
    spell2.type_data.str = "3rd-level Evocation";
    spell2.casting_time = "1 action";
    spell2.range = "150 feet";
    spell2.duration = "Instantaneous";
    spell2.classes = {"Sorcerer", "Wizard"};
    content.add_spell(dnd::Spell::create(std::move(spell2)));

    dnd::SpellData spell3;
    set_valid_mock_values(spell3, "Cure Wounds");
    spell3.components_data.str = "V, S";
    spell3.type_data.str = "1st-level Evocation";
    spell3.casting_time = "1 action";
    spell3.range = "Touch";
    spell3.duration = "Instantaneous";
    spell3.classes = {"Bard", "Cleric", "Druid", "Paladin", "Ranger"};
    content.add_spell(dnd::Spell::create(std::move(spell3)));
}

dnd::Content dnd::minimal_testing_content() {
    Content content;

    add_groups(content);
    add_spells(content);

    return content;
}
