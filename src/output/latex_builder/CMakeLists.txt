target_sources(${DND_APP_TARGET}
    PRIVATE
    latex_command.cpp
    latex_document.cpp
    latex_scope.cpp
    latex_text.cpp
)

target_sources(${DND_TEST_TARGET}
    PRIVATE
    latex_command.cpp
    latex_document.cpp
    latex_scope.cpp
    latex_text.cpp
)
