#pragma once
#include <unordered_map>
#include <glad/glad.h> // Include glad first
#include <GLFW/glfw3.h>
#include <iostream>


class Input {
public:
    // static maps that keep track of the current state of keypresses & mouse clicks (pressed or unpress)
    static std::unordered_map<int, bool> keys;
    static std::unordered_map<int, bool> mouseButtons;


    // Key callback function for GLFW
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            keys[key] = false;
        }
    }

    // Mouse callback function for GLFW
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        if (action == GLFW_PRESS) {
            mouseButtons[button] = true;
        }
        else if (action == GLFW_RELEASE) {
            mouseButtons[button] = false;
        }
    }

    static bool IsKeyPressed(int key) {
        return keys[key];
    }

    static bool IsMouseButtonPressed(int button) {
        return mouseButtons[button];
    }


    // Listens and Processes any incoming inputs
    static void RunInputListener() {
        if (IsKeyPressed(GLFW_KEY_W)) {
            std::cout << "W key pressed" << std::endl;
        }

        if (IsKeyPressed(GLFW_KEY_A)) {
            std::cout << "A key pressed" << std::endl;
        }

        if (IsKeyPressed(GLFW_KEY_D)) {
            std::cout << "D key pressed" << std::endl;
        }

        // Handle mouse button inputs
        if (IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
            std::cout << "Left mouse button clicked" << std::endl;
        }

        if (IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            std::cout << "Right mouse button clicked" << std::endl;
        }
    }


};

std::unordered_map<int, bool> Input::keys;
std::unordered_map<int, bool> Input::mouseButtons;
