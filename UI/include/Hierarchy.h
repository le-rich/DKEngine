#pragma once
#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <imgui.h>
#include <string>
#include <vector>
#include <memory>  // Use smart pointers for safe memory management
#include "Entity.h"
#include "Scene.h"

void drawHierarchyLine(Entity* entity);
void drawHierarchy(Scene* scene);
Entity* getSelectedEntity();
//std::string getNextRootObjectName();
#endif