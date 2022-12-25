# Content

- [Content](#content)
- [Formatting Content](#formatting-content)
  - [Characters](#characters)
  - [Races](#races)
  - [Subraces](#subraces)
  - [Classes](#classes)
  - [Subclasses](#subclasses)
  - [Spells](#spells)
  - [Features](#features)
  - [Effects](#effects)
  - [Anything unclear?](#anything-unclear)

This directory is where you store all your content i.e. definitions of classes, races, spells, items, and monsters.

Everything that applies to all of your campaigns, such as the SRD classes and races is placed in the `general` directory. Whereas everything related to the campaign itself is placed within its own folder for the campaign.

As an example, the content from SRD is formatted in this way inside of the `content_imperial` and `content_metric` directories. **Be aware:** They are just an example, all rights belong to WotC, and as an example, they are not guaranteed to be correct, complete, or up-to-date.

Furthermore, a few further examples using the SRD content are provided in the `example_campaign` directory. This directory should also show you, how you can create content for your own campaign and how to place it in a directory so that the DnD-Campaign-Manager can use it.

Within the `content_imperial` directory, you can find the original content with imperial units. I did my best to convert all imperial units into metric units. You can find this adapted content in the `content_metric` directory.

When you choose that your content directory has another name, remember to provide the directory name to the executable using `-d directory_name`.

# Formatting Content

All of your content is specified through JSON files.

The following chapters explain how to format certain types of content.

## Characters
Each character should be stored in a JSON file as a map (or "object") containing the following values:
- "name" (required) - the name of the character
- "class" (required) - the name of the class
- "subclass" (required at a certain level) - the name of the subclass
- "race" (required) - the name of the race
- "subrace" (required if race has subraces) - the name of the subrace
- "base_ability_scores" (required) - an array of initial ability scores **without any modifiers applied** (must be of length 6)
- "level" and "xp" (only one of them is required) - level or XP value of character
  - you can only provide one of these values e.g. only the levels of characters when you are using milestone levelling
- "hit_dice_rolls" (required) - an array of hit dice rolls on level-ups for hit points **without any modifiers applied** (should have one value for each level)
  - **be aware:** if you are using the rule that on level 1 everyone gets the maximum value of the hit dice as HP you still have to provide that value in this array
- "features" (optional) - map of character-specific [features](#features)

<!-- TODO: choices -->

## Races
Each race should be stored in a JSON file as a map (or "object"). The required values are:
- "name" - the name of the race
- "has_subraces" - a boolean whether subraces exist (true or false)
- "features" - map of [features](#features) (map can be empty) e.g. ability score increases or innate spellcasting abilities

Example of a race with only one feature:
```json
{
    "name": "Example Race",
    "has_subraces": false,
    "features": [
        {
            "Ability Score Increase (Example Race)": {
                "description": "Your Wisdom and Charisma scores both increase by 1.",
                "effects": ["WIS normal add 1", "CHA normal add 1"]
            }
        }
    ]
}
```
[Are you still having questions?](#anything-unclear)

## Subraces
Similarly to races, each subrace should be stored in a JSON file as a map (or "object"). The required values are:
- "name" - the name of the subrace
- "race" - the name of the race
- "features" - map of [features](#features) (map can be empty)

Example of a subrace of our Example Race with one feature:
```json
{
    "name": "Example Subrace",
    "race": "Example Race",
    "features": {
        "Longer Life": {
            "description": "The life expectancy for creatures of this subrace is about 50 years higher than the average life expectancy of the whole race."
        }
    }
}
```
[Are you still having questions?](#anything-unclear)

## Classes
Each class should be stored in a JSON file as a map (or "object"). The required values are:
- "name" - the name of the class
- "hit_dice" - string for hit dice i.e. "d6", "d8", "d10" or "d12"
- "asi_levels" (required) - an array of the levels at which characters of this class get Ability Score Increases
- "features" - map of [features](#features) e.g. innate spellcasting abilities
  - there needs to be at least one feature that has the key-value pair `"subclass": true` (this feature should just be a feature describing that from a certain level on, usually level 2 or 3, a subclass can be chosen)
  - usually, there is also a feature describing hit dice and proficiencies for armor, weapons, saving throws and skills

Example of a class without a proficiency feature (for more information on that, have a look at the examples provided in the `content_imperial` and `content_metric` directories):
```json
{
    "name": "Example Class",
    "hit_dice": "d10",
    "features": {
        "Special": {
            "description": "At 4th level, you chooose a subclass.",
            "activation": "LEVEL >= 4"
        }
    }
}
```
[Are you still having questions?](#anything-unclear)

## Subclasses
Similarly to classes, each subclass should be stored in a JSON file as a map (or "object"). The required values are:
- "name" - the name of the subclass
- "class" - the name of the class
- "features" - map of [features](#features) (map can be empty)

Example of a subclass with one feature:
```json
{
    "name": "Example Subclass",
    "class": "Example Class",
    "features": {
        "Do Stuff": {
            "description": "Starting when you choose this subclass at 4th level, you can do stuff as an action.",
            "activation": "LEVEL >= 4",
            "actions": {"Do Stuff": "You can do stuff"}
        }
    }
}
```
[Are you still having questions?](#anything-unclear)

## Spells
A spell should be an entry in a JSON map (or "object"), where the key is the name of the spell and the value is a map with exactly six entries (any further entries will not be parsed).
These should be
- "level_type" - the level and type of the spell
- "casting_time" - the casting time of the spell
- "range" - the range of the spell
- "components" - the required components for the spell
- "duration" - the duration of the spell
- "description" - a description of what the spell does and how it works

It is expected that all of the values are formatted in the typical D&D fashion, but especially the "level_type" and "components" must be in the typical format e.g. "2nd-level transmutation" or "V, M (a bit of fur)" respectively.

An example of a minimal spell JSON file would be:

```json
{
    "Acid Splash": {
        "level_type": "Conjuration cantrip",
        "casting_time": "1 action",
        "range": "60 feet",
        "components": "V, S",
        "duration": "Instantaneous",
        "description": "You hurl a bubble of acid. ..."
    }
}
```
[Are you still having questions?](#anything-unclear)

<!-- ## Items -->
<!-- TODO -->

<!-- ## Monsters -->
<!-- TODO -->

## Features
Races, subraces, classes, subclasses and characters can all have features. In the JSON files, the features are formatted as a map inside a larger map.
```json
{
    ...
    "features": {
        // the features go in here
    }
    ...
}
```
Each feature is represented as a key-value pair where the key is the name of the feature and the value is another JSON map containing the following values:
- "description" (required) - a human-readable description of the feature
- "effects" (optional) - an array of [effects](#effects) that are applied to the creature (these are parsed by the DnD-Campaign Manager tool and then used in calculations)
- "activation" (optional) - a machine-readable description of when this feature activates, the most common version of class features activating at a certain level: `"activation": "LEVEL >= 3"`
- "damage_resistance" (optional) - an array of damage resistances this feature gives you
- "damage_immunity" (optional) - an array of damage immunities this feature gives you
- "condition_immunity" (optional) - an array of condition immunities this feature gives you
- "languages" (optional) - an array of languages this feature allows you to understand
- "senses" (optional) - an array of senses this feature provides you with
- "armor_proficiency" (optional) - an array of armor proficiencies this feature provides
- "weapon_proficiency" (optional) - an array of weapon proficiencies this feature provides
- "tool_proficiency" (optional) - an array of tool proficiencies this feature provides
- "savingthrow_proficiency" (optional) - an array of saving throw proficiencies this feature provides
- "skill_proficiency" (optional) - an array of skill proficiencies this feature provides
- "actions" (optional) - a map of actions (key: expressive name, value: short description) which this feature allows
- "bonus_actions" (optional) - a map of bonus actions (key: expressive name, value: short description) which this feature allows
- "reactions" (optional) - a map of reactions (key: expressive name, value: short description) which this feature allows

Try to keep the descriptions for your actions, bonus action, and reactions as short as possible. They should just be short and descriptive, reminding the reader of what the action/bonus action/reaction does. For the full explanation, you can always read the feature's description which should be complete and impossible to misunderstand.

## Effects
Effects are defined as a string of the following format:
```
ATTRIBUTE time operation PARAMETER
```
The `ATTRIBUTE` is the value the effect is changing. This can be one of the following:
```
MAXHP, AC, SPEED,
STR,     DEX,     CON,     INT,     WIS,     CHA,
STRMAX,  DEXMAX,  CONMAX,  INTMAX,  WISMAX,  CHAMAX,
STRMOD,  DEXMOD,  CONMOD,  INTMOD,  WISMOD,  CHAMOD,
STRSAVE, DEXSAVE, CONSAVE, INTSAVE, WISSAVE, CHASAVE
ACROBATICS,    ANIMAL_HANDLING, ARCANA,   ATHLETICS,
DECEPTION,     HISTORY,         INSIGHT,  INTIMIDATION,
INVESTIGATION, MEDICINE,        NATURE,   PERCEPTION,
PERFORMANCE,   PERSUASION,      RELIGION, SLEIGHT_OF_HAND,
STEALTH,       SURVIVAL
```
The `time` describes how early in the calculation this effect should be applied. The possible values are:
- `earliest` - executed first, e.g. setting a new default value
- `early` - executed early, e.g. multiplication that shouldn't affect any added bonus
- `normal` - intended for a normal bonus e.g. -1, +1, +2 etc.
- `late` - executed late, e.g. multiplication that should affect every added bonus
- `latest` - executed latest, e.g. setting a fixed value

The `operation` is the operation that should be applied to the attribute.
At the moment addition, multiplication, division and setting values are supported.
The operations can use numeric values, other attributes or constants.
Therefore, you can use the following operations:
```
numeric:
    add, mult, div, set
other attributes:
    addOther, multOther, divOther, setOther
constants:
    addConst, multConst, divConst, setConst
```
For numeric operations, the `PARAMETER` needs to be a number e.g. `-1`, `2`, `-2.5` or `34`.

For operations with other attributes, the `PARAMETER` needs to be the other attribute.

For operations with constants, the `PARAMETER` needs to be the constant. The constants that can be used are:
```
LEVEL, XP
```

Examples:
```json
"effects": [
    "AC earliest set 12",     // set new default value for armor class to 12
    "SPEED normal add 1.5",   // increase speed by 1.5
    "AC normal add DEXMOD",   // add dexterity modifier to armor class
    "MAXHP normal add LEVEL"  // add the character level to its armor class
]
```
[Are you still having questions?](#anything-unclear)

## Anything unclear?

If something about the formatting of the content is not clear, please have a look at the SRD content, hopefully, that helps.

If you are comfortable reading C++ code, another possibility would be to have a look at the test cases for the particular content type you are having problems with e.g. the `spell_parser_test.cpp`.

**Either way**, do not hesitate to create an issue, I am happy to help. And if you can point out where this explanation is incomplete or unclear, I have the opportunity to improve it for future readers.

