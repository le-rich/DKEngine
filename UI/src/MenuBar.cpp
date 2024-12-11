#include "../include/MenuBar.h"
#include "imgui.h"

void createMenuBar() {
   if (ImGui::BeginMainMenuBar()) {
      bool notSelected = getSelectedEntity() == nullptr;
      if (notSelected) {
         ImGui::BeginDisabled();
      }
      if (ImGui::BeginMenu("Add Component")) {
         if (ImGui::MenuItem("Light Component") && getSelectedEntity()->getComponent(ComponentType::Light) == nullptr) {
            Component* light = new LightComponent(getSelectedEntity());
            getSelectedEntity()->addComponent(*light);
         }
         if (ImGui::MenuItem("Camera Component") && getSelectedEntity()->getComponent(ComponentType::Camera) == nullptr) {
            Component* camera = new CameraComponent(getSelectedEntity());
            getSelectedEntity()->addComponent(*camera);
         }
         if (ImGui::MenuItem("Script Component") && getSelectedEntity()->getComponent(ComponentType::Script) == nullptr) {
            Component* script = new ScriptComponent(getSelectedEntity());
            getSelectedEntity()->addComponent(*script);
         }
         if (notSelected) {

         }
         ImGui::EndMenu();

      }
      if (notSelected) {
         ImGui::EndDisabled();
      }
      ImGui::EndMainMenuBar();
   }
}
