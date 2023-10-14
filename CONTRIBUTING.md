# Contributing
- [Contributing](#contributing)
  - [Requirements](#requirements)
  - [Format](#format)
  - [Comments](#comments)
  - [Style](#style)
    - [File Names](#file-names)
    - [Includes](#includes)
    - [Header files](#header-files)
    - [Implementation files](#implementation-files)
  - [Commit types](#commit-types)

I am excited you are considering contributing to this project.

**Please contact me on GitHub or preferably [via email](mailto:friedrich.tschirpke@student.hu-berlin.de).**

This is especially important because the project is still in its beginnings and only managed by myself right now.
Thus, the documentation is lacking. Although I am trying my best to always document my ideas in markdown files and
list all the TODOs, I understand that it might be very hard to understand anything in this project.
There are probably many ideas which only exist in my head and I forgot to explain them anywhere. 

So, **do not hesitate to ask questions.**

I would be happy to clarify and explain my ideas. I am also open to any ideas to improve the project if you have any.

## Requirements

You need a C++ compiler and [CMake](https://cmake.org/) to build the project.
Additionally, you need to have X11 installed.
On Windows and MacOS it is installed by default, on Linux distributions you might to install it yourself.

## Format

Please use the format defined by the [clang-format file](.clang-format) as much as possible.
You may deviate from this format whenever doing so greatly improves the readability of the code.
However, this should be the exception.
In most cases changing the [clang-format file](.clang-format) itself might be the better solution.

## Comments

Please try to add at least a few words of explanation to every piece of code that is either hard to understand or
part of the public interface of a class.
I usually try to add a short explanation to protected and private class members as well.
I generally try to write a short comment for everything in a header file to reduce the amount a developer that
does not know my code needs to look up what things do and instead only needs to read a header file or
maybe their code editor even displays the comment for them directly.

For classes, structs, and enums, I use a small java-style doxygen comment:

```c++
/**
 * @brief An empty class
 */
class MyClass {};
```
For functions, I use the java-style doxygen comments as well:
```c++
/**
 * @brief Returns the given value, unless its zero then an exception is thrown
 * @param a some parameter without use
 * @return the input parameter a
 * @throws std::invalid_argument if a is zero
 */
int my_function(int a) {
    if (a == 0) {
        throw std::invalid_argument("Argument cannot be zero.");
    }
    return a;
}
```
And for (member) variables, I just use a single-line comment because that usually suffices
```c++
/**
 * @brief A class to represent animals
 */
class Animal {
    // the name of the animal
    int name;
};
```

So, please try to add comments to your code, and if you don't, please make clear that this still needs to be done
(by writing a sentence in your PR or opening an issue).

## Style

Please abide by the style guidelines below.

Besides that, try to write readable code that abides by the common points listed in popular style guides
e.g. [Google's C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

### File Names
Source code files should have the extensions `.cpp` and `.hpp`.
The names of the files should be descriptive but not too long. For files that only define one class,
which is the preferred way of doing things, the file name should be the "snake_case" version of the class name
(which should be in "PascalCase").

Tests should be implemented in files that correspond to the file it tests both in name and directory structure.
The test file for an implementation `src/path/to/implementation_file.cpp`
should be `tests/path/to/implementation_file_test.cpp`.

### Includes
Each file should include everything that it explicitly uses, even in implementation files where the corresponding
header file already imports the header.

Includes should be separated into multiple include-blocks which are each sorted and separated with newlines.

- first line: include config file that contains preprocessor macros (only in implementation files)
- second line: include header file in implementation files or the implementation files in test files
- first block: include standard library headers (format `<header_name>`)
- second block: include other library headers (format `<header_name.hpp>`)
- third block: include header files from this code base with their path relative to the `src/` directory and
  *not with relative paths* (format `<header_name.hpp>`)

An example:
```c++
#include <dnd_config.hpp>

#include "my_class_parser.hpp"

#include <iostream>
#include <string>
#include <vector>

#include <nlohann/json.hpp>

#include <models/my_class.hpp>
#include <models/another_class.hpp>
#include <parsing/models/another_class_parser.hpp>

// the implementation of MyClassParser
```
### Header files
Header files should have the following format:

(file_name.hpp)
```c++
#ifndef FILE_NAME_HPP_
#define FILE_NAME_HPP_

// includes go here

namespace dnd {

// definitions of constants and classes go here

// implementations of inline functions go here

}

#endif // FILE_NAME_HPP_
```
Notice, that implementations of **all** inline functions should be separated from the class definition to keep
the definition of the interface clean. This also applies to constructors.

For example:
```c++
class MyClass: {
public:
    MyClass(const std::string& name);
    void print() const;
private:
    std::string name;
};

inline MyClass::MyClass(const std::string& name) : name(name) {}

inline void MyClass::print() const { std::cout << name << '\n'; }
```

### Implementation files
As explained [above](#includes), implementation files should include everything that they explicitly use,
even when the header file already includes the same header.

In Implementation files, there should be no namespace-block.
Instead, write `dnd::MyClass::my_function` when implementing a function "my_function" of the class "MyClass"
which was defined inside this code base (therefore being in the "dnd" namespace).

## Commit types

- feat: The new feature you're adding to a particular application
- fix: A bug fix
- style: Feature and updates related to styling
- refactor: Refactoring a specific section of the codebase
- test: Everything related to testing
- docs: Everything related to documentation
- chore: Regular code maintenance.
