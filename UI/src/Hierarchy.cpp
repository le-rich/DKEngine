#include "../include/Hierarchy.h"
#include "../include/Console.h"
#include "Scene.h"
#include "Entity.h"
#include "Managers/EntityManager.h"
#include <set> // Include for std::set
#include <iostream>
using namespace std;

Entity* selectedEntity = nullptr;
Entity* rootEntity = nullptr;

// Global variable to track the renaming state
Entity* renamingEntity = nullptr; // Currently selected GameObject for renaming
char renamingBuffer[256]; // Buffer to hold the new name input (adjust size as needed)

// Helper function to draw individual hierarchy lines for each GameObject
void drawHierarchyLine(Entity* entity) {
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

    if (entity == selectedEntity) {
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
    }

    if (entity->getChildren().size() == 0) {
       nodeFlags |= ImGuiTreeNodeFlags_Leaf;
    }

    // Create a unique ID using the name and the memory address of the GameObject
    ImGui::PushID(entity->GetEntityID().str().c_str()); // Push the GameObject pointer to the ID stack
    bool nodeOpen = ImGui::TreeNodeEx(entity->GetDisplayName().c_str(), nodeFlags);
    ImGui::PopID(); // Pop the ID from the stack

    // Handle left-click selection
    if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !ImGui::IsItemToggledOpen() && !ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        selectedEntity = entity;
        consoleLog("Selected: " + entity->GetDisplayName());

    }

    // Check right-click to open the context menu
    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        selectedEntity = entity;  // Set as selected
        consoleLog("Right-clicked on " + entity->GetDisplayName());
    }

    // Handle right-click context menu for the GameObject
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Create Child GameObject")) {
            std::string childName = "New Entity";
            Entity* childObject = new Entity(childName);
            EntityManager::getInstance().addEntityToMap(*childObject);
            entity->addChild(childObject);
        }

        if (ImGui::MenuItem("Rename")) {
            renamingEntity = entity; // Set the GameObject to be renamed
            strncpy_s(renamingBuffer, entity->GetDisplayName().c_str(), sizeof(renamingBuffer)); // Copy name to buffer
            renamingBuffer[sizeof(renamingBuffer) - 1] = '\0'; // Ensure null-termination
        }

        if (ImGui::MenuItem("Duplicate")) {
           if (selectedEntity != rootEntity) {
              EntityManager::getInstance().duplicateEntity(entity);
           }
        }

        if (ImGui::MenuItem("Delete GameObject")) {
            if (selectedEntity) {
                consoleLog("Attempting to delete selected GameObject: " + selectedEntity->GetDisplayName());

                // Check if the selected object has a parent
                if (selectedEntity->getParent() != nullptr) {
                    // Adding log to ensure parent reference exists
                    consoleLog("Found parent for GameObject: " + selectedEntity->GetDisplayName() + ", Parent: " + selectedEntity->getParent()->GetDisplayName());

                    consoleLog("Deleting child object with ID: " + selectedEntity->GetEntityID().str() + " from parent: " + selectedEntity->getParent()->GetDisplayName());

                    EntityManager::getInstance().removeEntity(*selectedEntity);

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

    if (entity != rootEntity && ImGui::BeginDragDropSource()) {
       ImGui::SetDragDropPayload("Entity", &entity, sizeof(Entity*));
       ImGui::Text(entity->GetDisplayName().c_str());
       ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget()) {
       const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity");
       if (payload != nullptr) {
          Entity* child = *(Entity**)payload->Data;
          Entity* parent = entity;
          Entity* parentTree = parent;
          while (parentTree != rootEntity && parentTree != child) { //check for parent loop
             parentTree = parentTree->getParent();
          }
          if (parentTree == child) {
             consoleLog("Error: reparenting " + child->GetDisplayName() + " to " + parent->GetDisplayName() + " would cause hierarchy loop");
          }
          else if (child != parent) {
             child->setParent(parent);
             consoleLog(child->GetDisplayName() + " parented to " + parent->GetDisplayName());
          }
       }
       ImGui::EndDragDropTarget();
    }


    // Draw children recursively if the node is open
    if (nodeOpen) {
       for (int i = 0; i < entity->getChildren().size(); i++) {
            drawHierarchyLine(entity->getChildren()[i]);  // Use .get() to pass the raw pointer to drawHierarchyLine
        }
        ImGui::TreePop();
    }
}

// Function to draw the entire hierarchy window
void drawHierarchy(Scene* scene) {
    ImGui::Begin("Hierarchy");

    rootEntity = scene->getRoot();

    if (rootEntity) {
       drawHierarchyLine(rootEntity);
    }

    ImGui::End();
}

// Function to retrieve the currently selected GameObject
Entity* getSelectedEntity() {
    return selectedEntity;
}
