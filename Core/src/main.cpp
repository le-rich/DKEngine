#include <iostream>
#include <mutex>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core.h"
#include "Component.h"
#include "Entity.h"
#include "include/ui.h"
#include "Input.h"
#include "Managers/EntityManager.h"
#include "physics.h"
#include "Scene.h"
#include "Utils/IDUtils.h"

#include "Components/Transform.h"
#include "GLTFLoader.h"
#include "Renderer.h"
#include "System.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


// TODO: Refactor Window setup to enable concurrent access to the window object for the render thread.
// Theres some experimentation we'll need to do regarding updating objects on the main thread and rendering on the render thread.
// That can wait for the time being.
int run_glfw()
{

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

    // This function makes the OpenGL or OpenGL ES context of the specified window current on the calling thread.
    // That is, only one thread can be associated with one window at a time.
    // This'll likely be the cause of a lot of issues with rendering and ui.
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, Input::KeyCallback);
    glfwSetMouseButtonCallback(window, Input::MouseButtonCallback);

    // Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    // Call back for changing the viewport size of the OpenGL context.
    // Context needs to be syncronized to caller thread.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    std::vector<System*> systems;
    Scene* defaultScene = new Scene();

    Core::getInstance().SetScene(defaultScene);
    defaultScene->SpawnSceneDefinition();

    // TODO: Refactor to a Single Car Entity with Mesh and Rigidbody components
    // TODO: Refactor to some kind of Asset Manager and/or Scene Hierarchy for renderer to access
    std::string SOURCE_FOLDER = "Assets/TestAE/"; // TODO: JSONparser for list of assets, each asset has a PATH and FILE string.
    Entity testCar;
    tinygltf::Model gltfModel = GLTFLoader::LoadFromFile(SOURCE_FOLDER + "ae86.gltf"); // TODO: Figure out location of assets/non code files within solution
    std::vector<UUIDv4::UUID> textures = GLTFLoader::LoadTextures(gltfModel, SOURCE_FOLDER);
    std::vector<UUIDv4::UUID> materials = GLTFLoader::LoadMaterials(gltfModel, textures);
    Mesh testMesh = GLTFLoader::LoadMesh(gltfModel, gltfModel.meshes[0], materials);
    Transform* CAR_TRANSFORM = new Transform(&testCar);
    // end TODO
    EntityManager::getInstance().Instantiate(&testCar);

    UI* ui = new UI();
    Physics* physx = new Physics(CAR_TRANSFORM);
    Renderer* renderer = new Renderer();

    Core::getInstance().AddSystem(ui);
    Core::getInstance().AddSystem(physx);
    Core::getInstance().AddSystem(renderer);

    ui->Initialize();
    physx->Initialize();
    renderer->Initialize();

    // Timing
    double fixedUpdateBuffer = 0.0;
    double FIXED_UPDATE_INTERVAL = 20; // in milliseconds
    auto previousTime = std::chrono::high_resolution_clock::now();

    // TODO: Refactor to getting Scene instance
    renderer->testMesh = testMesh;
    //renderer->testTextures = textures;
    renderer->testMaterials = materials;
    renderer->testTransform = CAR_TRANSFORM;

    // end TODO

    // We want some check like this visible to the other threads
    // That way those threads will stop once the window closes. ### Has to be conditional for main thread ###
    while (!glfwWindowShouldClose(window))
    {
        // TODO: Refactor to proper input handler
        if (Input::keys[GLFW_KEY_W])
            physx->body->addForce(AE86::Vector3(-1.0, 0.0, 0.0));
        if (Input::keys[GLFW_KEY_S])
            physx->body->addForce(AE86::Vector3(1.0, 0.0, 0.0));

        auto currentTime = std::chrono::high_resolution_clock::now();

        auto elapsedTime = currentTime - previousTime;

        previousTime = currentTime;

        fixedUpdateBuffer += std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count();
        std::cout << "COUNT: " << fixedUpdateBuffer << "\n"; // Commenting this line causes the Fixed update loop to stutter.

        Input::RunInputListener();
        glfwMakeContextCurrent(window);
        // Rendering related calls, we can move these to the loop of the rendering thread
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer->Update(); // draw tri or square

        // The window has two buffers, front and back.
        // This allows us to display the front buffer while writing to the back buffer.
        // Once rendering is done/finished writing to the back buffer, we call this function to swap the front and back buffers
        // This allows us to draw onto the screen without drawing on top of the previous frame.
        // This may or may not need to be thread safe. There's conflicting information about it.
        glfwSwapBuffers(window);

        // This function processes all events in the event queue, including window and input events.
        // Should be called in the main thread.
        glfwPollEvents();
        // Callbacks on all the keys that sets key-codes Or prsssdown to true or false.
        // Potential Mouse inputs; May have to figure out how it can work when extracting
        // Scrollwheels

        // TODO: Create extractions/enums for key presses on whether they would be pressed-down or not,
        // Have them be updated by GLFW callback. This works because glfwpollevents() is a synchronous method that runs all callbacks
        // As long as all components are called after glfwpollevents, behavior should be fine.

        while (fixedUpdateBuffer >= FIXED_UPDATE_INTERVAL)
        {
            physx->FixedUpdate();
            /*	for (auto system : systems) {
                    system->FixedUpdate();
                }*/

            fixedUpdateBuffer -= FIXED_UPDATE_INTERVAL;
            //CAR_TRANSFORM->mtx.lock();
            //glm::vec3 carPos = CAR_TRANSFORM->getLocalPosition();
            //std::cout << "MAIN LOOP - CAR TRANSFORM POSITION: " << carPos.x << ", " << carPos.y << ", " << carPos.z << "\n";
            //CAR_TRANSFORM->mtx.unlock();
        }

        for (auto system : systems)
        {
            system->Update();
        }

        //REMOVE THIS LATER. Above loops are never getting entered so UI update was never getting called. Remove this line below when fixed.
        ui->Update();
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

    // TODO - By Rendering Team Make this a call to the Render Project

    // Currently has its own while loop blocking main
    run_glfw();



    return 0;
}

