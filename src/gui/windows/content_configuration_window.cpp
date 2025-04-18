#include <dnd_config.hpp>

#include "content_configuration_window.hpp"

#include <imgui/imgui.h>
#include <imgui_filebrowser/imfilebrowser.h>

#include <core/errors/errors.hpp>
#include <core/session.hpp>

namespace dnd {

static constexpr ImGuiFileBrowserFlags file_dialog_flags = ImGuiFileBrowserFlags_SelectDirectory
                                                           | ImGuiFileBrowserFlags_HideRegularFiles;

static const ImGuiWindowFlags error_popup_options = ImGuiWindowFlags_AlwaysAutoResize;

ContentConfigurationWindow::ContentConfigurationWindow(Session& session)
    : session(session), content_dir_dialog(file_dialog_flags), is_selecting_campaign(false) {}

void ContentConfigurationWindow::initialize() {
    content_dir_dialog.SetTitle("Select content directory");
    content_dir_dialog.SetWindowSize(1200, 900);

    switch (session.get_status()) {
        case SessionStatus::CONTENT_DIR_SELECTION:
            open_content_directory_selection();
            return;
        case SessionStatus::PARSING:
        case SessionStatus::READY:
        case SessionStatus::UNKNOWN_ERROR:
            return;
    }
    assert(false);
}

void ContentConfigurationWindow::open_content_directory_selection() {
    if (content_dir_dialog.HasSelected()) {
        content_dir_dialog.SetPwd(content_dir_dialog.GetSelected().parent_path());
    } else {
        content_dir_dialog.SetPwd(session.get_content_directories().begin()->parent_path());
    }
    content_dir_dialog.Open();
}

void ContentConfigurationWindow::open_campaign_selection() { is_selecting_campaign = true; }

void ContentConfigurationWindow::render() { render_content_dir_selection(); }

void ContentConfigurationWindow::render_content_dir_selection() {
    DND_MEASURE_FUNCTION();
    content_dir_dialog.Display();
    Errors errors;
    if (content_dir_dialog.HasSelected()) {
        content_dir_dialog.Close();
        errors = session.add_content_directory(content_dir_dialog.GetSelected());
        if (!errors.ok()) {
            ImGui::OpenPopup("Invalid content directory");
        }
    }

    if (ImGui::BeginPopupModal("Invalid content directory", nullptr, error_popup_options)) {
        ImGui::Text("Selected directory: %s", content_dir_dialog.GetSelected().string().c_str());
        for (const Error& error : errors.get_errors()) {
            std::visit([](const auto& err) { ImGui::Text("Error: %s", err.get_error_message().c_str()); }, error);
        }
        if (ImGui::Button("Select other directory")) {
            ImGui::CloseCurrentPopup();
            open_content_directory_selection();
        }
        ImGui::EndPopup();
    }
}

} // namespace dnd
