#include "Window/Window.h"

Window::Window() : mWidth(800), mHeight(600), mTitle("DKEngine"), mMonitor(NULL), mShare(NULL)
{
	mWindow = glfwCreateWindow(mWidth, mHeight, mTitle, mMonitor, mShare);
}

Window::Window(int pWidth, int pHeight, const char* pTitle, GLFWmonitor* pMonitor, GLFWwindow* pShare)
	: mWidth(pWidth), mHeight(pHeight), mTitle(pTitle), mMonitor(pMonitor), mShare(pShare)
{
	mWindow = glfwCreateWindow(mWidth, mHeight, mTitle, mMonitor, mShare);
}