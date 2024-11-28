#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <functional>

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

    // glfw key callbacks
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    // callback addition/removal
    void RegisterKeyCallback(int key, const std::function<void(ActionType)>& callback);
    void UnregisterKeyCallback(int key, const std::function<void(ActionType)>& callback);
    void UnregisterAllKeyCallbacks(int key);

    void Update();

    // quick query for external use to track what is active
    bool IsKeyPressed(int key) const;
    bool IsKeyHeld(int key) const;
    bool IsMouseButtonPressed(int button) const;

    void SetWindow(GLFWwindow* glfwWindow);

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