#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <thread>
#include <mutex>
#include <ui.hpp>
#include <physics.hpp>
#include <render.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


int run_glfw() {
	 
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

	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}



void run_physics(Physics& physx) {
	while (true) {
		physx.Update();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

void run_ui(UI& ui) {
	while (true) {
		ui.Update();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}



int main(int argc, char* argv[])
{

	std::cout << "Do you know what DK Stands for? Donkey Kong? Nah. Drift King." << std::endl;


	// TODO: Make this a call to the Render Project
	//run_glfw();

	UI ui;
	Physics physx;
	//Render render;

	ui.initialize();
	physx.initialize();
	//render.initialize();


	//std::thread render_thread(run_glfw);
	std::thread physics_thread(run_physics, std::ref(physx));
	std::thread ui_thread(run_ui, std::ref(ui));

	//render_thread.join();
	physics_thread.join();
	ui_thread.join();



	return 0;
}

