#include "Input.h"

// glfw key callback
void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	ActionType actionType = (action == GLFW_PRESS) ? PRESS : (action == GLFW_RELEASE ? RELEASE : HOLD);
	GetInstance().ProcessKeyEvent(key, actionType);
}

// glfw mouse callback
void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	ActionType actionType = (action == GLFW_PRESS) ? PRESS : (action == GLFW_RELEASE ? RELEASE : HOLD);
	GetInstance().ProcessMouseEvent(button, actionType);
}

// register to a specific key
void Input::RegisterKeyCallback(int key, const std::function<void(ActionType)>& callback) {
	keyCallbacks[key].push_back(callback);
}

// unregister a callback
void Input::UnregisterKeyCallback(int key, const std::function<void(ActionType)>& callback) {
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
void Input::UnregisterAllKeyCallbacks(int key) {
    keyCallbacks.erase(key);
}

// quick queries
bool Input::IsKeyPressed(int key) const {
    return activeKeys.count(key);
}

bool Input::IsKeyHeld(int key) const {
    return activeKeys.count(key);
}

bool Input::IsMouseButtonPressed(int button) const {
    return activeKeys.count(button);
}

// process the events, not sure if the queue part is even necessary anymore
void Input::Update() {

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


// for initializing
void Input::SetWindow(GLFWwindow* glfwWindow) {
    window = glfwWindow;
}