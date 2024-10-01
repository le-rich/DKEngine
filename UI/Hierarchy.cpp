#include "Hierarchy.h"
#include "Console.h"

std::string selectedNode = "";

//Draws contents of hierarchy window
void hierarchyLine(std::string name) {//Replace "name" with game objects
   ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
   if (name == selectedNode) {
      nodeFlags |= ImGuiTreeNodeFlags_Selected;
   }

   //replace name with object name
   bool nodeOpen = ImGui::TreeNodeEx(name.c_str(), nodeFlags);

   if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
      selectedNode = name;
      consoleLog(name);
   }

   if (nodeOpen) {
      //replace with getChildren for game objects
      hierarchyLine(name + 'a');
      hierarchyLine(name + 'b');

      ImGui::TreePop();
   }
}

void drawHierarchy() {
   ImGui::Begin("Hierarchy");
   hierarchyLine("a");//Replace with rootnode
   ImGui::End();
}