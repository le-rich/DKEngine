#include "../include/Inspector.h"
#include "../include/Hierarchy.h"
#include "../include/Console.h"

void drawInspector() {
   //Get object information and display
   ImGui::Begin("Inspector");
   GameObject* selectedGameObject = getSelectedGameObject();  // Use the new function
   if (selectedGameObject != nullptr) {
       // Access and display the GameObject's properties in the Inspector

       // Section: Name
       if (ImGui::CollapsingHeader("Object", ImGuiTreeNodeFlags_DefaultOpen)) {
           ImGui::Text("Name");
           static char nameBuffer[256];
           strncpy_s(nameBuffer, selectedGameObject->getName().c_str(), sizeof(nameBuffer));
           if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer))) {
               selectedGameObject->setName(nameBuffer);
               consoleLog("GameObject renamed to: " + std::string(nameBuffer));
           }
       }
       //ImGui::Text("Selected Object: %s", selected->getName().c_str());

       if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
           // Position
           std::array<float, 3>& position = selectedGameObject->getPosition();
           if (ImGui::DragFloat3("Position", position.data(), 0.1f)) {
               selectedGameObject->setPosition(position);
           }

           // Rotation
           std::array<float, 3>& rotation = selectedGameObject->getRotation();
           if (ImGui::DragFloat3("Rotation", rotation.data(), 0.1f)) {
               selectedGameObject->setRotation(rotation);
           }

           // Scale
           std::array<float, 3>& scale = selectedGameObject->getScale();
           if (ImGui::DragFloat3("Scale", scale.data(), 0.1f)) {
               selectedGameObject->setScale(scale);
           }
       }
   }
   else {
       ImGui::Text("No Game Object Selected");
   }
   ImGui::End();
}

