#include "../include/Inspector.h"
#include "../include/Hierarchy.h"
#include "../include/Console.h"
#include "Entity.h"
#include "Component.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>
#include "Core.h"

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

       drawTransformComponent();

       for (auto component : selectedEntity->getComponents()) {
          switch (component->componentType) {
          case ComponentType::Light:
             drawLightComponent(component);
             break;
          case ComponentType::Mesh:
             drawMeshComponent(component);
             break;
          //UNCOMMENT WHEN RIGIDBODY COMPONENT IS ADDED
          //case ComponentType::RigidBody:
          //   drawRigidBodyComponent(component);
          //   break;
          case ComponentType::Camera:
             drawCameraComponent(component);
             break;
          case ComponentType::Script:
             drawScriptComponent(component);
             break;
          default:
             break;
          }
       }
       
   }
   else {
       ImGui::Text("No Entity Selected");
   }
   ImGui::End();
}

void drawTransformComponent() {
   if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
      TransformComponent* transform = getSelectedEntity()->transform;
      if (transform != nullptr) {
         int sectionWidth = 75;

         // Position
         ImGui::SetNextItemWidth(sectionWidth);
         glm::vec3 position = transform->getLocalPosition();
         ImGui::Text("Position");
         ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - sectionWidth);
         ImGui::SameLine(sectionWidth);
         if (ImGui::DragFloat3("##Position", glm::value_ptr(position), 0.1f)) {
            transform->setLocalPosition(position);
         }

         // Rotation
         ImGui::SetNextItemWidth(sectionWidth);
         glm::vec3 rotation = glm::degrees(glm::eulerAngles(transform->getLocalOrientation()));
         ImGui::Text("Rotation");
         ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - sectionWidth);
         ImGui::SameLine(sectionWidth);
         if (ImGui::DragFloat3("##Rotation", glm::value_ptr(rotation), 0.1f)) {

            transform->setLocalOrientation(glm::quat(glm::radians(rotation)));
         }

         // Scale
         ImGui::SetNextItemWidth(sectionWidth);
         glm::vec3 scale = transform->getLocalScale();
         ImGui::Text("Scale");
         ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - sectionWidth);
         ImGui::SameLine(sectionWidth);
         if (ImGui::DragFloat3("##Scale", glm::value_ptr(scale), 0.1f)) {
            transform->setLocalScale(scale);
         }
      }
   }
}

void drawLightComponent(Component* component) {
   if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen)) {
      LightComponent* light = static_cast<LightComponent*>(component);
      int sectionWidth = 100;

      ImGui::SetNextItemWidth(sectionWidth);
      const char* types[] = { "Ambient", "Point", "Directional", "Spot", "Area" };
      LightType lightType = light->GetType();
      int index = static_cast<int>(lightType);
      ImGui::Text("Type");
      ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - sectionWidth);
      ImGui::SameLine(sectionWidth);
      if (ImGui::Combo("##Type", &index, types, 5)) {
         light->SetType(static_cast<LightType>(index));
      }

      ImGui::SetNextItemWidth(sectionWidth);
      float intensity = light->GetIntensity();
      ImGui::Text("Intensity");
      ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - sectionWidth);
      ImGui::SameLine(sectionWidth);
      if (ImGui::DragFloat("##Intensity", &intensity, 0.1f, 0.0f)) {
         light->SetIntensity(intensity);
      }

      ImGui::SetNextItemWidth(sectionWidth);
      float constant = light->GetConstant();
      ImGui::Text("Constant");
      ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - sectionWidth);
      ImGui::SameLine(sectionWidth);
      if (ImGui::DragFloat("##Constant", &constant, 0.1f)) {
         light->SetConstant(constant);
      }

      ImGui::SetNextItemWidth(sectionWidth);
      float linear = light->GetLinear();
      ImGui::Text("Linear");
      ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - sectionWidth);
      ImGui::SameLine(sectionWidth);
      if (ImGui::DragFloat("##Linear", &linear, 0.1f)) {
         light->SetLinear(linear);
      }

      ImGui::SetNextItemWidth(sectionWidth);
      float quadratic = light->GetQuadratic();
      ImGui::Text("Quadratic");
      ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - sectionWidth);
      ImGui::SameLine(sectionWidth);
      if (ImGui::DragFloat("##Quadratic", &quadratic, 0.1f)) {
         light->SetQuadratic(quadratic);
      }

      ImGui::SetNextItemWidth(sectionWidth);
      float cutoff = light->GetCutoff();
      ImGui::Text("Cutoff");
      ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - sectionWidth);
      ImGui::SameLine(sectionWidth);
      if (ImGui::DragFloat("##Cutoff", &cutoff, 0.1f)) {
         light->SetCutoff(cutoff);
      }

      ImGui::SetNextItemWidth(sectionWidth);
      float outerCutoff = light->GetOuterCutoff();
      ImGui::Text("Outer Cutoff");
      ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - sectionWidth);
      ImGui::SameLine(sectionWidth);
      if (ImGui::DragFloat("##OuterCutoff", &outerCutoff, 0.1f)) {
         light->SetOuterCutoff(outerCutoff);
      }
   }
}
void drawMeshComponent(Component* component) {
   if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen)) {
      MeshComponent* mesh = static_cast<MeshComponent*>(component);
      int sectionWidth = 100;
   }
}

void drawRigidBodyComponent(Component* component)
{
   if (ImGui::CollapsingHeader("Rigid Body", ImGuiTreeNodeFlags_DefaultOpen)) {
      RigidBodyComponent* rb = static_cast<RigidBodyComponent*>(component);
   }
}

void drawCameraComponent(Component* component)
{
   if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
      CameraComponent* camera = static_cast<CameraComponent*>(component);
      int sectionWidth = 115;

      ImGui::SetNextItemWidth(sectionWidth);
      float fov = camera->getFieldOfView();
      ImGui::Text("Field Of View");
      ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - sectionWidth);
      ImGui::SameLine(sectionWidth);
      if (ImGui::DragFloat("##fov", &fov, 0.1f)) {
         camera->setFieldOfView(fov);
      }

      ImGui::SetNextItemWidth(sectionWidth);
      float fcp = camera->getFarClipPlane();
      ImGui::Text("Far Clip Plane");
      ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - sectionWidth);
      ImGui::SameLine(sectionWidth);
      if (ImGui::DragFloat("##fcp", &fcp, 0.1f)) {
         camera->setFarClipPlane(fcp);
      }

      ImGui::SetNextItemWidth(sectionWidth);
      float ncp = camera->getNearClipPlane();
      ImGui::Text("Near Clip Plane");
      ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - sectionWidth);
      ImGui::SameLine(sectionWidth);
      if (ImGui::DragFloat("##ncp", &ncp, 0.1f)) {
         camera->setNearClipPlane(ncp);
      }

      ImGui::SetNextItemWidth(sectionWidth);
      float ar = camera->getAspectRatio();
      ImGui::Text("Aspect Ratio");
      ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - sectionWidth);
      ImGui::SameLine(sectionWidth);
      if (ImGui::DragFloat("##ar", &ar, 0.1f)) {
         camera->setAspectRatio(ar);
      }
   }
}

void drawScriptComponent(Component* component)
{
   if (ImGui::CollapsingHeader("Script", ImGuiTreeNodeFlags_DefaultOpen)) {
      ScriptComponent* script = static_cast<ScriptComponent*>(component);
      int sectionWidth = 100;
   }
}
