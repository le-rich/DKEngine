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





class Core {
private:
	std::vector<System*> systems;
	
public:
	
	template<typename T, typename... Args>
	void AddSystem(T* system, Args*... args) {
		RegisterSystem(system);
		(RegisterSystem(args), ...);
	}
};

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
	// Nobody dare touch this... I'm watching you... ?_?

	std::cout << "Do you know what DK Stands for? Donkey Kong? Nah. Drift King." << std::endl;


	// TODO - By Rendering Team Make this a call to the Render Project
	run_glfw();



	std::vector<System*> system;
	Core* core = new Core();


	UI* ui = new UI();
	Physics* physx = new Physics();
	
	core->AddSystem(ui, physx);


	//ui->initialize();
	//physx->initialize();
	//render.initialize();


	std::thread physics_thread(RunPhysics, std::ref(*physx));
	std::thread ui_thread(RunUI, std::ref(*ui));

	//render_thread.join();
	physics_thread.join();
	ui_thread.join();


	return 0;
}

