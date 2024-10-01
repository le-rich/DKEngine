#include "Inspector.h"
#include "Hierarchy.h"

void drawInspector() {
   //Get object information and display
   ImGui::Begin("Inspector");
   ImGui::Text(("Selected Object: " + selectedNode).c_str());
   ImGui::Text("Parent: TODO");
   ImGui::Text("Position: TODO");
   ImGui::Text("Rotation: TODO");
   ImGui::Text("Scale: TODO");
   ImGui::End();
}