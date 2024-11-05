#include "../include/Inspector.h"
#include "../include/Hierarchy.h"
#include "../include/Console.h"
#include "Entity.h"
#include "Component.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>

void drawInspector() {
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
               ImGui::Text("%s", selectedEntity->getParent()->GetDisplayName().c_str());
           }
           else {
               ImGui::Text("Root");
           }
       }
       //ImGui::Text("Selected Object: %s", selected->getName().c_str());

       if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
          Transform* transform = selectedEntity->transform;
          if (transform != nullptr) {

             // Position
             glm::vec3 position = transform->getLocalPosition();
             ImGui::Text("Position");
             ImGui::SameLine(75); // Adjust the value (100) to set appropriate spacing for label alignment
             if(ImGui::DragFloat3("##Position", glm::value_ptr(position), 0.1f)) {
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
                transform->setLocalScale(position);
             }
          }
       }
   }
   else {
       ImGui::Text("No Game Object Selected");
   }
   ImGui::End();
}

