#pragma once

#include <cstdint>
#include <imgui.h>
#include <string_view>
#include <vector>

class WindowClass {
public:
  using PointData = std::tuple<ImVec2, ImColor, float>;

    static constexpr ImGuiWindowFlags popupFlags =
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
  static constexpr ImVec2 popupSize = ImVec2(300.0F, 100.0F);
  static constexpr ImVec2 popupPos =
      ImVec2(1280.0F / 2.0F - popupSize.x / 2.0F,
             720.0F / 2.0F - popupSize.y / 2.0F);
  static constexpr ImVec2 popupButtonSize = ImVec2(120.0F, 0.0F);
  
  WindowClass()
      : points({}), canvasPos({}), drawColor(ImColor(255, 255, 255)),
        pointDrawSize(2.0F), filenameBuffer("test.bin") {}

  void draw(std::string_view label);

private:
  std::uint32_t numRows = 800;
  std::uint32_t numCols = 600;
  std::uint32_t numChannels = 3;

  ImVec2 canvasSize =
      ImVec2(static_cast<float>(numRows), static_cast<float>(numCols));
  std::vector<PointData> points;
  ImVec2 canvasPos;

  ImColor drawColor;
  float pointDrawSize;

  char filenameBuffer[256];

  void drawMenu();  
  void drawMenuSavePopup();  
  void drawMenuReadPopup();
  void drawCanvas();
  void drawControls();  
  void drawSizeSettings();  
  void drawColorButtons();  
  void saveToImageFile(std::string_view filename);
  void loadFromImageFile(std::string_view filename);
  void clearCanvas();  
};

void render(WindowClass &window_obj);
