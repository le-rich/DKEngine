#include "../include/console.h"


std::vector<std::string> logs;

void consoleLog(std::string str) {
   logs.push_back(str);
}

void drawConsole() {
   ImGui::Begin("Console");
   for (int i = 0; i < logs.size(); i++) {
      ImGui::Text(logs[i].c_str());
   }
   ImGui::End();
}