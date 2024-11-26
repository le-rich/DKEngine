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
         if (ImGui::MenuItem("Mesh Component (Default Contructor Crashes)") && getSelectedEntity()->getComponent(ComponentType::Mesh) == nullptr) {
            Component* mesh = new MeshComponent(getSelectedEntity());
            getSelectedEntity()->addComponent(*mesh);
         }
         if (ImGui::MenuItem("RigidBody Component") && getSelectedEntity()->getComponent(ComponentType::RigidBody) == nullptr) {
            Component* rigidBody = new RigidBodyComponent(getSelectedEntity());
            getSelectedEntity()->addComponent(*rigidBody);
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
