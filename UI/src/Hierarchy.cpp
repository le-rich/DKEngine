#include "../include/Hierarchy.h"
#include "../include/Console.h"
#include "../GameObject.h"
#include <set> // Include for std::set
#include <iostream>
using namespace std;

GameObject* selectedGameObject = nullptr;

std::vector<GameObject*> gameObjects = {};

// Global variable to track the renaming state
GameObject* renamingGameObject = nullptr; // Currently selected GameObject for renaming
char renamingBuffer[256]; // Buffer to hold the new name input (adjust size as needed)

// Helper function to draw individual hierarchy lines for each GameObject
void drawHierarchyLine(GameObject* gameObject) {
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

    if (gameObject == selectedGameObject) {
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
    }

    // Create a unique ID using the name and the memory address of the GameObject
    ImGui::PushID(gameObject); // Push the GameObject pointer to the ID stack
    bool nodeOpen = ImGui::TreeNodeEx(gameObject->getName().c_str(), nodeFlags);
    ImGui::PopID(); // Pop the ID from the stack

    // Handle left-click selection
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
        selectedGameObject = gameObject;
        consoleLog("Selected: " + gameObject->getName());

        gameObject->listChildren();
    }

    // Check right-click to open the context menu
    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        selectedGameObject = gameObject;  // Set as selected
        consoleLog("Right-clicked on " + gameObject->getName());
    }

    // Handle right-click context menu for the GameObject
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Create Child GameObject")) {
            std::string childName = gameObject->getNextChildName();
            GameObject* childObject = new GameObject(childName);
            gameObject->addChild(childObject);
        }
        if (ImGui::MenuItem("Add Component")) {
            if (selectedGameObject) {
                selectedGameObject->addComponent(DebugComponentType::Transform, "NewTransform");
                consoleLog("Added Transform component to " + selectedGameObject->getName());
            }
        }

        if (ImGui::MenuItem("Rename")) {
            renamingGameObject = gameObject; // Set the GameObject to be renamed
            strncpy_s(renamingBuffer, gameObject->getName().c_str(), sizeof(renamingBuffer)); // Copy name to buffer
            renamingBuffer[sizeof(renamingBuffer) - 1] = '\0'; // Ensure null-termination
        }

        if (ImGui::MenuItem("Delete GameObject")) {
            if (selectedGameObject) {
                consoleLog("Attempting to delete selected GameObject: " + selectedGameObject->getName());

                // Check if the selected object has a parent
                if (selectedGameObject->getParent() != nullptr) {
                    // Adding log to ensure parent reference exists
                    consoleLog("Found parent for GameObject: " + selectedGameObject->getName() +
                        ", Parent: " + selectedGameObject->getParent()->getName());

                    consoleLog("Deleting child object with ID: " + std::to_string(selectedGameObject->getID()) +
                        " from parent: " + selectedGameObject->getParent()->getName());
                    selectedGameObject->getParent()->removeChild(selectedGameObject);

                    // Log to confirm child removal process initiated
                    consoleLog("Child object removed from parent: " + selectedGameObject->getName());
                }
                else {
                    consoleLog("No parent found for GameObject: " + selectedGameObject->getName());
                }

                // If no parent, it's a root object, so delete from the global list
                auto it = std::find(gameObjects.begin(), gameObjects.end(), selectedGameObject);
                if (it != gameObjects.end()) {
                    consoleLog("Deleting root object: " + selectedGameObject->getName());
                    delete* it;
                    gameObjects.erase(it);
                }
                else {
                    consoleLog("Root object not found in global list: " + selectedGameObject->getName());
                }

                selectedGameObject = nullptr; // Clear the selected pointer after deletion
            }
            else {
                consoleLog("No selected GameObject found to delete.");
            }
        }

        ImGui::EndPopup();
    }

    // Renaming logic
    if (renamingGameObject == gameObject) {
        ImGui::InputText("##RenameInput", renamingBuffer, sizeof(renamingBuffer)); // Render input field for renaming

        if (ImGui::IsItemDeactivatedAfterEdit() || ImGui::IsKeyPressed(ImGuiKey_Enter)) {
            consoleLog("Renamed " + gameObject->getName() + " to: " + renamingBuffer);
            gameObject->setName(renamingBuffer); // Set new name
            renamingGameObject = nullptr; // Clear renaming state
        }
    }

    // Draw children recursively if the node is open
    if (nodeOpen) {
        for (auto* child : gameObject->getChildren()) {
            drawHierarchyLine(child);  // Use .get() to pass the raw pointer to drawHierarchyLine
        }
        ImGui::TreePop();
    }
}

// Function to draw the entire hierarchy window
void drawHierarchy() {
    ImGui::Begin("Hierarchy");

    // Create a new GameObject on right-click
    if (ImGui::BeginPopupContextWindow()) {
        if (ImGui::MenuItem("New GameObject")) {
            std::string rootName = getNextRootObjectName();
            GameObject* newObject = new GameObject(rootName); // Default name
            gameObjects.emplace_back(newObject);
            selectedGameObject = newObject; // Select newly created object
            consoleLog("Created New GameObject: " + newObject->getName() + " with ID: " + std::to_string(reinterpret_cast<uintptr_t>(newObject)));
            
        }

        ImGui::EndPopup();
    }

    // Loop over all root game objects in the scene
    for (auto* rootObject : gameObjects) {
        drawHierarchyLine(rootObject);
    }

    ImGui::End();
}

// Function to retrieve the currently selected GameObject
GameObject* getSelectedGameObject() {
    return selectedGameObject;
}


std::string getNextRootObjectName() {
    int count = 0;
    std::string baseName = "GameObject";
    std::string newName = baseName;

    // Count how many root objects have the name "GameObject", "GameObject(1)", etc.
    for (const auto* rootObject : gameObjects) {
        const std::string& rootName = rootObject->getName();
        if (rootName == baseName || (rootName.find(baseName + "(") == 0 && rootName.back() == ')')) {
            count++;
        }
    }

    // Append the number if count > 0 (e.g., "GameObject(1)", "GameObject(2)")
    if (count > 0) {
        newName = baseName + "(" + std::to_string(count) + ")";
    }

    return newName;
}
