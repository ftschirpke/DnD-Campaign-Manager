#include "launcher.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cxxopts/cxxopts.hpp>

#include "helper/argv.hpp"

TEST_CASE("dnd::launch: the program is run with the -h argument") {
    Argv argv_obj({"dndmanager", "-h"});
    int argc = argv_obj.argc();
    char** argv = argv_obj.argv();

    REQUIRE_NOTHROW(dnd::launch(argc, argv));
    REQUIRE(dnd::launch(argc, argv) == 0);
}

TEST_CASE("dnd::launch: the program is run with the -v argument") {
    Argv argv_obj({"dndmanager", "-v"});
    int argc = argv_obj.argc();
    char** argv = argv_obj.argv();

    REQUIRE_NOTHROW(dnd::launch(argc, argv));
    REQUIRE(dnd::launch(argc, argv) == 0);
}

TEST_CASE("dnd::launch: the program is run without a -c argument") {
    Argv argv_obj({"dndmanager"});
    int argc = argv_obj.argc();
    char** argv = argv_obj.argv();

    REQUIRE_NOTHROW(dnd::launch(argc, argv));
    REQUIRE(dnd::launch(argc, argv) != 0);
}

TEST_CASE("dnd::launch: the program is run without empty -c argument") {
    Argv argv_obj({"dndmanager", "-c", "\"\""});
    int argc = argv_obj.argc();
    char** argv = argv_obj.argv();

    REQUIRE_NOTHROW(dnd::launch(argc, argv));
    REQUIRE(dnd::launch(argc, argv) != 0);
}
