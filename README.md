# DnD-Campaign-Manager
A simple tool providing multiple features that are helpful for DMs managing their DnD5e campaigns.

This tool is inspired by DnD management tools such as [D&D Beyond](https://www.dndbeyond.com/) and [Foundry Virtual Tabletop](https://foundryvtt.com/).
The goal of this tool is to provide similar functionality for managing characters, spells and monsters of your DnD5e world while being lightweight and easy to use.

The first version of this tool is supposed to be a command line tool that allows:
- creation of your own characters, classes, races and spells through JSON files
- access to information about characters and spells at any time (e.g. while DM-ing a session)

Ideas for future improvements include:
- multiclassing
- adding items
- adding monsters and NPCs
- management of XP and level-ups of your characters
- a user interface
- automatic creation of character sheets and other helpful character documents
- keeping track of a character's resources (such as spell slots, HP, Ki, or exhaustion) while playing

## You want to use the software?

At the moment, the only possibility to use the tool is by [building it from source code](#building-from-source-code) first.

## You want to contribute?

Please have a look at [this](CONTRIBUTING.md).

## Building from source code

1. Make sure you have all [required dependencies](#requirements) installed
2. Clone the repository
3. Testing (optional)
   1. Build the CMake target "dndmanager_tests"
   2. Execute the tests using ctest
4. Running
   1. Build the Cmake target "dndmanager"
   2. Run the executable
