#ifndef GAMECOMPONENT_H
#define GAMECOMPONENT_H

#include <string>

enum class ComponentType {
    Transform,
    Renderer,
    Collider,
    Custom
};

class GameComponent {
public:
    // Constructor
    GameComponent(ComponentType type, const std::string& name);

    // Get the type of the component
    ComponentType getType() const;

    // Get the name of the component
    std::string getName() const;

    // Set a new name for the component
    void setName(const std::string& newName);

private:
    ComponentType type; // Component type (Transform, Renderer, etc.)
    std::string name;   // Component name
};

#endif // GAMECOMPONENT_H
