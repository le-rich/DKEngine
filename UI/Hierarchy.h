#pragma once
#ifndef HIERARCHY_H
#define HIERARCHY_H

#include "imgui.h"
#include <string>

extern std::string selectedNode;

void hierarchyLine(std::string name);
void drawHierarchy();

#endif