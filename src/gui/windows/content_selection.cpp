#include <dnd_config.hpp>

#include "content_selection.hpp"

#include <imgui/imgui.h>
#include <imgui_filebrowser/imfilebrowser.h>

#include <core/errors/errors.hpp>
#include <core/session.hpp>

static constexpr ImGuiFileBrowserFlags file_dialog_flags = ImGuiFileBrowserFlags_SelectDirectory
                                                           | ImGuiFileBrowserFlags_HideRegularFiles;

static const ImGuiWindowFlags error_popup_options = ImGuiWindowFlags_AlwaysAutoResize;

dnd::ContentSelection::ContentSelection(Session& session)
    : session(session), content_dir_dialog(file_dialog_flags), is_selecting_campaign(false) {}

void dnd::ContentSelection::initialize() {
    content_dir_dialog.SetTitle("Select content directory");
    content_dir_dialog.SetWindowSize(1200, 900);

    switch (session.get_status()) {
        case SessionStatus::CONTENT_DIR_SELECTION:
            open_content_dir_dialog();
            break;
        case SessionStatus::CAMPAIGN_SELECTION:
            is_selecting_campaign = true;
            break;
        default:
            break;
    }
}

void dnd::ContentSelection::select_content_directory() { open_content_dir_dialog(); }

void dnd::ContentSelection::select_campaign() { is_selecting_campaign = true; }

void dnd::ContentSelection::render() {
    render_content_dir_selection();
    render_campaign_selection();
}

void dnd::ContentSelection::open_content_dir_dialog() {
    if (content_dir_dialog.HasSelected()) {
        content_dir_dialog.SetPwd(content_dir_dialog.GetSelected().parent_path());
    } else {
        content_dir_dialog.SetPwd(session.get_content_directory().parent_path());
    }
    content_dir_dialog.Open();
}

void dnd::ContentSelection::render_content_dir_selection() {
    DND_MEASURE_FUNCTION();
    content_dir_dialog.Display();
    if (content_dir_dialog.HasSelected()) {
        content_dir_dialog.Close();
        bool valid = session.set_content_directory(content_dir_dialog.GetSelected()).ok();
        if (!valid) {
            ImGui::OpenPopup("Invalid content directory");
        }
    }

    if (ImGui::BeginPopupModal("Invalid content directory", nullptr, error_popup_options)) {
        ImGui::Text("Selected directory: %s", content_dir_dialog.GetSelected().string().c_str());
        if (ImGui::Button("Select other directory")) {
            ImGui::CloseCurrentPopup();
            open_content_dir_dialog();
        }
        ImGui::EndPopup();
    }
}

void dnd::ContentSelection::render_campaign_selection() {
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
            open_content_dir_dialog();
        }
        if (close || ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
            is_selecting_campaign = false;
        }
        ImGui::EndPopup();
    }
}
