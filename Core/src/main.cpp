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

#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>

int run_glfw() {
	std::atomic<bool> running(true);
	float deltaTimeFloatSeconds;

	// WINDOW ================================================== 
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

	// ECS ======================================================
    std::vector<System*> systems;
    Scene* defaultScene = new Scene();

	Core::getInstance().SetScene(defaultScene);
	defaultScene->SpawnSceneDefinition();


	auto testCarUUID = EntityManager::getInstance().findFirstEntityByDisplayName("Test Car");
	Entity* testCarEntity = EntityManager::getInstance().getEntity(testCarUUID);

	TransformComponent* CAR_TRANSFORM = testCarEntity->transform;

    UI* ui = new UI(Core::getInstance().GetScene());
    Physics* physics = new Physics(CAR_TRANSFORM);
    Renderer* renderer = new Renderer(&window);
    Game* game = new Game();

	Core::getInstance().AddSystem(ui);
	Core::getInstance().AddSystem(physics);
	Core::getInstance().AddSystem(renderer);
	Core::getInstance().AddSystem(game);

	ui->Initialize();
	physics->Initialize();
	renderer->Initialize();
	game->Initialize();

    std::thread gameThread([&]()
    {
	    auto previousTime = std::chrono::high_resolution_clock::now();
        while (running) 
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> deltaTime = currentTime - previousTime;
            deltaTimeFloatSeconds = deltaTime.count();
            previousTime = currentTime;

            game->Update(deltaTimeFloatSeconds);
        }
    });

    std::thread rendererThread([&]() 
    {
	    auto previousTime = std::chrono::high_resolution_clock::now();
        while(running) 
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> deltaTime = currentTime - previousTime;
            deltaTimeFloatSeconds = deltaTime.count();
            previousTime = currentTime;

            renderer->Update(deltaTimeFloatSeconds);      
        }
    });

    std::thread uiThread([&]() 
    {
        auto previousTime = std::chrono::high_resolution_clock::now();
        while (running) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> deltaTime = currentTime - previousTime;
            deltaTimeFloatSeconds = deltaTime.count();
            previousTime = currentTime;
            ui->Update(deltaTimeFloatSeconds);
        }
    });

	// RUN =======================================================
	double fixedUpdateBuffer = 0.0;
	double FIXED_UPDATE_INTERVAL = 20; // in milliseconds
	auto previousTime = std::chrono::high_resolution_clock::now();

    auto glfwWindow = window.GetWindow();
    while (!glfwWindowShouldClose(glfwWindow))
    {
		auto currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime = currentTime - previousTime;
		deltaTimeFloatSeconds = deltaTime.count();
		previousTime = currentTime;
		fixedUpdateBuffer += std::chrono::duration_cast<std::chrono::milliseconds>(deltaTime).count();

        window.PollEvents();
        Input::RunInputListener(physics->body);

		while (fixedUpdateBuffer >= FIXED_UPDATE_INTERVAL)
		{
			physics->FixedUpdate();
			fixedUpdateBuffer -= FIXED_UPDATE_INTERVAL;
		}
	}

	// TEARDOWN ==================================================
	running = false;
    
    if (gameThread.joinable())      { gameThread.join(); }
    if (rendererThread.joinable())  { rendererThread.join(); }
    if (uiThread.joinable())        { uiThread.join(); }

	for (auto sys : systems)
	{
		sys->Kill();
	}

	glfwTerminate();
}

int main(int argc, char* argv[])
{
    // Nobody dare touch this... I'm watching you... ?_?
    std::cout << "Do you know what DK Stands for? Donkey Kong? Nah. Drift King." << std::endl;

    run_glfw();

    return 0;
}

