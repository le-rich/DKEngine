#pragma once
#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <imgui.h>
#include "Scene.h"
#include "Components/LightComponent.h"
#include "Components/MeshComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/CameraComponent.h"
#include "Components/ScriptComponent.h"
#include "Core.h"

void drawInspector(Scene* scene);
void drawTransformComponent();
void drawLightComponent(Component* component);
void drawMeshComponent(Component* component);
void drawRigidBodyComponent(Component* component);
void drawCameraComponent(Component* component);
void drawScriptComponent(Component* component);

#endif