# Content

This directory is where you store all your content i.e. definitions of classes, races, spells, items, and monsters.

Everything that applies to all of your campaigns, such as the SRD classes and races is placed in the `general` directory. Whereas everything related to the campaign itself is placed within its own folder for the campaign.

As an example, the content from SRD is formatted in this way inside of the `content_imperial` and `content_metric` directories. **Be aware:** They are just an example, all rights belong to WotC, and as an example they are not guaranteed to be correct, complete, or up-to-date.

Furthermore, a few further examples using the SRD content are provided in the `example_campaign` directory. This directory should also show you, how you can create content for your own campaign and how to place it in a directory so that the DnD-Campaign-Manager can use it.

Within the `content_imperial` directory, you can find the original content with imperial units. I did my best to convert all imperial units into metric units. You can find this adapted content in the `content_metric` directory.

When you choose that your content directory has another name, remember to provide the directory name to the executable using `-d directory_name`.

## Format

All of your content is specified through JSON files.

### Races
TODO

### Classes
TODO

### Spells
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

### Items
TODO

### Monsters
TODO

### Anything unclear?

If something about the formatting of the content is not clear, please have a look at the SRD content, hopefully, that helps.

If you are comfortable reading C++ code, another possibility would be to have a look at the test cases for the particular content type you are having problems with e.g. the `spell_parser_test.cpp`.

**Either way**, do not hesitate to create an issue, I am happy to help. And if you can point out where this explanation is incomplete or unclear, I have the opportunity to improve it for future readers.

