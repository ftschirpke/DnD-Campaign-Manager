# DnD-Campaign-Manager
A simple tool providing multiple features that are helpful for DMs managing their DnD5e campaigns.

This tool is inspired by DnD management tools such as [D&D Beyond](https://www.dndbeyond.com/) and 
[Foundry Virtual Tabletop](https://foundryvtt.com/).
The goal of this tool is to provide similar functionality for managing characters, spells and monsters of
your DnD5e world while being lightweight and easy to use.

This tool is supposed to be a simple cross-platform tool that allows:
- creation of your own characters, classes, races and spells through JSON files
- access to information about characters and spells at any time (e.g. while DM-ing a session)

Ideas for future improvements include:
- add character-sheet-like view of characters to the GUI app
- add monsters and NPCs
- allow in-app creation of content (as an alternative to manually writing JSON files) in the GUI app
    - keep track of a character's resources (such as spell slots, HP, Ki, or exhaustion) while playing
- support multiclassing
- automatic creation of character sheets and other helpful character documents for example as PDFs

## You want to use the software?

At the moment, the only possibility to use the tool is by
[building it from source code](#building-from-source-code) first.

## You want to contribute?

Please have a look at [this](CONTRIBUTING.md).

## Building from source code

1. Clone the repository
2. Testing (optional)
   1. Build the CMake target "dndmanager_tests"
   2. Execute the tests using ctest
3. Running
   1. Build the Cmake target "dndmanager_gui" for the GUI app, or alternatively the "dndmanager_cli" target for the
      CLI app.
   2. Run the executable
