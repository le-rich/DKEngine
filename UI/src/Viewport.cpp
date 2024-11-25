#include "../include/Viewport.h"
#include "Renderer.h"

void drawViewport(const FrameBuffer* framebuffer) {
   ImGui::Begin("Viewport");

   ImVec2 availableSize = ImGui::GetContentRegionAvail();
   float aspectRatio = (float)framebuffer->getBufferWidth() / (float)framebuffer->getBufferHeight();

   float width = availableSize.x;
   float height = width / aspectRatio;

   if (height > availableSize.y) {
      height = availableSize.y;
      width = height * aspectRatio;
   }

   float centerWidth = (availableSize.x / 2) - (width / 2);
   float centerHeight = (availableSize.y / 2) - (height / 2);

   ImGui::SetCursorPosX(ImGui::GetCursorPosX() + centerWidth);
   ImGui::SetCursorPosY(ImGui::GetCursorPosY() + centerHeight);

   ImGui::Image((void*)(intptr_t)framebuffer->GetColorBufferID(), ImVec2(width, height), ImVec2(0, 1), ImVec2(1, 0));

   ImGui::End();
}