#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <vector>
#include <array>
#include "GameComponent.h"

class GameObject{
private:
    std::string name;                         // Name of the game object
    std::vector<GameComponent> components;    // Components attached to the game object
    std::vector<GameObject*> children = {};        // Children game objects
    GameObject* parent = nullptr;                       // Parent game object (nullptr if no parent)
    int id;

    static int nextID;

    // Transform properties using std::array
    std::array<float, 3> position; // Position (x, y, z)
    std::array<float, 3> rotation; // Rotation (x, y, z)
    std::array<float, 3> scale;    // Scale (x, y, z)
public:
    // Constructor
    GameObject(const std::string& name);

    // Destructor
    ~GameObject();

    // Get the name of the game object
    std::string getName() const;
    // Setter for the name
    void setName(const std::string& name); // Declaration of setName method

    std::array<float, 3>& getPosition();
    void setPosition(const std::array<float, 3>& newPosition);
    std::array<float, 3>& getRotation();
    void setRotation(const std::array<float, 3>& newRotation);
    std::array<float, 3>& getScale();
    void setScale(const std::array<float, 3>& newScale);
    
    // Get the unique ID of the game object
    int getID() const;
    
    // Add a component to the game object
    void addComponent(DebugComponentType type, const std::string& componentName);

    // List all components attached to this game object
    void listComponents() const;

    // Get all components attached to this game object
    std::vector<GameComponent>& getComponents();

    // Set a parent for this game object
    void setParent(GameObject* parent);
    // Add a child to this game object
    void addChild(GameObject* child);
    // Remove a child by pointer
    void removeChild(GameObject* child);

    std::string getNextChildName();

    void deleteObject();

    // List children of this game object
    void listChildren() const;

    //// Get the list of children
    std::vector<GameObject*>& getChildren();

    //// Get the parent of this game object
    GameObject* getParent() const;

};
#endif // GAMEOBJECT_H
