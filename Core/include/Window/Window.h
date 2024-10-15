#pragma once

#include <string>
#include <memory>
#include <mutex>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

	inline void SwapWindowBuffers() { std::lock_guard<std::mutex> lock(mMutex); glfwSwapBuffers(mWindow); }
	inline void PollEvents() { std::lock_guard<std::mutex> lock(mMutex); glfwPollEvents(); }

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

