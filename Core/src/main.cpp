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
#include "Managers/AudioManager.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <Components/RigidBodyComponent.h>
#include "Components/AudioComponent.h"

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

    Input& input = Input::GetInstance();
    input.SetWindow(window.GetWindow());
    
    // the following is how you can call register a callback THIS IS JUST EXAMPLE CODE FOR PEOPLE TO USE
    /*
    input.RegisterKeyCallback(GLFW_KEY_W, [](Input::ActionType action) {
        if (action == Input::HOLD) {
            std::cout << "W KEY HELD" << std::endl;
        }
        else if (action == Input::RELEASE) {
            std::cout << "W KEY RELEASED" << std::endl;
        }
        else if (action == Input::PRESS) {
            std::cout << "W KEY PRESESD" << std::endl;
        }
        });
    */

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

    Entity* testCarEntity = EntityManager::getInstance().findFirstEntityByDisplayName("Test Car");
    Entity* cameraEnt = EntityManager::getInstance().findFirstEntityByDisplayName("Main Camera");

	TransformComponent* CAR_TRANSFORM = testCarEntity->transform;
   auto glfwWindow = window.GetWindow();

    Physics* physics = new Physics();
    Renderer* renderer = new Renderer(&window);
    UI* ui = new UI(Core::getInstance().GetScene(), renderer->GetFrameBuffer(), glfwWindow);
    Game* game = new Game();
    AudioManager* audioManager = new AudioManager();    
	
	
	Core::getInstance().AddSystem(ui);
	Core::getInstance().AddSystem(physics);
	Core::getInstance().AddSystem(renderer);
	Core::getInstance().AddSystem(game);
	Core::getInstance().AddSystem(audioManager);

	ui->Initialize();
	physics->Initialize();
	renderer->Initialize();
	game->Initialize();
	audioManager->Initialize();
	
    // TODO: componentize all of this, so it isn't hard-coded to two sounds/channels
	FMOD::Sound* backgroundMusic = audioManager->LoadAudio("Assets/Audio/car-motor.mp3");
    FMOD::Sound* audienceSound = audioManager->LoadAudio("Assets/Audio/audience.mp3");
	audioManager->PlayDynamicSound(backgroundMusic, true, {0, 50.0f, 0});
    audioManager->PlayStaticSound(audienceSound, true, { 0, 0.0f, 0 });

    std::thread gameThread([&]()
    {
        auto previousTime = std::chrono::high_resolution_clock::now();
        while (running) 
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> deltaTime = currentTime - previousTime;
            deltaTimeFloatSeconds = deltaTime.count();
            previousTime = currentTime;
            if (!ui->isPaused()) {
               game->Update(deltaTimeFloatSeconds);
            }
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    });
    
    std::thread physicsThread([&]()
        {
            double fixedUpdateBuffer = 0.0;
            auto previousTime = std::chrono::high_resolution_clock::now();
            while (running)
            {
                auto currentTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<float> deltaTime = currentTime - previousTime;
                deltaTimeFloatSeconds = deltaTime.count();
                previousTime = currentTime;
                fixedUpdateBuffer += std::chrono::duration_cast<std::chrono::milliseconds>(deltaTime).count();

                if (fixedUpdateBuffer >= PHYSICS_UPDATE_INTERVAL && !ui->isPaused()) {
                    physics->FixedUpdate();
                    fixedUpdateBuffer -= PHYSICS_UPDATE_INTERVAL;
                }
                std::this_thread::sleep_for(std::chrono::microseconds(1));
                audioManager->updateListenerPosition(cameraEnt->transform->getWorldPosition());
                audioManager->updateSoundPosition(testCarEntity->transform->getWorldPosition());
                audioManager->Update(0.0);
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
            ui->Update(deltaTimeFloatSeconds);
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    });

	// RUN =======================================================
	double fixedUpdateBuffer = 0.0;
	double FIXED_UPDATE_INTERVAL = 20; // in milliseconds
	auto previousTime = std::chrono::high_resolution_clock::now();
	
    while (!glfwWindowShouldClose(glfwWindow))
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - previousTime; 
        deltaTimeFloatSeconds = deltaTime.count();
        previousTime = currentTime;

        window.PollEvents();
        input.Update();


	}

	// TEARDOWN ==================================================
	running = false;
    
    if (gameThread.joinable())      { gameThread.join(); }
    if (physicsThread.joinable())   { physicsThread.join(); }
    if (rendererThread.joinable())  { rendererThread.join(); }

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

