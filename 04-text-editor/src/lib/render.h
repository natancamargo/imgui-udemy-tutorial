#pragma once

#include <cstddef>
#include <cstring>
#include <imgui.h>
#include <string>
#include <string_view>

class WindowClass {
public:
  static constexpr const std::size_t bufferSize = size_t{1024};
  static constexpr ImGuiWindowFlags popupFlags =
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

  static constexpr ImVec2 popupSize = ImVec2(300.0F, 100.0F);
  static constexpr ImVec2 popupPos =
      ImVec2(1280.0F / 2.0F - popupSize.x / 2.0F,
             720.0F / 2.0F - popupSize.y / 2.0F);
  static constexpr ImVec2 popupButtonSize = ImVec2(120.0F, 0.0F);

  static constexpr ImVec2 inputTextSize = ImVec2(1200.0F, 625.0F);
  static constexpr auto inputTextFlags = ImGuiInputTextFlags_AllowTabInput;

  static constexpr ImVec2 lineNumberSize = ImVec2(30.0F, inputTextSize.y);  
  
  WindowClass() : currentFileName("") {
    std::memset(textBuffer, 0, bufferSize);
  }

  void draw(std::string_view label);
  void drawMenu();
  void drawMenuSavePopup();
  void drawMenuReadPopup();
  void drawContent();
  void drawInfo();

private:
  char textBuffer[bufferSize];
  std::string currentFileName;

  void saveToFile(std::string_view fileName);
  void readFromFile(std::string_view fileName);
  std::string getFileExtension(std::string_view fileName);
};

void render(WindowClass &window_obj);
