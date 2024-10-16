#pragma once

#include <string>
#include <memory>
#include <mutex>

#include <glad/glad.h>
#include <GLFW/include/GLFW/glfw3.h>

class Window
{
public:
	Window();
	Window(int pWidth, int pHeight, const char* pTitle, GLFWmonitor* pMonitor = NULL, GLFWwindow* pShare = NULL);

	static void InitWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}

	// The window has two buffers, front and back.
	// This allows us to display the front buffer while writing to the back buffer.
	// Once rendering is done/finished writing to the back buffer, we call this function to swap the front and back buffers
	// This allows us to draw onto the screen without drawing on top of the previous frame.
	// This may or may not need to be thread safe. There's conflicting information about it.
	inline void SwapWindowBuffers()
	{
		std::lock_guard<std::mutex> lock(mMutex);
		glfwSwapBuffers(mWindow);
	}

	// This function processes all events in the event queue, including window and input events.
	// Should be called in the main thread.
	// Callbacks on all the keys that sets key-codes Or prsssdown to true or false.
	inline void PollEvents()
	{
		std::lock_guard<std::mutex> lock(mMutex);
		glfwPollEvents();
	}

	// Sets the OpenGL or OpenGL ES context of the specified window current on the calling thread.
	// That is, only one thread can be associated with one window at a time.
	inline void SetWindowToCurrentThread()
	{
		std::lock_guard<std::mutex> lock(mMutex);
		glfwMakeContextCurrent(mWindow);
	}

	inline void SetKeyCallback(GLFWkeyfun pCallback)
	{
		std::lock_guard<std::mutex> lock(mMutex);
		glfwSetKeyCallback(mWindow, pCallback);
	}

	inline void SetMouseButtonCallback(GLFWmousebuttonfun pCallback)
	{
		std::lock_guard<std::mutex> lock(mMutex);
		glfwSetMouseButtonCallback(mWindow, pCallback);
	}

	// Call back for changing the viewport size of the OpenGL context.
	// Context needs to be syncronized to caller thread.
	inline void SetFramebufferSizeCallback()
	{
		std::lock_guard<std::mutex> lock(mMutex);
		glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
	}

	inline GLFWwindow* GetWindow() { return mWindow; }
	inline int GetWidth() { return mWidth; }
	inline int GetHeight() { return mHeight; }

private:
	std::mutex mMutex;

	int mWidth, mHeight;
	const char* mTitle;
	GLFWmonitor* mMonitor;
	GLFWwindow* mShare;
	GLFWwindow* mWindow;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
};

