# TODO

Content that may not be formatted perfectly or a part of it is not implemented as functionality.

## Features that are description-only for now, but could be implemented in the future
(vaguely in order of importance)

- Armor, Shield(s), and Weapons are just listed as groups for now, but can/should be Items later

- Ability Score Increases (ASIs) and Feats
  - Characters need to define their ASI choices and/or chosen feats
  - feats need to be defined (probably just like features)

- Special choices such as Eldritch Invocations (Warlock) and Fighting Styles ([Fighter](classes/fighter.json), [Paladin](classes/paladin.json), and [Ranger](classes/ranger.json)), [Sorcerer's](classes/sorcerer.json) Metamagic, [Warlock's](classes/warlock.json) Pact Boon

- how to format features that have multiple steps e.g. something at level 3, something at level 5, and another thing at level 7
  - [Infernal Legacy (Tiefling)](species/tiefling.json)
  - [Song of Rest (Bard)](classes/bard.json)
  - [Life Domain Spells (Life Domain - Cleric)](subclasses/life_domain.json)
  - [Oath of Devotion Spells (Oath of Devotion - Paladin)](subclasses/oath_of_devotion.json)
  - [Unarmored Movement (Monk)](classes/monk.json)
- similarly, there are features that need a similar division of effects into multiple parts
  - [Draconic Bloodline's](subclasses/draconic_bloodline.json) Draconic Resilience (always: MAXP increase, only when no armor: 13 + DEXMOD)

- [Magical Secrets (Bard)](classes/bard.json) - I think, at the moment this is not doing what it should

- [Martial Arts (Monk)](classes/monk.json) needs implementing

- [Warlock's](classes/warlock.json) Mystic Arcanum

- how to format features that have choices and effects for these choices e.g. expertise
  - [Expertise (Bard)](classes/bard.json), [Expertise (Rogue)](classes/rogue.json)
  - [Circle Spells (Circle of the Land)](subclasses/circle_of_the_land.json)
  - Everything from [Hunter](subclasses/hunter.json)

- choices that have alternatives - e.g. choose one of this or two of that
  - [Jack of All Trades (Bard)](classes/bard.json) - no idea how to implement this yet...
  - [Ranger's](classes/ranger.json) Favored Enemy

- should "Resources" (name could be changed) exist outside of the description? (examples would be ki points, bardic inspiration, or sorcery points)

- free decisions should they even get an implementation?
  - [Fighter's](classes/fighter.json) action surge
  - [Life Domain's](subclasses/life_domain.json) Divine Strike
  - [Monk's](classes/monk.json) Stunning Strike
  - [Monk's](classes/monk.json) Diamond Soul
  - [Monk's](classes/monk.json) Empty Body Astral Projection
  - [Monk's](classes/monk.json) Quivering Palm Start
  - [Paladin's](classes/paladin.json) Divine Smite
  - [Ranger's](classes/ranger.json) Feral Senses
  - [Ranger's](classes/ranger.json) Foe Slayer
  - [Rogue's](classes/rogue.json) Sneak Attack
  - [Rogue's](classes/rogue.json) Stroke of Luck
  - [Thief's](subclasses/thief.json) "Thief's Reflexes"
  - [The Fiend's](subclasses/the_fiend.json) Fiendish Resillience and Hurl Through Hell
  - [Wizard's](classes/wizard.json) Arcane Recovery

- [Paladin's](classes/paladin.json) Natural Explorer

- [Ki-Empowered Strikes (Monk)](classes/monk.json) ?
- [Wizard's](classes/wizard.json) Spell Mastery and Signature Spells
- [Rage (Barbarian)](classes/barbarian.json) - could Rage be a parameter?
- [Path of the Berserker](subclasses/path_of_the_berserker.json): "Frenzy" - the bonus action attack (and the exhaustion)
- [Path of the Berserker](subclasses/path_of_the_berserker.json): "Mindless Rage" - Rage as a parameter would be needed though - probably not going to happen

- [Eldritch Invocation](eldritch_invocations.json) Book of Ancient Secrets
- [Eldritch Invocation](eldritch_invocations.json) Devil's Sight

- Should immunity against diseases be somehow integrated? ([Nature's Ward (Circle of the Land)](subclasses/circle_of_the_land.json), ([Purity of Body (Monk)](classes/monk.json), [Divine Health (Paladin)](classes/paladin.json))

- [Monk's](classes/monk.json) Tongue of the Sun and Moon?
- class starting equipment?

- flying speed? (e.g. [Draconic Bloodline's](subclasses/draconic_bloodline.json) Dragon Wings)

## Changes that could bring more clarity

- "spells_added_to_spell_list" - maybe should be renamed


- [Path of the Berserker](subclasses/path_of_the_berserker.json): "Intimidation Presence" - splitting the action into 2 e.g. "Start Intimidation Presence" and "Extend Intimidation Presence"?

## Things that might become problems in the future

- when multiclassing is implemented, the Unarmored Defense features of the [Barbarian](classes/barbarian.json) and [Monk](classes/monk.json) classes probably need changing, because the maximum should be taken, but at the moment, they would add up
