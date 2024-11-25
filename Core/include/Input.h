#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <list>
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

    // singleton instance
    static Input& GetInstance() {
        static Input instance;
        return instance;
    }

    // glfw key callback
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        ActionType actionType = (action == GLFW_PRESS) ? PRESS : (action == GLFW_RELEASE ? RELEASE : HOLD);
        GetInstance().ProcessKeyEvent(key, actionType);
    }

    // glfw mouse callback
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        ActionType actionType = (action == GLFW_PRESS) ? PRESS : (action == GLFW_RELEASE ? RELEASE : HOLD);
        GetInstance().ProcessMouseEvent(button, actionType);
    }

    // register the callback to a specific key
    void RegisterKeyCallback(int key, const std::function<void(ActionType)>& callback) {
        keyCallbacks[key].push_back(callback);
    }

    // unregister the callback for a specific key
    void UnregisterKeyCallback(int key, const std::function<void(ActionType)>& callback) {
        auto& callbacks = keyCallbacks[key];

        callbacks.remove_if([&](const std::function<void(ActionType)>& existingCallback) {
            return existingCallback.target<void(ActionType)>() == callback.target<void(ActionType)>();
            });
        // remove key if no callbacks are left
        if (callbacks.empty()) {
            keyCallbacks.erase(key);
        }
    }

    // unregister all callbacks
    void UnregisterAllKeyCallbacks(int key) {
        keyCallbacks.erase(key);
    }

    // process the events, not sure if the queue part is even necessary anymore
    void Update() {

        double currentTime = glfwGetTime() * 1000.0; // time in ms

        // process events in queue
        while (!eventQueue.empty()) {
            InputEvent event = eventQueue.front();
            eventQueue.pop();

            if (event.action == RELEASE) {
                activeKeys.erase(event.key);
                keyTimers.erase(event.key);
                keyStates[event.key] = RELEASE;
            }
            else if (event.action == PRESS) {
                activeKeys.insert(event.key);
                keyTimers[event.key] = currentTime;
                keyStates[event.key] = PRESS;
            }

            // trigger the callbacks
            auto it = keyCallbacks.find(event.key);
            if (it != keyCallbacks.end()) {
                for (const auto& callback : it->second) {
                    callback(event.action);
                }
            }
        }

        // check for keys that are still pressed and trigger HOLD events
        for (const int key : activeKeys) {
            if (glfwGetKey(window, key) == GLFW_PRESS) {
                double elapsedTime = currentTime - keyTimers[key];

                if (keyStates[key] == PRESS && elapsedTime > PRESS_DURATION_THRESHOLD) {
                    keyStates[key] = HOLD;
                }

                if (keyStates[key] == HOLD) {
                    auto it = keyCallbacks.find(key);
                    if (it != keyCallbacks.end()) {
                        for (const auto& callback : it->second) {
                            callback(HOLD);
                        }
                    }
                }
            }
        }

        // clean up RELEASE states
        for (auto it = keyStates.begin(); it != keyStates.end();) {
            if (it->second == RELEASE) {
                it = keyStates.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    // quick query for external use to track what is active
    bool IsKeyPressed(int key) const {
        return activeKeys.count(key);
    }

    bool IsKeyHeld(int key) const {
        return activeKeys.count(key);
    }

    bool IsMouseButtonPressed(int button) const {
        return activeKeys.count(button);
    }

    void SetWindow(GLFWwindow* glfwWindow) {
        window = glfwWindow;
    }

private:
    Input() = default;
    ~Input() = default;

    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;

    void ProcessKeyEvent(int key, ActionType action) {
        eventQueue.push({ key, action });
        if (action == PRESS) {
            keyStates[key] = PRESS;
        }
        else if (action == RELEASE) {
            keyStates[key] = RELEASE;
            activeKeys.erase(key);
        }
    }

    void ProcessMouseEvent(int button, ActionType action) {
        eventQueue.push({ button, action });
    }

    // queue to store input events
    std::queue<InputEvent> eventQueue;

    // set to track active keys
    std::unordered_set<int> activeKeys;

    // disgusting map of keycodes to a list of callbacks
    std::unordered_map<int, std::list<std::function<void(ActionType)>>> keyCallbacks;

    // tracking the current state of each key
    std::unordered_map<int, ActionType> keyStates;

    // timers to differentiate between press and hold
    std::unordered_map<int, double> keyTimers;
    const double PRESS_DURATION_THRESHOLD = 200; // ms

    // have to pass in the window for glfwGetKey
    GLFWwindow* window = nullptr;
};