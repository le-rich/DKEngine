#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <queue>
#include <iostream>

#include "include/Body.h"

class Input
{
public:
    enum ActionType { PRESS, RELEASE, HOLD };

    struct InputEvent {
        int key;
        ActionType action;
    };

    // Queue to store input events
    static std::queue<InputEvent> eventQueue;

    // Key callback functions for GLFW
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        ActionType actionType = (action == GLFW_PRESS) ? PRESS : (action == GLFW_RELEASE ? RELEASE : HOLD);
        eventQueue.push({ key, actionType });
    }

    // Mouse callback for GLFW 
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        ActionType actionType = (action == GLFW_PRESS) ? PRESS : (action == GLFW_RELEASE ? RELEASE : HOLD);
        eventQueue.push({ button, actionType });
    }
    
    static void RunInputListener(AE86::RigidBody* rigidBody)
    {
        while (!eventQueue.empty())
        {
            InputEvent event = eventQueue.front();
            eventQueue.pop();

            // Checks for press, release or hold
            if (event.action == PRESS)
            {
                if (event.key == GLFW_KEY_W)
                    rigidBody->addForce(AE86::Vector3(-1.0, 0.0, 0.0));
                else if (event.key == GLFW_KEY_S)
                    rigidBody->addForce(AE86::Vector3(1.0, 0.0, 0.0));
                else if (event.key == GLFW_KEY_A)
                    std::cout << "A key pressed" << std::endl;
                else if (event.key == GLFW_KEY_D)
                    std::cout << "D key pressed" << std::endl;
                else if (event.key == GLFW_MOUSE_BUTTON_LEFT)
                    std::cout << "LMB clicked" << std::endl;
                else if (event.key == GLFW_MOUSE_BUTTON_RIGHT)
                    std::cout << "RMB clicked" << std::endl;
            }
            else if (event.action == HOLD)
            {
                if (event.key == GLFW_KEY_W)
                    std::cout << "holding W key" << std::endl;
                else if (event.key == GLFW_KEY_A)
                    std::cout << "holding A key" << std::endl;
                else if (event.key == GLFW_KEY_D)
                    std::cout << "holding D key" << std::endl;
                else if (event.key == GLFW_MOUSE_BUTTON_LEFT)
                    std::cout << "Left mouse button is being held down" << std::endl;
                else if (event.key == GLFW_MOUSE_BUTTON_RIGHT)
                    std::cout << "Right mouse button is being held down" << std::endl;
            }
            else if (event.action == RELEASE)
            {
                // Can change this if we want to check on the release of a specific key
                // e.g. if (event.code == GLFW_KEY_W)
                std::cout << "released" << std::endl;
            }
        }
    }
};

std::queue<Input::InputEvent> Input::eventQueue;
