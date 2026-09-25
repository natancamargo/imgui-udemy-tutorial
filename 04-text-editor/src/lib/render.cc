#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <filesystem>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>
#include <sstream>
#include <string_view>

#include "render.h"

void WindowClass::draw(std::string_view label) {
  constexpr static ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;

  constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
  constexpr static auto window_pos = ImVec2(0.0F, 0.0F);
  ImGui::SetNextWindowSize(window_size);
  ImGui::SetNextWindowPos(window_pos);
  ImGui::Begin(label.data(), nullptr, window_flags);
  drawMenu();
  drawMenuSavePopup();
  drawMenuReadPopup();
  drawContent();
  drawInfo();
  ImGui::End();
}

void render(WindowClass &window_obj) { window_obj.draw("Label"); }

void WindowClass::drawMenu() {
  const bool ctrl_pressed = ImGui::GetIO().KeyCtrl;
  const bool s_pressed = ImGui::IsKeyPressed(ImGuiKey_S);
  const bool l_pressed = ImGui::IsKeyPressed(ImGuiKey_L);

  if (ImGui::Button("Save") || (ctrl_pressed && s_pressed)) {
    ImGui::OpenPopup("Save File");
  }
  ImGui::SameLine();
  if (ImGui::Button("Read") || (ctrl_pressed && l_pressed)) {
    ImGui::OpenPopup("Read File");
  }
  ImGui::SameLine();
  if (ImGui::Button("Clear")) {
    std::memset(textBuffer, 0, bufferSize);
  }
}
void WindowClass::drawMenuSavePopup() {
  const bool esc_pressed = ImGui::IsKeyPressed(ImGuiKey_Escape);

  static char saveFileNameBuffer[256] = "text.txt";

  ImGui::SetNextWindowSize(popupSize);
  ImGui::SetNextWindowPos(popupPos);
  if (ImGui::BeginPopupModal("Save File", nullptr, popupFlags)) {

    ImGui::InputText("Filename", saveFileNameBuffer,
                     sizeof(saveFileNameBuffer));

    if (ImGui::Button("Save", popupButtonSize)) {
      saveToFile(saveFileNameBuffer);
      currentFileName = saveFileNameBuffer;
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel", popupButtonSize) || esc_pressed) {
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}
void WindowClass::drawMenuReadPopup() {
  const bool esc_pressed = ImGui::IsKeyPressed(ImGuiKey_Escape);

  static char readFileNameBuffer[256] = "text.txt";

  ImGui::SetNextWindowSize(popupSize);
  ImGui::SetNextWindowPos(
      ImVec2(ImGui::GetIO().DisplaySize.x / 2.0F - popupSize.x / 2.0F,
             ImGui::GetIO().DisplaySize.y / 2.0F - popupSize.y / 2.0F)

  );
  if (ImGui::BeginPopupModal("Read File", nullptr, popupFlags)) {

    ImGui::InputText("Filename", readFileNameBuffer,
                     sizeof(readFileNameBuffer));

    if (ImGui::Button("Read", popupButtonSize)) {
      readFromFile(readFileNameBuffer);
      currentFileName = readFileNameBuffer;
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel", popupButtonSize) || esc_pressed) {
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}

void WindowClass::saveToFile(std::string_view fileName) {
  std::ofstream out = std::ofstream{fileName.data()};

  if (out) {
    out << textBuffer;
    out.close();    
  }
}
void WindowClass::readFromFile(std::string_view fileName) {
  std::ifstream in = std::ifstream{fileName.data()};

  if (in) {
    std::stringstream buffer = std::stringstream{};
    buffer << in.rdbuf();
    std::memcpy(textBuffer, buffer.str().data(), bufferSize);    
    in.close();    
  }  
}
std::string WindowClass::getFileExtension(std::string_view fileName) {
  return std::filesystem::path{fileName}.extension().string();
}  

void WindowClass::drawContent() {
  ImGui::BeginChild("LineNumbers", lineNumberSize);
  const std::size_t lineCount = std::count(textBuffer, textBuffer + bufferSize, '\n');
  for (std::size_t i = 1; i <= lineCount; i++) {
    ImGui::Text("%zu", i);    
  }
  ImGui::EndChild();
  ImGui::SameLine();  
  ImGui::InputTextMultiline("###inputField", textBuffer, bufferSize,
                            inputTextSize, inputTextFlags);
}
void WindowClass::drawInfo() {
  if (currentFileName.size() == 0) {
    ImGui::Text("No File Opened!");
    return;
  }

  const std::string_view fileExtension = getFileExtension(currentFileName);
  ImGui::Text("Opened file %s | File extension %s", currentFileName.data(), fileExtension.data());  
  
}
