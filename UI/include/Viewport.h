#pragma once
#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <imgui.h>
#include "Buffers/FrameBuffer.h"

void drawViewport(const FrameBuffer* framebuffer);

#endif