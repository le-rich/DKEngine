#pragma once
#ifndef UI_H
#define UI_H

#include <iostream>
#include "System.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "DKWindows.h"  
#include "MenuBar.h"
#include "Scene.h"
#include "Buffers/FrameBuffer.h"

struct GLFWwindow;

class UI : public System {

public:

   UI(Scene* scene, const FrameBuffer* framebuffer, GLFWwindow* glfwWindow);

    const char* GetName() const override {
        return "UI";
    }

    void Update(float deltaTime) override;

    void FixedUpdate() override {
        // Update Loop logic here

    }

private:

   GLFWwindow* window;
   Scene* scene;
   const FrameBuffer* framebuffer;
   GLFWwindow* glfwWindow;
   
};

void framebuffer_size_callback_UI(GLFWwindow* window, int width, int height);

#endif 