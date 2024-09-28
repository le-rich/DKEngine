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
#include "System.h"




class Core {
private:
	std::vector<System*> systems;
	
public:
	void AddSystem(System* system) {
		systems.push_back(system);
	}
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


// TODO: Refactor Window setup to enable concurrent access to the window object for the render thread.
// Theres some experimentation we'll need to do regarding updating objects on the main thread and rendering on the render thread.
// That can wait for the time being.
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

	// This function makes the OpenGL or OpenGL ES context of the specified window current on the calling thread.
	// That is, only one thread can be associated with one window at a time.
	// This'll likely be the cause of a lot of issues with rendering and ui.
	glfwMakeContextCurrent(window);

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

    // primitive shapes
    Triangle triangle;
    Square square;

	Renderer renderer = Renderer();

	// We want some check like this visible to the other threads
	// That way those threads will stop once the window closes.
	while (!glfwWindowShouldClose(window))
	{
		// Rendering related calls, we can move these to the loop of the rendering thread
		glClear(GL_COLOR_BUFFER_BIT);
		renderer.Draw(triangle); // draw tri or square

		// The window has two buffers, front and back.
		// This allows us to display the front buffer while writing to the back buffer.
		// Once rendering is done/finished writing to the back buffer, we call this function to swap the front and back buffers
		// This allows us to draw onto the screen without drawing on top of the previous frame.
		// This may or may not need to be thread safe. There's conflicting information about it.
		glfwSwapBuffers(window);

		// This function processes all events in the event queue, including window and input events.
		// Should be called in the main thread.
		glfwPollEvents();
	}

	// Destroys library, may cause race condition if it gets destroyed while other threads are using it.
	glfwTerminate();
}


int main(int argc, char* argv[])
{
	// Nobody dare touch this... I'm watching you... ?_?

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
	//run_glfw();



	std::vector<System*> system;
	Core* core = new Core();


	UI* ui = new UI();
	Physics* physx = new Physics();
	
	core->AddSystem(ui);
	core->AddSystem(physx);

	ui->Initialize(1000.0);
	physx->Initialize(10.0);

	while (ui->IsActive() || physx->IsActive()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}

