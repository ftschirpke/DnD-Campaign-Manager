# Content

- [Content](#content)
- [Formatting Content](#formatting-content)
  - [Characters](#characters)
  - [Races](#races)
  - [Subraces](#subraces)
  - [Classes](#classes)
    - [Spellcasting](#spellcasting)
  - [Subclasses](#subclasses)
  - [Items](#items)
  - [Spells](#spells)
  - [Features](#features)
  - [Effects](#effects)
    - [Custom Attributes](#custom-attributes)
  - [Groups](#groups)
  - [Anything unclear?](#anything-unclear)

This directory is where you store all your content i.e. definitions of classes, races, spells, items, and monsters.

Everything that applies to all of your campaigns, such as the SRD classes and races is placed in the `general` directory. Whereas everything related to the campaign itself is placed within its own folder for the campaign.

As an example, the content from SRD is formatted in this way. These examples are located in the [`content_imperial/general/srd`](../content_imperial/general/srd/) and [`content_metric/general/srd`](../content_metric/general/srd/) directories. **Be aware:** They are just an example, all rights belong to WotC, and as an example, they are not guaranteed to be correct, complete, or up-to-date.

Furthermore, a few further examples using the SRD content are provided in the `example_campaign` directories ([imperial](../content_imperial/example_campaign/) or [metric](../content_metric/example_campaign/)). These directories should also show you, how you can create content for your own campaign and how to place it in a directory so that the DnD-Campaign-Manager can use it.

Within the [`content_imperial`](../content_imperial/) directory, you can find the original content using imperial units. I did my best to convert all imperial units into metric units. You can find this adapted content in the [`content_metric`](../content_metric/) directory.

By default, the DnD-Campaign-Manager parses all content from a directory called `content` in the location where you are starting the tool.
Therefore, when your content is located somewhere else, remember to provide the directory name to the executable using `-d path/to/dir`.

# Formatting Content

All of your content is specified through JSON files.

The following chapters explain how to format certain types of content.

## Characters
Each character should be stored in a JSON file as a map (or "object") containing the following values:
| Name | required? | format | description |
|------|:---------:|:------:|-------------|
| name | yes | string | name of the character |
| description | yes | string | a (short) description |
| class | yes | string | name of the class |
| subclass | at certain levels | string | name of the subclass |
| race | yes | string | name of the race |
| subrace | if race has subraces | string | name of the subrace |
| base_ability_scores | yes | array of integers | array of initial ability scores **without any modifiers applied** (must be of length 6) |
| level | if "xp" not provided | integer | level value of character <ul><li>if you provide this, you do not need to provide XP values which is useful for milestone levelling</li><li>but if you do not want a specific XP value (and not the minimum value for that level), you can and *should* still provide the XP value</li></ul> |
| xp | if "level" not provided | integer | XP value of character <ul><li>if you provide this, you do not need to provide a level, but you can</li></ul> |
| hit_dice_rolls | yes | array of integers | hit dice rolls on level-ups for hit points **without any modifiers applied** (should have one value for each level) <ul><li>**be aware:** if you are using the rule that on level 1 everyone gets the maximum value of the hit die as HP, you still have to provide that value in this array</li></ul> |
| features | no | [features](#features) map | character-specific features |

<!-- TODO: choices -->

## Races
Each race should be stored in a JSON file as a map (or "object"). The values possible values are:
| Name | required? | format | description |
|------|:---------:|:------:|-------------|
| name | yes | string | name of the race |
| description | yes | string | a (short) description |
| has_subraces | yes | boolean | describes whether subraces exist (true or false) |
| features | no | [features](#features) map | racial features e.g. ability score increases or innate spellcasting abilities |

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

## Subraces
Similarly to races, each subrace should be stored in a JSON file as a map (or "object"). The possible values are:
| Name | required? | format | description |
|------|:---------:|:------:|-------------|
| name | yes | string | name of the subrace |
| description | yes | string | a (short) description |
| race | yes | string | name of the race |
| features | no | [features](#features) map | subrace features |

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

## Classes
Each class should be stored in a JSON file as a map (or "object"). The required values are:
| Name | required? | format | description |
|------|:---------:|:------:|-------------|
| name | yes | string | name of the class |
| description | yes | string | a (short) description |
| hit_dice | yes | string | string for hit dice i.e. "d6", "d8", "d10" or "d12" |
| feat_levels | yes | array of integers | levels at which characters of this class get feats or Ability Score Increases |
| spellcasting | no | [spellcasting](#spellcasting) map | all information about spellcasting, if the class is a spellcasting class |
| features | yes | [features](#features) map | class features <ul><li>there needs to be exactly one feature that has the key-value pair `"subclass": true` (this feature should just be a feature describing that from a certain level you can choose a subclass, usually level 1, 2 or 3)</li><li>usually, there is also a feature describing hit dice and proficiencies for armor, weapons, saving throws and skills</li></ul> |

Example of a class without a proficiency feature (for more information on proficiency features, have a look at the examples provided in the [`content_imperial`](../content_imperial/general/srd/classes/) and [`content_metric`](../content_metric/general/srd/classes/) directories):
```json
{
    "name": "Example Class",
    "hit_dice": "d10",
    "features": {
        "Special": {
            "description": "At 4th level, you chooose a subclass.",
            "activation": "CLASS_LEVEL >= 4",
            "subclass": true
        }
    }
}
```

### Spellcasting
Some classes allow spellcasting. In that case, you have to provide the key "spellcasting" to the map. And the value mapped to "spellcasting" is another map with (some of) the following values:
| Name | required? | format | description |
|------|:---------:|:------:|-------------|
| ability | yes | string | one of the abilities "STR", "DEX", "CON", "INT", "WIS", "CHA" |
| ritual_casting | yes | boolean | describes whether the class allows spells with the ritual tag to be cast as a ritual |
| spells_known | if "preparation_caster" not provided | array of integers | length-20-array containing the amounts of spells known for each level<ul><li>**mutually exclusive to "preparation_caster"**</li></ul> |
| preparation_caster | if "spells_known" not provided | string | one of "half" or "full" to describe how many spells can be prepared: `spellcasting ability + (half of) level`<ul><li>**mutually exclusive to "spells_known"**</li></ul> |
| cantrips_known | no | array of integers | length-20-array containing the amounts of cantrips known for each level from 1 to 20 <ul><li>if not provided, the tool assumes that the class provides no cantrips (e.g. Paladins and Rangers)</li></ul> |
| levelX_slots | no | array of integers | length-20-array containing the amounts of spells slots of level X, where X is between 1 and 9, for each level <ul><li>if for example only "level2_slots" and "level3_slots" are provided, the tool assumes that the class provides no spell slots of any other level</li></ul> |


## Subclasses
Similarly to classes, each subclass should be stored in a JSON file as a map (or "object"). The possible values are:
| Name | required? | format | description |
|------|:---------:|:------:|-------------|
| name | yes | string | name of the subclass |
| description | yes | string | a (short) description |
| class | yes | string | name of the class |
| spellcasting | no | [spellcasting](#spellcasting) map | all information about spellcasting, if the subclass is a spellcasting subclass |
| features | no | [features](#features) map | subclass features |

Example of a subclass with one feature:
```json
{
    "name": "Example Subclass",
    "class": "Example Class",
    "features": {
        "Do Stuff": {
            "description": "Starting when you choose this subclass at 4th level, you can do stuff as an action.",
            "activation": "CLASS_LEVEL >= 4",
            "actions": {"Do Stuff": "You can do stuff"}
        }
    }
}
```

## Spells
A spell should be an entry in a JSON map (or "object"), where the key is the name of the spell and the value is a map with exactly six entries (any further entries will not be parsed).
These should be
| Name | required? | format | description |
|------|:---------:|:------:|-------------|
| level_type | yes | string | level and type of the spell |
| casting_time | yes | string | casting time of the spell |
| range | yes | string | range of the spell |
| components | yes | string | required components for the spell |
| duration | yes | string | duration of the spell |
| description | yes | string | description of what the spell does and how it works |

It is expected that all of the values are formatted in the typical D&D fashion, but especially the "level_type" and "components" **must be** in the typical format e.g. "2nd-level transmutation" or "V, M (a bit of fur)" respectively.

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

## Items
An item should be an entry in a JSON map (or "object"), where the key is the name of the item and the value is a map with the following values
| Name | required? | format | description |
|------|:---------:|:------:|-------------|
| requires_attunement | yes | boolean | `true` if item requires attunement, `false` otherwise |
| description | yes | string | description of what the item does and how it works |
| cosmetic_description | no | string | description of other aspects of the item e.g. how it looks or its history |

An example of a minimal item JSON file would be:

```json
{
    "+1 Shortsword": {
        "requires_attunement": false,
        "description": "This shortsword gives a +1 bonus to attack and damage rolls.",
        "cosmetic_description": "It looks like any other simple shortsword, it does not look special at all."
    }
}
```

<!-- ## Monsters -->
<!-- TODO -->

<!-- ## NPCs -->
<!-- TODO -->

## Features
Races, subraces, classes, subclasses and characters can all have features. In the JSON files, the features are formatted as a map inside a larger map.
```jsonc
{
    // ...
    "features": {
        // the features go in here
    }
    // ...
}
```
Each feature is represented as a key-value pair where the key is the name of the feature and the value is another JSON map containing the following values:

| Name | required? | format | description |
|------|:---------:|:------:|-------------|
| description | yes | string | a human-readable description of the feature|
| effects | no | array of [effects](#effects) | effect are applied to the creature (these are parsed by the DnD-Campaign Manager tool and then used in calculations) |
| activation | no | string | a machine-readable description of when this feature activates, the most common version of class features activating at a certain class level: `"activation": "CLASS_LEVEL >= 3"` |
| activations | no | array of strings | If you want to provide multiple activations per feature, which all have to be true, you can put multiple of them into an array. Please provide either "activation", "activations", or neither of them **but not both**. |
| damage_resistances | no | array of strings | damage resistances this feature gives you |
| damage_immunities | no | array of strings | damage immunities this feature gives you |
| condition_immunities | no | array of strings | condition immunities this feature gives you |
| languages | no | array of strings | languages this feature allows you to understand |
| senses | no | array of strings | senses this feature provides you with |
| armor_proficiencies | no | array of strings | armor proficiencies this feature provides |
| weapon_proficiencies | no | array of strings | weapon proficiencies this feature provides |
| tool_proficiencies | no | array of strings | tool proficiencies this feature provides |
| savingthrow_proficiencies | no | array of strings | saving throw proficiencies this feature provides |
| skill_proficiencies | no | array of strings | skill proficiencies this feature provides |
| cantrips_free | no | array of strings | cantrips that do not count against the number of cantrips known |
| spells_at_will | no | array of strings | non-cantrip spells that you can cast at will i.e. without expending a spell slot or material components |
| spells_innate | no | array of strings | non-cantrip spells that you can cast once a day (or rather once between two long rests) **without** expending a spell slot<ul><li>examples: innate spellcasting such as Tiefling's Infernal Legacy</li></ul> |
| spells_free_once_a_day | no | array of strings | non-cantrip spells that you can cast once a day (or rather once between two long rests) **with** expending a spell slot<ul><li>examples: eldritch invocations</li></ul> |
| spells_known | no | array of strings | non-cantrip spells that are added to your spell list and you know them / you always have them prepared <ul><li>these spells do not count to the number of spells you know</li><li>examples: Cleric's domain spells, Paladin's oath spells</li></ul> |
| spells_always_prepared | no | array of strings | a synonym for "spells_known"<ul><li>It doesn't matter whether you write "spells_known" or "spells_always_prepared", the tool will treat them the same and will know what to do with the spells, this is just for convenience and readability</li></ul> |
| spells_known_included | no | array of strings | non-cantrip spells that are added to your spell list and you know them / you always have them prepared <ul><li> **BUT** these spells **do** count to the number of spells you know / the number of spells you can prepare</li><li>you probably will not ever need this, but the Bard's "Magical Secrets" require this</li></ul> |
| spells_added_to_spell_list | no | array of strings | non-cantrip spells that are added to your spell list <ul><li>you **do not need to do this manually for any of the above**, so just use this if these spells are **only** added to your spell list</li><li>examples: Druid's circle spells, Warlock's patron spells</li></ul> |
| actions | no | string-string map | map of actions (key: expressive name, value: short description) which this feature allows |
| bonus_actions | no | string-string map | map of bonus actions (key: expressive name, value: short description) which this feature allows |
| reactions | no | string-string map | map of reactions (key: expressive name, value: short description) which this feature allows |

Try to keep the descriptions for your actions, bonus action, and reactions as short as possible. They should just be short and descriptive, reminding the reader of what the action/bonus action/reaction does. For the full explanation, you can always read the feature's description which should be complete and impossible to misunderstand.

## Effects
Effects are defined as a string of the following format:
```
ATTRIBUTE time operation PARAMETER
```
The `ATTRIBUTE` is the value the effect is changing.
Below, I listed the hard-coded attributes that will always be there, but [you can also use and create your own](#custom-attributes).
```
// mutable attributes:
MAX_HP, AC, SPEED,
STR,      DEX,      CON,      INT,      WIS,      CHA,
STR_MAX,  DEX_MAX,  CON_MAX,  INT_MAX,  WIS_MAX,  CHA_MAX,
STR_MOD,  DEX_MOD,  CON_MOD,  INT_MOD,  WIS_MOD,  CHA_MOD,
STR_SAVE, DEX_SAVE, CON_SAVE, INT_SAVE, WIS_SAVE, CHA_SAVE
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
At the moment the following values are supported:

|operation|description|parameter limitations|
|---------|-----------|---------------------|
| add | add the parameter value to the attribute value | booleans not allowed |
| mult | multiply the attribute value with the parameter value | booleans not allowed |
| div | divide the attribute value by the parameter value | booleans not allowed |
| set | set the attribute value to the parameter value | none (booleans allowed) |
| max | set the attribute value to the maximum of the parameter value and its previous value | booleans not allowed |
| min | set the attribute value to the minimum of the parameter value and its previous value | booleans not allowed |

The `PARAMETER` can be a number e.g. `-1`, `2`, `-2.5` or `34`, but floating point numbers may only have up to 2 decimal places.

`PARAMETER` can also be a boolean value (i.e. `true` or `false`) but in that case only the `set` operation is allowed (see table above).

Lastly, the `PARAMETER` can also be another attribute. In that case the value for the given attribute is retrieved and used for the calculation. Here, you can choose one of the mutable attributes above, or one of the hard-coded immutable ones:
```
// immutable attributes:
ARMOR_ON, HAS_SPELLCASTING
LEVEL, XP
```

Examples:
```jsonc
"effects": [
    "AC earliest set 12",           // set new default value for armor class to 12
    "SPEED normal add 1.5",         // increase speed by 1.5
    "AC normal add DEXMOD",         // add dexterity modifier to armor class
    "MAX_HP normal add LEVEL"       // add the character level to its armor class
]
```

### Custom Attributes
Declaring your custom attributes allows you to create even more complex effects.
A custom attribute is created the first time you use it as the `ATTRIBUTE` part of an effect.
It is recommended to `set` the attribute to a value first, otherwise it will be initialized with the value `0` (or `false`) by default.
After it has been created, you can manipulate the value as any other attribute and recombine it with existing attributes to implement effects like the Monk's Unarmored Defense:
```jsonc
"effects": [
    "UNARMORED_DEFENSE_MONK earliest set 10",
    "UNARMORED_DEFENSE_MONK normal add DEXMOD",
    "UNARMORED_DEFENSE_MONK normal add WISMOD",
    "AC latest max UNARMORED_DEFENSE_MONK"
]
```
Custom attributes are a nice way to keep each effect's calculations separate and only combine them when and how it is intended.

## Groups
Groups define any kind of related things, that you might want to put into a group for the tool to use.

There are two ways of defining groups and subgroups.

You can define groups and subgroups explicitly in a `groups.json` file using maps and arrays. You should use this for things like musical instruments for example, where you want a selection (usually of strings), but the selection itself does not have any calculation consequences. Only the group or subgroup might have consequences (e.g. having a shield or having medium armor vs. having light armor).

An example:
```jsonc
{
    // for all tools see the general/srd/groups.json files, this is just an excerpt
    "tools": { // a group
        "artisan's tools": [ // a subgroup with strings as possible choices
            "alchemist's supplies", "brewer's supplies"
        ],
        "musical instruments": [
            "bagpipes", "drum", "flute", "lute"
        ],
        // "thieves' tools", <- because you cannot write single values in a map, do this:
        "__no_subgroup__": [ // use "__no_subgroup__" to mark all these strings as tools of no subgroup
            "disguise kit", "forgery kit", "poisoner's kit", "thieves' tools"
        ],
        "vehicles": { // a subgroup that has subgroups itself
            "mounts": [
                "camel", "elephant", "pony", "warhorse"
            ],
            "__no_subgroup__": [ // you can do this for subgroups as well
                "special vehicle"
                // this special vehicle is a vehicle that doesn't belong to any vehicle subgroup
            ]
        }
    }
}
```

You can also define groups implicitly by creating a file `groups/group_name.json`. Thereby you implicitly define a group with the file name (underscores converted to spaces). And in the file, you define the members of this group. You should use this for things like eldritch invocations, where the (sub)group members can be far more complicated than simple strings and selecting one of them can have a large impact.

Because such choices are usually very similar to features that a class or race would grant. Group members are formatted and parsed like [features](#features) except that activations are replaced by prerequisites.
(see one of the `general/srd/groups/` directories for examples)

<!-- TODO: give more information on that -->

If you find that this way of formatting and parsing such groups and their members is not sufficient for your needs, please let me know.


## Anything unclear?

If something about the formatting of the content is not clear, please have a look at the Open Game Content examples (in [imperial units](../content_imperial/) or [metric units](../content_metric/)), hopefully, that helps.

If you are comfortable reading C++ code, another possibility would be to have a look at the test cases for the particular content type you are having problems with e.g. the `spell_parser_test.cpp`.

**Either way**, do not hesitate to create an issue, I am happy to help. And if you can point out where this explanation is incomplete or unclear, I have the opportunity to improve it for future readers.

