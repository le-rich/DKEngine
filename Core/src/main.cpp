#include "Component.h"
#include "Components/TransformComponent.h"
#include "Core.h"
#include "Entity.h"
#include "Input.h"
#include "Managers/EntityManager.h"
#include "Game.h"
#include "Physics.h"
#include "Renderer.h"
#include "Scene.h"
#include "System.h"
#include "UI.h"
#include "Utils/IDUtils.h"
#include "Window/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <mutex>
#include <thread>

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
    Scene* defaultScene = new Scene();

	Core::getInstance().SetScene(defaultScene);
	defaultScene->SpawnSceneDefinition();


	auto testCarUUID = EntityManager::getInstance().findFirstEntityByDisplayName("Test Car");
	Entity* testCarEntity = EntityManager::getInstance().getEntity(testCarUUID);

	TransformComponent* CAR_TRANSFORM = testCarEntity->transform;

    window.SetWindowToCurrentThread();
    Renderer* renderer = new Renderer(&window);
    Physics* physx = new Physics(CAR_TRANSFORM);
    UI* ui = new UI(Core::getInstance().GetScene());
    Game* game = new Game();

	Core::getInstance().AddSystem(ui);
	Core::getInstance().AddSystem(physx);
	Core::getInstance().AddSystem(renderer);
	Core::getInstance().AddSystem(game);

	ui->Initialize();
	renderer->Initialize();
	physx->Initialize();
	game->Initialize();

	// Timing
	double fixedUpdateBuffer = 0.0;
	double FIXED_UPDATE_INTERVAL = 20; // in milliseconds
	auto previousTime = std::chrono::high_resolution_clock::now();

    // We want some check like this visible to the other threads
    // That way those threads will stop once the window closes. ### Has to be conditional for main thread ###
    auto glfwWindow = window.GetWindow();
    while (!glfwWindowShouldClose(glfwWindow))
    {
        // TODO: Refactor to proper input handler
        if (Input::keys[GLFW_KEY_W])
            physx->body->addForce(AE86::Vector3(-1.0, 0.0, 0.0));
        if (Input::keys[GLFW_KEY_S])
            physx->body->addForce(AE86::Vector3(1.0, 0.0, 0.0));

		auto currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime = currentTime - previousTime;
		auto deltaTimeFloatSeconds = deltaTime.count();

		previousTime = currentTime;

		fixedUpdateBuffer += std::chrono::duration_cast<std::chrono::milliseconds>(deltaTime).count();
		std::cout << "COUNT: " << fixedUpdateBuffer << "\n"; // Commenting this line causes the Fixed update loop to stutter.


        // Callbacks on all the keys that sets key-codes Or prsssdown to true or false.
        // Potential Mouse inputs; May have to figure out how it can work when extracting
        // Scrollwheels
        
        // TODO: Create extractions/enums for key presses on whether they would be pressed-down or not,
        // Have them be updated by GLFW callback. This works because glfwpollevents() is a synchronous method that runs all callbacks
        // As long as all components are called after glfwpollevents, behavior should be fine.
        window.PollEvents();
        Input::RunInputListener();

        // Rendering related calls, we can move these to the loop of the rendering thread
        renderer->Update(deltaTimeFloatSeconds); // draw tri or square

        while (fixedUpdateBuffer >= FIXED_UPDATE_INTERVAL)
        {
            physx->FixedUpdate();
            fixedUpdateBuffer -= FIXED_UPDATE_INTERVAL;
        }
        game->Update(deltaTimeFloatSeconds);

        //@TODO: REMOVE THIS LATER. Above loops are never getting entered so UI update was never getting called. Remove this line below when fixed.
        ui->Update(deltaTimeFloatSeconds);
    }

	// Destroys library, may cause race condition if it gets destroyed while other threads are using it.
	glfwTerminate();

	// Destroy all systems upon closing window
	for (auto sys : systems)
	{
		sys->Kill();
	}

}


// TODO: Make core class static or singleton
int main(int argc, char* argv[])
{
    // Nobody dare touch this... I'm watching you... ?_?
    std::cout << "Do you know what DK Stands for? Donkey Kong? Nah. Drift King." << std::endl;

    // Currently has its own while loop blocking main
    run_glfw();

    return 0;
}

