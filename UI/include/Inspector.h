#pragma once
#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <imgui.h>
#include "Scene.h"
#include "Components/LightComponent.h"

void drawInspector(Scene* scene);
void drawTransformComponent();
void drawLightComponent(Component* component);
#endif