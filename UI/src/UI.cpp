#include "../include/ui.h"

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

UI::UI(Scene* scene) {
   std::cout << "UI PROJECT" << std::endl;

   this->scene = scene;

   // Initialize GLFW (Already initialized in main)
   //glfwInit();
   //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   window = glfwCreateWindow(1280, 720, "DKEngine", NULL, NULL);
   if (window == NULL) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return;
   }
   //glfwMakeContextCurrent(window);

   //// Load GLAD (Already loaded in main)
   //if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
   //   std::cout << "Failed to initialize GLAD" << std::endl;
   //   return;
   //}

   
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_UI);

   // Initialize ImGui
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGuiIO& io = ImGui::GetIO();
   io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
   ImGui::StyleColorsDark();
   ImGui_ImplGlfw_InitForOpenGL(window, true);
   ImGui_ImplOpenGL3_Init("#version 130");

   // Main loop


   // Cleanup
   //ImGui_ImplOpenGL3_Shutdown();
   //ImGui_ImplGlfw_Shutdown();
   //ImGui::DestroyContext();
   //glfwTerminate();
   //return 0;
}

void UI::Update(float deltaTime) {
   // Update Loop logic here
   std::cout << "UI Update" << std::endl;
   //while (!glfwWindowShouldClose(window)) {
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
   createImGuiWindows(this->scene);

   // Render ImGui
   ImGui::Render();
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

   // Swap buffers and poll events
   glfwSwapBuffers(window);
   glfwPollEvents();
   //}

}

void framebuffer_size_callback_UI(GLFWwindow* window, int width, int height) {
   glViewport(0, 0, width, height);
}