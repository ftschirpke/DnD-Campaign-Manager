#include <dnd_config.hpp>

#include "error_messages_window.hpp"

#include <imgui/imgui.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/session.hpp>
#include <core/storage_content_library.hpp>

namespace dnd {

ErrorMessagesWindow::ErrorMessagesWindow(Session& session) : session(session) {}

static void render_session_error_messages(const char* label, const std::vector<std::string>& messages) {
    if (messages.empty()) {
        return;
    }
    ImGui::SeparatorText(label);
    for (const std::string& message : messages) {
        ImGui::TextWrapped("%s", message.c_str());
    }
}

template <typename T>
static void render_draft_error_messages(const char* label, const StorageContentLibrary<T>& library) {
    if (library.get_drafts().empty()) {
        return;
    }
    ImGui::SeparatorText(label);
    for (const auto& [_, errors] : library.get_drafts()) {
        for (const Error& error : errors.get_errors()) {
            const std::string& message = std::visit([](const auto& e) { return e.get_error_message(); }, error);
            ImGui::TextWrapped("%s", message.c_str());
        }
    }
}

void ErrorMessagesWindow::render() {
    DND_MEASURE_FUNCTION();
    ImGui::Begin("Errors");
    render_session_error_messages("Unknown errors", session.get_unknown_error_messages());
    render_session_error_messages("Parsing errors", session.get_parsing_error_messages());
    render_session_error_messages("Validation errors", session.get_validation_error_messages());
    render_draft_error_messages("Invalid characters", session.get_content().get_character_library());
    render_draft_error_messages("Invalid classes", session.get_content().get_class_library());
    render_draft_error_messages("Invalid subclasses", session.get_content().get_subclass_library());
    render_draft_error_messages("Invalid species", session.get_content().get_species_library());
    render_draft_error_messages("Invalid subspecies", session.get_content().get_subspecies_library());
    render_draft_error_messages("Invalid Items", session.get_content().get_item_library());
    render_draft_error_messages("Invalid Spells", session.get_content().get_spell_library());
    render_draft_error_messages("Invalid Choosables", session.get_content().get_choosable_library());
    ImGui::End();
}

} // namespace dnd
