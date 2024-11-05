#include "../include/DKWindows.h"

// Function to create the dockspace and additional windows
void createImGuiWindows(Scene* scene) {
   // Setup a dockspace
   ImGuiWindowFlags dockspace_flags = ImGuiWindowFlags_NoDocking;
   ImGuiViewport* viewport = ImGui::GetMainViewport();

   // Get the work area (the area that excludes the space taken up by the menu bar)
   ImVec2 work_pos = viewport->WorkPos;
   ImVec2 work_size = viewport->WorkSize;

   // Begin dockspace layout
   ImGui::SetNextWindowPos(work_pos);
   ImGui::SetNextWindowSize(work_size);
   ImGui::SetNextWindowViewport(viewport->ID);
   dockspace_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

   ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
   ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
   ImGui::Begin("DockSpace Demo", nullptr, dockspace_flags);
   ImGui::PopStyleVar(2);

   // Create DockSpace ID
   ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
   ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
   ImGui::End();

   // First time configuration for docking layout
   static bool firstTime = true;
   if (firstTime) {
      firstTime = false;

      // Split the dockspace into 4 nodes (top, left, right, bottom)
      ImGui::DockBuilderRemoveNode(dockspace_id); // Clear any previous layout
      ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_None); // Recreate the dockspace
      ImGuiID dock_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.25f, nullptr, &dockspace_id);
      ImGuiID dock_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.25f, nullptr, &dockspace_id);
      ImGuiID dock_bottom = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);
      ImGuiID dock_top = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.25f, nullptr, &dockspace_id);

      // Dock windows
      ImGui::DockBuilderDockWindow("Viewport", dock_top);
      ImGui::DockBuilderDockWindow("Hierarchy", dock_left);
      ImGui::DockBuilderDockWindow("Inspector", dock_right);
      ImGui::DockBuilderDockWindow("Browser", dock_bottom);
      ImGui::DockBuilderDockWindow("Console", dock_bottom);
      ImGui::DockBuilderFinish(dockspace_id); // Apply the layout
   }

   drawViewport();
   drawHierarchy(scene);
   drawInspector();
   drawBrowser();
   drawConsole();

}