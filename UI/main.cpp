#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Callback to adjust the viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Function to create the dockspace and additional windows
void createImGuiWindows() {
    // Setup a dockspace
    ImGuiWindowFlags dockspace_flags = ImGuiWindowFlags_NoDocking;
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    // Begin dockspace layout
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    dockspace_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::Begin("DockSpace Demo", nullptr, dockspace_flags);
    ImGui::PopStyleVar(2);

    // Create DockSpace ID
    ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
    ImGui::End();

    // First time configuration for docking layout
    static bool firstTime = true;
    if (firstTime) {
        firstTime = false;

        // Split the dockspace into 4 nodes (top, left, right, bottom)
        ImGui::DockBuilderRemoveNode(dockspace_id); // Clear any previous layout
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_None); // Recreate the dockspace
        ImGuiID dock_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.25f, nullptr, &dockspace_id);
        ImGuiID dock_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.25f, nullptr, &dockspace_id);
        ImGuiID dock_bottom = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);
        ImGuiID dock_top = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.25f, nullptr, &dockspace_id);

        // Dock windows
        ImGui::DockBuilderDockWindow("Viewport", dock_top);
        ImGui::DockBuilderDockWindow("Hierarchy", dock_left);
        ImGui::DockBuilderDockWindow("Inspector", dock_right);
        ImGui::DockBuilderDockWindow("Browser", dock_bottom);
        ImGui::DockBuilderDockWindow("Console", dock_bottom);
        ImGui::DockBuilderFinish(dockspace_id); // Apply the layout
    }

    // Hierarchy window
    ImGui::Begin("Viewport");
    ImGui::Text("Viewport goes here");
    ImGui::End();

    // Hierarchy window
    ImGui::Begin("Hierarchy");
    ImGui::Text("Hierarchy goes here");
    ImGui::End();

    // Inspector window
    ImGui::Begin("Inspector");
    ImGui::Text("Inspector goes here");
    ImGui::End();

    // Browser window
    ImGui::Begin("Browser");
    ImGui::Text("Browser goes here");
    ImGui::End();

    // Console window
    ImGui::Begin("Console");
    ImGui::Text("Console goes here");
    ImGui::End();
}

int main(int argc, char* argv[]) {
    std::cout << "UI PROJECT" << std::endl;

    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "DKEngine", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 1280, 720);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Start a new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create the ImGui windows
        createImGuiWindows();

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
