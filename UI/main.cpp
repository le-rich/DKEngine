#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(int argc, char* argv[])
{
	std::cout << "UI PROJECT" << std::endl;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "DKEngine", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Load GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	bool exampleCheckboxValue = false;
	float exampleSliderValue = 0.0;

	while (!glfwWindowShouldClose(window))
	{
		//Clear buffer so ImGui windows don't smear when dragged/resized
		glClear(GL_COLOR_BUFFER_BIT);

		//New Imgui Frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//opengl drawing goes here

		//New ImGui Window
		ImGui::Begin("Testing ImGui Window");
		ImGui::Text("Body text for window");
		ImGui::Checkbox("Example Checkbox", &exampleCheckboxValue);
		ImGui::SliderFloat("Example Slider", &exampleSliderValue, -1.0f, 1.0f);
		ImGui::End();

		//Second ImGui Window
		ImGui::Begin("Second Window");
		ImGui::Text("Body text for window 2");
		ImGui::Checkbox("Example Checkbox", &exampleCheckboxValue);
		ImGui::SliderFloat("Example Slider", &exampleSliderValue, -1.0f, 1.0f);
		ImGui::End();

		//Render ImGui windows
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}