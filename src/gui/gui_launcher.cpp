// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context
// creation, etc.) If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs
#include <dnd_config.hpp>

#include "gui_launcher.hpp"

#include <exception>
#include <filesystem>
#include <iostream>

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

#include <gui/gui_app.hpp>
#include <gui/gui_fonts.hpp>

#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

namespace dnd {

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

const char* setup_glfw() {
#if defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
    return glsl_version;
}

static void setup_style() {
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular
    // ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
}

constexpr float font_size = 24.0;

static GuiFonts setup_fonts() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    std::filesystem::path asset_directory = std::filesystem::path(DND_ASSET_DIRECTORY);

    ImFont* main_font = io.Fonts->AddFontFromFileTTF(
        (asset_directory / "Aileron-Regular.otf").string().c_str(), font_size
    );
    IM_ASSERT(main_font != nullptr);
    io.FontDefault = main_font;

    ImFont* bold_font = io.Fonts->AddFontFromFileTTF(
        (asset_directory / "Aileron-Bold.otf").string().c_str(), font_size
    );
    ImFont* italic_font = io.Fonts->AddFontFromFileTTF(
        (asset_directory / "Aileron-Italic.otf").string().c_str(), font_size
    );
    ImFont* bold_italic_font = io.Fonts->AddFontFromFileTTF(
        (asset_directory / "Aileron-BoldItalic.otf").string().c_str(), font_size
    );

    return GuiFonts{
        .regular = main_font,
        .bold = bold_font,
        .italic = italic_font,
        .bold_italic = bold_italic_font,
    };
}

static void setup_imgui_context() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;
}

static void setup_backends(GLFWwindow* window, const char* glsl_version) {
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

static void render(GLFWwindow* window) {
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

static void render_platform_windows() {
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

static void clean_up(GLFWwindow* window) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

// Main code
int launch() {
    glfwSetErrorCallback(glfw_error_callback);

// NOTE: force X11 on Linux - see https://github.com/glfw/glfw/issues/2621
#if defined(__linux__)
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif

    if (!glfwInit()) {
        return 1;
    }
    const char* glsl_version = setup_glfw();

    GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* video_mode = glfwGetVideoMode(primary_monitor);
    int screen_width = video_mode->width * 9 / 10;
    int screen_height = video_mode->height * 9 / 10;

    GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "DnD Campaign Manager", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    setup_imgui_context();
    setup_backends(window, glsl_version);
    setup_style();
    GuiFonts fonts = setup_fonts();

    try {
        GuiApp app(fonts);
        app.initialize();

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            app.render();

            render(window);
            render_platform_windows();
            glfwSwapBuffers(window);
        }
    } catch (const std::exception& e) {
        DND_UNUSED(e);
        std::cerr << "An unhandled error occured: " << e.what() << std::endl;
        clean_up(window);
        return 1;
    }

    clean_up(window);
    return 0;
}

} // namespace dnd
