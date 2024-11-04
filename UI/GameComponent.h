#ifndef GAMECOMPONENT_H
#define GAMECOMPONENT_H

#include <string>

enum class DebugComponentType {
    Transform,
    Renderer,
    Collider,
    Custom
};

class GameComponent {
public:
    // Constructor
    GameComponent(DebugComponentType type, const std::string& name);

    // Get the type of the component
    DebugComponentType getType() const;

    // Get the name of the component
    std::string getName() const;

    // Set a new name for the component
    void setName(const std::string& newName);

private:
    DebugComponentType type; // Component type (Transform, Renderer, etc.)
    std::string name;   // Component name
};

#endif // GAMECOMPONENT_H
