#include <iostream>

#include "Component.h"
#include "Entity.h"
#include "include/ui.h"
#include "Input.h"
#include "Managers/EntityManager.h"
#include "physics.h"
#include "render.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mutex>
#include <thread>

#include "GLTFLoader.h"
#include "Renderer.h"
#include "System.h"
#include <iostream>
#include "Window/Window.h"



class Core {
private:
	std::vector<System*> systems;

public:
	void AddSystem(System* system) {
		systems.push_back(system);
	}

	std::vector<System*> GetSystems() {
		return this->systems;
	}

	void Synchronize() {

	}


};


// TODO: Refactor Window setup to enable concurrent access to the window object for the render thread.
// Theres some experimentation we'll need to do regarding updating objects on the main thread and rendering on the render thread.
// That can wait for the time being.
int run_glfw() {

	Window::InitWindow();
	Window window;
	if (window.GetWindow() == NULL)
	{
		printf("Failed to create GLFW window");
		glfwTerminate();
	}

	window.SetWindowToCurrentThread();
	window.SetKeyCallback(Input::KeyCallback);
	window.SetMouseButtonCallback(Input::MouseButtonCallback);

	// Load GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Set size of framebuffer
	glViewport(0, 0, window.GetWidth(), window.GetHeight());


	window.SetFramebufferSizeCallback();

	std::vector<System*> systems;
	Core* core = new Core();

	UI* ui = new UI();
	Physics* physx = new Physics();
	Renderer* renderer = new Renderer();

	core->AddSystem(ui);
	core->AddSystem(physx);
	core->AddSystem(renderer);

	ui->Initialize();
	physx->Initialize();
	renderer->Initialize();

	// Timing
	double fixedUpdateBuffer = 0.0;
	double FIXED_UPDATE_INTERVAL = 0.016;
	auto previousTime = std::chrono::high_resolution_clock::now();

	// TODO: Refactor to some kind of Asset Manager and/or Scene Hierarchy for renderer to access
	tinygltf::Model gltfModel = GLTFLoader::LoadFromFile("Assets/TestAE/ae86.gltf"); // TODO: Figure out location of assets/non code files within solution
	Mesh testMesh = GLTFLoader::LoadMesh(gltfModel, gltfModel.meshes[0]);
	renderer->testMesh = testMesh;
	// end TODO

	// We want some check like this visible to the other threads
	// That way those threads will stop once the window closes. ### Has to be conditional for main thread ###
	while (!glfwWindowShouldClose(window.GetWindow()))
	{
		// TODO: Create extractions/enums for key presses on whether they would be pressed-down or not,
		// Have them be updated by GLFW callback. This works because glfwpollevents() is a synchronous method that runs all callbacks
		// As long as all components are called after glfwpollevents, behavior should be fine.
		window.PollEvents();
		Input::RunInputListener();

		// TODO: Move to render thread
		// Rendering related calls, we can move these to the loop of the rendering thread
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer->Update(); // draw tri or square
		window.SwapWindowBuffers();

		auto currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsedTime = currentTime - previousTime;

		while (fixedUpdateBuffer >= FIXED_UPDATE_INTERVAL) {
			for (auto system : systems) {
				system->FixedUpdate();
			}
		}

		for (auto system : systems) {
			system->Update();
		}

	}

	// Destroys library, may cause race condition if it gets destroyed while other threads are using it.
	glfwTerminate();

	// Destroy all systems upon closing window
	for (auto sys : systems) {
		sys->Kill();
	}

}


// TODO: Make core class static or singleton
int main(int argc, char* argv[])
{
	// Nobody dare touch this... I'm watching you... ?_?

	std::cout << "Do you know what DK Stands for? Donkey Kong? Nah. Drift King." << std::endl;

	// TODO - By Rendering Team Make this a call to the Render Project

	// Currently has its own while loop blocking main
	run_glfw();

	// Create Window
	// Run Window



	return 0;
}

