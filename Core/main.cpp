#include <iostream>

#include "include/Component.h"
#include "include/Entity.h"
#include "Managers/EntityManager.h"
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

    // primitive shapes
    Triangle triangle;
    Square square;

	Renderer renderer = Renderer();

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		renderer.Draw(triangle); // draw tri or square
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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
	
	/* UUIDgen testing code
	UUIDv4::UUIDGenerator<std::mt19937_64> uuidGen;
	Entity* e1 = new Entity();
	UUIDv4::UUID uuid = uuidGen.getUUID();
	UUIDv4::UUID uuid2 = uuidGen.getUUID();

	std::cout << "random uuid 1:" << std::endl;
	std::cout << uuid << std::endl;
	std::cout << "random uuid 2:" << std::endl;
	std::cout << uuid2 << std::endl;
;

	e1->SetEntityID(uuid);
	std::cout << "entiy 1 with uuid 1" << std::endl;
	std::cout << e1->GetEntityID() << std::endl;
	*/

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

