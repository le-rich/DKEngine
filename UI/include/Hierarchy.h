#pragma once
#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <imgui.h>
#include <string>

//std::string selectedNode;

void hierarchyLine(std::string name);
void drawHierarchy();
std::string getSelectedNode();

#endif