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
        case SessionStatus::CAMPAIGN_SELECTION:
            is_selecting_campaign = true;
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
        content_dir_dialog.SetPwd(session.get_content_directory().parent_path());
    }
    content_dir_dialog.Open();
}

void ContentConfigurationWindow::open_campaign_selection() { is_selecting_campaign = true; }

void ContentConfigurationWindow::render() {
    render_content_dir_selection();
    render_campaign_selection();
}

void ContentConfigurationWindow::render_content_dir_selection() {
    DND_MEASURE_FUNCTION();
    content_dir_dialog.Display();
    Errors errors;
    if (content_dir_dialog.HasSelected()) {
        content_dir_dialog.Close();
        errors = session.set_content_directory(content_dir_dialog.GetSelected());
        if (!errors.ok()) {
            ImGui::OpenPopup("Invalid content directory");
        }
        if (errors.ok() && session.get_status() == SessionStatus::CAMPAIGN_SELECTION) {
            is_selecting_campaign = true;
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

void ContentConfigurationWindow::render_campaign_selection() {
    DND_MEASURE_FUNCTION();
    if (is_selecting_campaign) {
        ImGui::OpenPopup("Select campaign");
    }
    if (ImGui::BeginPopupModal("Select campaign")) {
        bool close = false;
        for (const std::string& possible_campaign_name : session.get_possible_campaign_names()) {
            if (ImGui::Button(possible_campaign_name.c_str())) {
                close = session.set_campaign_name(possible_campaign_name).ok();
            }
        }
        ImGui::Separator();
        if (ImGui::Button("Select other directory")) {
            ImGui::CloseCurrentPopup();
            close = true;
            open_content_directory_selection();
        }
        if (close || ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
            is_selecting_campaign = false;
        }
        ImGui::EndPopup();
    }
}

} // namespace dnd
