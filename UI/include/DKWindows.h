#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <string>
#include <vector>
#include "Browser.h"
#include "Console.h"
#include "Hierarchy.h"
#include "Inspector.h"
#include "Viewport.h"
#include "Scene.h"
#include "Buffers/FrameBuffer.h"

void createImGuiWindows(Scene* scene, const FrameBuffer* framebuffer);

