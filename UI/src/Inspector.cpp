#include "../include/Inspector.h"
#include "../include/Hierarchy.h"
#include "../include/Console.h"
#include "Entity.h"
#include "Component.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>

void drawInspector(Scene* scene) {
   //Get object information and display
   ImGui::Begin("Inspector");
   Entity* selectedEntity = getSelectedEntity();  // Use the new function
   if (selectedEntity != nullptr) {
       // Access and display the GameObject's properties in the Inspector

       // Section: Name
       if (ImGui::CollapsingHeader("Object", ImGuiTreeNodeFlags_DefaultOpen)) {
           ImGui::Text("Name");
           ImGui::SameLine(75);
           static char nameBuffer[256];
           strncpy_s(nameBuffer, selectedEntity->GetDisplayName().c_str(), sizeof(nameBuffer));
           if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer))) {
               selectedEntity->SetDisplayName(nameBuffer);
               consoleLog("GameObject renamed to: " + std::string(nameBuffer));
           }

           ImGui::Text("Parent: ");
           ImGui::SameLine(75);
           if (selectedEntity->getParent() != nullptr) {
               ImGui::Selectable(selectedEntity->getParent()->GetDisplayName().c_str());
           }
           else {
               ImGui::Selectable("None");
           }

           if (selectedEntity != scene->getRoot() && ImGui::BeginDragDropTarget()) {
              const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity");
              if (payload != nullptr) {
                 Entity* parent = *(Entity**)payload->Data;
                 Entity* child = selectedEntity;
                 Entity* parentTree = parent;
                 while (parentTree != scene->getRoot() && parentTree != child) { //check for parent loop
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

       }

       drawTransform();
       //TODO: Check for which components an entity has and call the approprite function to draw
       
   }
   else {
       ImGui::Text("No Entity Selected");
   }
   ImGui::End();
}

void drawTransform() {
   if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
      TransformComponent* transform = getSelectedEntity()->transform;
      if (transform != nullptr) {

         // Position
         glm::vec3 position = transform->getLocalPosition();
         ImGui::Text("Position");
         ImGui::SameLine(75); // Adjust the value (100) to set appropriate spacing for label alignment
         if (ImGui::DragFloat3("##Position", glm::value_ptr(position), 0.1f)) {
            transform->setLocalPosition(position);
         }

         // Rotation
         glm::vec3 rotation = glm::degrees(glm::eulerAngles(transform->getLocalOrientation()));
         ImGui::Text("Rotation");
         ImGui::SameLine(75);
         if (ImGui::DragFloat3("##Rotation", glm::value_ptr(rotation), 0.1f)) {

            transform->setLocalOrientation(glm::quat(glm::radians(rotation)));
         }

         // Scale
         glm::vec3 scale = transform->getLocalScale();
         ImGui::Text("Scale");
         ImGui::SameLine(75);
         if (ImGui::DragFloat3("##Scale", glm::value_ptr(scale), 0.1f)) {
            transform->setLocalScale(scale);
         }
      }
   }
}