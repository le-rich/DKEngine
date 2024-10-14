#pragma once
#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <imgui.h>
#include <string>
#include <vector>
#include <memory>  // Use smart pointers for safe memory management
#include "../GameObject.h"

void drawHierarchyLine(GameObject* obj);
void drawHierarchy();
GameObject* getSelectedGameObject();
std::string getNextRootObjectName();
extern std::vector<GameObject*> gameObjects;
#endif