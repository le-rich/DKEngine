#include "../include/Hierarchy.h"
#include "../include/Console.h"
#include "Scene.h"
#include "Entity.h"
#include <set> // Include for std::set
#include <iostream>
using namespace std;

Entity* selectedEntity = nullptr;

// Global variable to track the renaming state
Entity* renamingEntity = nullptr; // Currently selected GameObject for renaming
char renamingBuffer[256]; // Buffer to hold the new name input (adjust size as needed)

// Helper function to draw individual hierarchy lines for each GameObject
void drawHierarchyLine(Entity* entity) {
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

    if (entity == selectedEntity) {
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
    }

    // Create a unique ID using the name and the memory address of the GameObject
    ImGui::PushID(entity->GetEntityID().str().c_str()); // Push the GameObject pointer to the ID stack
    bool nodeOpen = ImGui::TreeNodeEx(entity->GetDisplayName().c_str(), nodeFlags);
    ImGui::PopID(); // Pop the ID from the stack

    // Handle left-click selection
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
        selectedEntity = entity;
        consoleLog("Selected: " + entity->GetDisplayName());

        //entity->listChildren();
    }

    // Check right-click to open the context menu
    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        selectedEntity = entity;  // Set as selected
        consoleLog("Right-clicked on " + entity->GetDisplayName());
    }

    // Handle right-click context menu for the GameObject
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Create Child GameObject")) {
            //std::string childName = entity->getNextChildName();
           std::string childName = "New Entity";
            Entity* childObject = new Entity(childName);
            entity->addChild(childObject);
        }
        if (ImGui::MenuItem("Add Component")) {
            if (selectedEntity) {
                //selectedEntity->addComponent(DebugComponentType::Transform, "NewTransform");
                consoleLog("Added Transform component to " + selectedEntity->GetDisplayName());
            }
        }

        if (ImGui::MenuItem("Rename")) {
            renamingEntity = entity; // Set the GameObject to be renamed
            strncpy_s(renamingBuffer, entity->GetDisplayName().c_str(), sizeof(renamingBuffer)); // Copy name to buffer
            renamingBuffer[sizeof(renamingBuffer) - 1] = '\0'; // Ensure null-termination
        }

        if (ImGui::MenuItem("Delete GameObject")) {
            if (selectedEntity) {
                consoleLog("Attempting to delete selected GameObject: " + selectedEntity->GetDisplayName());

                // Check if the selected object has a parent
                if (selectedEntity->getParent() != nullptr) {
                    // Adding log to ensure parent reference exists
                    consoleLog("Found parent for GameObject: " + selectedEntity->GetDisplayName() + ", Parent: " + selectedEntity->getParent()->GetDisplayName());

                    consoleLog("Deleting child object with ID: " + selectedEntity->GetEntityID().str() + " from parent: " + selectedEntity->getParent()->GetDisplayName());
                    selectedEntity->getParent()->removeChild(selectedEntity);

                    // Log to confirm child removal process initiated
                    consoleLog("Child object removed from parent: " + selectedEntity->GetDisplayName());
                }
                else {
                    consoleLog("No parent found for GameObject: " + selectedEntity->GetDisplayName());
                }

                selectedEntity = nullptr; // Clear the selected pointer after deletion
            }
            else {
                consoleLog("No selected GameObject found to delete.");
            }
        }

        ImGui::EndPopup();
    }

    // Renaming logic
    if (renamingEntity == entity) {
        ImGui::InputText("##RenameInput", renamingBuffer, sizeof(renamingBuffer)); // Render input field for renaming

        if (ImGui::IsItemDeactivatedAfterEdit() || ImGui::IsKeyPressed(ImGuiKey_Enter)) {
            consoleLog("Renamed " + entity->GetDisplayName() + " to: " + renamingBuffer);
            entity->SetDisplayName(renamingBuffer); // Set new name
            renamingEntity = nullptr; // Clear renaming state
        }
    }

    // Draw children recursively if the node is open
    if (nodeOpen) {
        for (auto* child : entity->getChildren()) {
            drawHierarchyLine(child);  // Use .get() to pass the raw pointer to drawHierarchyLine
        }
        ImGui::TreePop();
    }
}

// Function to draw the entire hierarchy window
void drawHierarchy(Scene* s) {
    ImGui::Begin("Hierarchy");

    Entity* rootEntity = s->getRoot();

    if (rootEntity) {
       drawHierarchyLine(rootEntity);
    }

    // Create a new GameObject on right-click
    //if (ImGui::BeginPopupContextWindow()) {
    //    if (ImGui::MenuItem("New GameObject")) {
    //        std::string rootName = getNextRootObjectName();
    //        GameObject* newObject = new GameObject(rootName); // Default name
    //        gameObjects.emplace_back(newObject);
    //        selectedGameObject = newObject; // Select newly created object
    //        consoleLog("Created New GameObject: " + newObject->getName() + " with ID: " + std::to_string(reinterpret_cast<uintptr_t>(newObject)));
    //        
    //    }

    //    ImGui::EndPopup();
    //}

    ImGui::End();
}

// Function to retrieve the currently selected GameObject
Entity* getSelectedEntity() {
    return selectedEntity;
}


//std::string getNextRootObjectName() {
//    int count = 0;
//    std::string baseName = "GameObject";
//    std::string newName = baseName;
//
//    // Count how many root objects have the name "GameObject", "GameObject(1)", etc.
//    for (const auto* rootObject : gameObjects) {
//        const std::string& rootName = rootObject->getName();
//        if (rootName == baseName || (rootName.find(baseName + "(") == 0 && rootName.back() == ')')) {
//            count++;
//        }
//    }
//
//    // Append the number if count > 0 (e.g., "GameObject(1)", "GameObject(2)")
//    if (count > 0) {
//        newName = baseName + "(" + std::to_string(count) + ")";
//    }
//
//    return newName;
//}
