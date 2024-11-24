#include "../include/ui.h"

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

UI::UI(Scene* scene, const FrameBuffer* framebuffer, GLFWwindow* glfwWindow) {

   this->scene = scene;
   this->framebuffer = framebuffer;
   this->glfwWindow = glfwWindow;

   window = glfwCreateWindow(1280, 720, "DKEngine", NULL, glfwWindow);
   if (window == NULL) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return;
   }

   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_UI);

   // Initialize ImGui
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGuiIO& io = ImGui::GetIO();
   io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
   ImGui::StyleColorsDark();
   ImGui_ImplGlfw_InitForOpenGL(window, true);
   ImGui_ImplOpenGL3_Init("#version 130");

}

void UI::Update(float deltaTime) {
   // Update Loop logic here

   glfwMakeContextCurrent(window);
   // Clear the buffer
   glClear(GL_COLOR_BUFFER_BIT);

   // Start a new ImGui frame
   ImGui_ImplOpenGL3_NewFrame();
   ImGui_ImplGlfw_NewFrame();
   ImGui::NewFrame();

   // Create the menu bar
   createMenuBar();

   // Create the ImGui windows
   createImGuiWindows(this->scene, this->framebuffer);

   // Render ImGui
   ImGui::Render();
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

   // Swap buffers and poll events
   glfwSwapBuffers(window);
   glfwPollEvents();

   if (glfwWindowShouldClose(window)) {
      glfwDestroyWindow(window);
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();
      glfwTerminate();

      glfwSetWindowShouldClose(glfwWindow, 0);
   }

}

void framebuffer_size_callback_UI(GLFWwindow* window, int width, int height) {
   glViewport(0, 0, width, height);
}