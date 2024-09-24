#include <iostream>

#include "include/Component.h"
#include "include/Entity.h"
#include "ui.h"
#include "physics.h"
#include "render.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mutex>
#include <thread>


#include <iostream>

#include "Renderer.h"
#include "Primitives.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}



int run_glfw()
{
	// Window setup, Can extract to separate window class
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

	// Apparently the window can only be with one context at a time????
	glfwMakeContextCurrent(window);

	// Load GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// primitive shapes
	// Rendering will move these
	Triangle triangle;
	Square square;

	Renderer renderer = Renderer();

	// We want some check like this visible to the other threads
	// That way those threads will stop once the window closes.
	while (!glfwWindowShouldClose(window))
	{
		// Rendering related call, we can move this to the loop of the rendering thread
		glClear(GL_COLOR_BUFFER_BIT);
		// Rendering related call, we can move this to the loop of the rendering thread
		renderer.Draw(triangle); // draw tri or square

		// The window has two buffers, front and back.
		// This allows us to display the front buffer while writing to the back buffer.
		// Once rendering is done/finished writing to the back buffer, we call this function to swap the front and back buffers
		// This allows us to draw onto the screen without drawing on top of the previous frame.
		// Rendering and UI needs access to this function or a way to tell Core that drawing is done and it's safe to swap.
		glfwSwapBuffers(window);

		// This function processes all events in the event queue, including window and input events.
		// Should be called in the main thread.
		glfwPollEvents();
	}

	// Destroys library, may cause race condition if it gets destroyed while other threads are using it.
	glfwTerminate();
}



void RunPhysics(Physics& physx) {
	while (true) {
		physx.Update();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

void RunUI(UI& ui) {
	while (true) {
		ui.Update();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}



int main(int argc, char* argv[])
{

	std::cout << "Do you know what DK Stands for? Donkey Kong? Nah. Drift King." << std::endl;


	// TODO - By Rendering Team Make this a call to the Render Project
	run_glfw();

	UI* ui = new UI();
	Physics* physx = new Physics();
	//Render render;

	//ui->initialize();
	//physx->initialize();
	//render.initialize();


	//std::thread render_thread(run_glfw);
	std::thread physics_thread(RunPhysics, std::ref(*physx));
	std::thread ui_thread(RunUI, std::ref(*ui));

	//render_thread.join();
	physics_thread.join();
	ui_thread.join();


	return 0;
}

