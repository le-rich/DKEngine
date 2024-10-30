#include "../include/MenuBar.h"

#include "imgui.h"

void createMenuBar() {
   if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("File")) {
         if (ImGui::MenuItem("New")) {
            // Action for "New" menu item
         }
         if (ImGui::MenuItem("Open", "Ctrl+O")) {
            // Action for "Open"
         }
         if (ImGui::MenuItem("Save", "Ctrl+S")) {
            // Action for "Save"
         }
         if (ImGui::MenuItem("Exit")) {
            // Action for "Exit"
         }
         ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Edit")) {
         if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
            // Action for "Undo"
         }
         if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
            // Action for "Redo"
         }
         if (ImGui::MenuItem("Cut", "Ctrl+X")) {
            // Action for "Cut"
         }
         if (ImGui::MenuItem("Copy", "Ctrl+C")) {
            // Action for "Copy"
         }
         if (ImGui::MenuItem("Paste", "Ctrl+V")) {
            // Action for "Paste"
         }
         ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("View")) {
         if (ImGui::MenuItem("Toggle Fullscreen")) {
            // Action for toggling fullscreen
         }
         if (ImGui::MenuItem("Show/Hide Dockspace")) {
            // Action for showing/hiding dockspace
         }
         ImGui::EndMenu();
      }

      ImGui::EndMainMenuBar();
   }
}
