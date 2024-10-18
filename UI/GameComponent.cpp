#include "GameComponent.h"

// Constructor definition
GameComponent::GameComponent(ComponentType type, const std::string& name)
    : type(type), name(name) {}

// Get the type of the component
ComponentType GameComponent::getType() const {
    return type;
}

// Get the name of the component
std::string GameComponent::getName() const {
    return name;
}

// Set a new name for the component
void GameComponent::setName(const std::string& newName) {
    name = newName;
}
