#include "GameObject.h"
#include <iostream>
#include "include/Console.h"
#include "include/Hierarchy.h"
#include <vector>

int GameObject::nextID = 1; // Initialize the static variable

// Constructor
GameObject::GameObject(const std::string& name)
    : name(name), parent(nullptr), id(nextID++), position({ 0.0f, 0.0f, 0.0f }), rotation({ 0.0f, 0.0f, 0.0f }), scale({ 1.0f, 1.0f, 1.0f }) {}

GameObject::~GameObject() {
    for (auto* child : children) {
        delete child;  // Ensure all children are deleted
    }
    children.clear();  // Clear the list of children to avoid dangling references
    std::cout << "GameObject destroyed: " << name << std::endl;
}

// Get the unique ID of the game object
int GameObject::getID() const {
    return id;
}
// Add a child GameObject
void GameObject::addChild(GameObject* child) {
    if (this == child) {
        consoleLog("Error: Cannot add a GameObject as its own child!");
        return;
    }

    if (std::find(children.begin(), children.end(), child) == children.end()) {
        children.push_back(child);
        child->setParent(this); // Set this object as the parent
        consoleLog("Added child: " + child->getName() + " to parent: " + getName());
    }
    else {
        consoleLog("Warning: Child already exists in the parent!");
    }
}
// Remove a child by pointer
void GameObject::removeChild(GameObject* child) {
    auto it = std::remove(children.begin(), children.end(), child);
    if (it != children.end()) {
        consoleLog("Removing child: " + child->getName() + " from parent: " + getName());
        children.erase(it, children.end());
    }
    else {
        consoleLog("Child not found in parent's children list: " + child->getName());
    }
}

// Get the name of the game object
std::string GameObject::getName() const {
    return name;
}

void GameObject::setName(const std::string& name) {
    this->name = name; // Update the GameObject's name
}

// Get and set methods for Position, Rotation, and Scale
std::array<float, 3>& GameObject::getPosition() {
    return position;
}

void GameObject::setPosition(const std::array<float, 3>& newPosition) {
    position = newPosition;
}

std::array<float, 3>& GameObject::getRotation() {
    return rotation;
}

void GameObject::setRotation(const std::array<float, 3>& newRotation) {
    rotation = newRotation;
}

std::array<float, 3>& GameObject::getScale() {
    return scale;
}

void GameObject::setScale(const std::array<float, 3>& newScale) {
    scale = newScale;
}

// Add a component to the game object
void GameObject::addComponent(ComponentType type, const std::string& componentName) {
    components.emplace_back(type, componentName);
}

// List all components attached to this game object
void GameObject::listComponents() const {
    std::cout << "Components of " << name << ":" << std::endl;
    for (const auto& component : components) {
        std::cout << "  - " << component.getName() << std::endl;
    }
}

// Get all components attached to this game object
std::vector<GameComponent>& GameObject::getComponents() {
    return components;
}

std::string GameObject::getNextChildName() {
    int count = 0;
    std::string baseName = "GameObject";
    std::string newName = baseName;

    // Count how many children already have the name "GameObject", "GameObject(1)", etc.
    for (const auto* child : getChildren()) {
        const std::string& childName = child->getName();
        if (childName == baseName || (childName.find(baseName + "(") == 0 && childName.back() == ')')) {
            count++;
        }
    }

    // Append the number if count > 0 (e.g., "GameObject(1)", "GameObject(2)")
    if (count > 0) {
        newName = baseName + "(" + std::to_string(count) + ")";
    }

    return newName;
}

// Get a reference to the list of child GameObjects
std::vector<GameObject*>& GameObject::getChildren() {
    return children; // Return the vector of child GameObjects
}

void GameObject::deleteObject() {
    // Logging deletion for debugging
    consoleLog("Attempting to delete GameObject: " + name);

    // Remove this GameObject from its parent's children list, if it has a parent
    if (this->parent != nullptr) {
        consoleLog("Removing from parent: " + this->parent->getName());
        this->parent->removeChild(this);
        this->parent = nullptr; // Nullify the parent reference
    }

    // If this GameObject is a root object, remove it from the global gameObjects list
    auto it = std::find(gameObjects.begin(), gameObjects.end(), this);
    if (it != gameObjects.end()) {
        consoleLog("Removing GameObject from global list: " + name);
        gameObjects.erase(it);
    }

    // Create a temporary list to hold child references before clearing
    std::vector<GameObject*> tempChildren = children;

    // Clear the children list to avoid invalid access during recursion
    children.clear();

    // Recursively delete all children
    for (auto* child : tempChildren) {
        consoleLog("Recursively deleting child: " + child->getName());
        child->deleteObject(); // Recursively delete each child
    }

    // Finally, delete this GameObject
    consoleLog("Deleting GameObject: " + name);
    delete this;
}

// List children of this game object
void GameObject::listChildren() const {
    std::cout << "Children of " << name << ":" << std::endl;
    for (const auto* child : children) {
        std::cout << "  - " << child->getName() << std::endl;
    }
}

void GameObject::setParent(GameObject* newParent) {
    if (this->parent != nullptr) {
        consoleLog("Removing from old parent: " + this->parent->getName());
        this->parent->removeChild(this);
    }

    this->parent = newParent;

    if (newParent != nullptr) {
        consoleLog("Setting new parent: " + newParent->getName() + " for " + this->getName());
        newParent->addChild(this);
    }
}

// Get the parent of this game object
GameObject* GameObject::getParent() const {
    return parent;
}

