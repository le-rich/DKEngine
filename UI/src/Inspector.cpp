#include "../include/Inspector.h"
#include "../include/Hierarchy.h"

void drawInspector() {
   //Get object information and display
   ImGui::Begin("Inspector");
   GameObject* selected = getSelectedGameObject();  // Use the new function
   if (selected != nullptr) {
       // Access and display the GameObject's properties in the Inspector
       ImGui::Text("Selected Object: %s", selected->getName().c_str());
   }
   ImGui::Text("Parent: TODO");
   ImGui::Text("Position: TODO");
   ImGui::Text("Rotation: TODO");
   ImGui::Text("Scale: TODO");
   ImGui::End();
}