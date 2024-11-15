# DnD-Campaign-Manager
A simple tool providing multiple features that are helpful for DMs managing their DnD5e campaigns.

This tool is inspired by DnD management tools such as [D&D Beyond](https://www.dndbeyond.com/) and 
[Foundry Virtual Tabletop](https://foundryvtt.com/) but is not supposed to be a VTT.
Instead, the goal of this tool is to provide functionality for managing characters, spells and monsters of
your DnD5e world while being lightweight and easy to use.

This tool is supposed to be a simple cross-platform tool that allows:
- creation of your own characters, classes, races and spells through JSON files
- access to information about characters and spells at any time (e.g. while DM-ing a session)

Ideas for future improvements include:
- add character-sheet-like view of characters to the GUI app (partially implemented)
- automatic creation of character sheets and other helpful character documents for example as PDFs (partially implemented)
- allow in-app creation of content (as an alternative to manually writing JSON files) in the GUI app
- support 2024 rule changes
- add monsters and NPCs
- support multiclassing
- keep track of a character's resources (such as spell slots, HP, Ki, or exhaustion) while playing

## You want to use the software?

At the moment, the only possibility to use the tool is by
[building it from source code](#building-from-source-code) first.

## You want to contribute?

Please have a look at [this](CONTRIBUTING.md).

## Building from source code

1. Clone the repository
2. Optionally: Testing the code 
   1. Build the `dndmanager_tests` CMake target
   2. Execute the tests using ctest / Run the `dndmanager_tests` executable
3. Running
   1. Build the `dndmanager` Cmake target for the GUI app 
   2. Run the `dndmanager` executable
