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
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class UI : public System {

public:

   UI();

    const char* GetName() const override {
        return "UI";
    }

    void Update() override;

    void FixedUpdate() override {
        // Update Loop logic here

    }

private:

   GLFWwindow* window;
   
};

void framebuffer_size_callback_UI(GLFWwindow* window, int width, int height);

#endif 