#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>
#include <memory>
#include <tuple>

#include "render.h"

void WindowClass::draw(std::string_view label) {
  constexpr static ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar |
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |
      ImGuiWindowFlags_MenuBar;

  constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
  constexpr static auto window_pos = ImVec2(0.0F, 0.0F);
  ImGui::SetNextWindowSize(window_size);
  ImGui::SetNextWindowPos(window_pos);
  ImGui::Begin(label.data(), nullptr, window_flags);
  drawMenu();
  drawCanvas();
  ImGui::End();
}

void WindowClass::drawMenu() {
  const bool ctrl_pressed = ImGui::GetIO().KeyCtrl;
  const bool s_pressed = ImGui::IsKeyPressed(ImGuiKey_S);
  const bool o_pressed = ImGui::IsKeyPressed(ImGuiKey_O);

  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Save", "Ctrl+s") || (ctrl_pressed && s_pressed)) {
        saveOpenTrigger = true;
      }
      if (ImGui::MenuItem("Read", "Ctrl+o") || (ctrl_pressed && o_pressed)) {
        readOpenTrigger = true;
      }
      if (ImGui::MenuItem("Clear")) {
        clearCanvas();
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Quit")) {
      }
      ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
  }

  drawControls();

  drawMenuSavePopup();
  drawMenuReadPopup();
}
void WindowClass::drawCanvas() {
  canvasPos = ImGui::GetCursorPos();
  const float borderThickness = 1.5F;
  const ImVec2 buttonSize = ImVec2(canvasSize.x + 2.0F * borderThickness,
                                   canvasSize.y + 2.0F * borderThickness);
  ImGui::InvisibleButton("##canvas", buttonSize);

  const ImVec2 mousePos = ImGui::GetMousePos();
  const bool isHovering = ImGui::IsItemHovered();
  if (isHovering && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
    const ImVec2 point = ImVec2(mousePos.x - canvasPos.x - borderThickness,
                                mousePos.y - canvasPos.y - borderThickness);
    points.push_back(std::make_tuple(point, drawColor, drawSize));
    std::cout << "here";
  }

  ImDrawList *drawList = ImGui::GetWindowDrawList();

  for (const auto &[point, color, size] : points) {
    const ImVec2 pos = ImVec2(canvasPos.x + borderThickness + point.x,
                              canvasPos.y + borderThickness + point.y);
    drawList->AddCircleFilled(pos, size, color);
  }

  const ImVec2 border_min = canvasPos;
  const auto border_max = ImVec2(canvasPos.x + buttonSize.x - borderThickness,
                                 canvasPos.y + buttonSize.y - borderThickness);
  drawList->AddRect(border_min, border_max, IM_COL32(255, 255, 255, 255));
}
void WindowClass::drawControls() {
  ImGui::SeparatorText("Controls");
  drawColorButtons();
  drawSizeSettings();
  ImGui::Separator();
}
void WindowClass::drawColorButtons() {
  const bool selected_red = drawColor == ImColor(255, 0, 0, 255);
  const bool selected_green = drawColor == ImColor(0, 255, 0, 255);
  const bool selected_blue = drawColor == ImColor(0, 0, 255, 255);
  const bool selected_white = drawColor == ImColor(255, 255, 255, 255);
  const bool none_preset_color =
      !selected_red && !selected_blue && !selected_green && !selected_white;

  constexpr const ImVec4 orange = ImVec4(1.0F, 0.5F, 0.0F, 1.0F);

  ImGui::Text("Color Buttons");
  ImGui::SameLine(120);
  if (selected_red) {
    ImGui::PushStyleColor(ImGuiCol_Button, orange);
  }
  if (ImGui::Button("Red")) {
    drawColor = ImColor(255, 0, 0, 255);
  }
  if (selected_red) {
    ImGui::PopStyleColor();
  }

  ImGui::SameLine();

  if (selected_green) {
    ImGui::PushStyleColor(ImGuiCol_Button, orange);
  }
  if (ImGui::Button("Green")) {
    drawColor = ImColor(0, 255, 0, 255);
  }
  if (selected_green) {
    ImGui::PopStyleColor();
  }

  ImGui::SameLine();

  if (selected_blue) {
    ImGui::PushStyleColor(ImGuiCol_Button, orange);
  }
  if (ImGui::Button("Blue")) {
    drawColor = ImColor(0, 0, 255, 255);
  }
  if (selected_blue) {
    ImGui::PopStyleColor();
  }

  ImGui::SameLine();

  if (selected_white) {
    ImGui::PushStyleColor(ImGuiCol_Button, orange);
  }
  if (ImGui::Button("White")) {
    drawColor = ImColor(255, 255, 255, 255);
  }
  if (selected_white) {
    ImGui::PopStyleColor();
  }

  ImGui::SameLine();

  if (none_preset_color) {
    ImGui::PushStyleColor(ImGuiCol_Button, orange);
  }
  if (ImGui::Button("Choose")) {
    ImGui::OpenPopup("Color Picker");
  }
  if (ImGui::BeginPopup("Color Picker")) {
    ImGui::ColorPicker3("##Color", reinterpret_cast<float *>(&drawColor));
    ImGui::EndPopup();
  }
  if (none_preset_color) {
    ImGui::PopStyleColor();
  }
}
void WindowClass::drawSizeSettings() {
  ImGui::Text("Draw Size");
  ImGui::SameLine(120);
  ImGui::PushItemWidth(canvasSize.x - ImGui::GetCursorPosX());
  ImGui::SliderFloat("##drawSize", &drawSize, 1.0F, 10.0F);
  ImGui::PopItemWidth();
}
void WindowClass::drawMenuSavePopup() {
  const bool esc_pressed = ImGui::IsKeyPressed(ImGuiKey_Escape);

  static char saveFileNameBuffer[256];
  std::memcpy(saveFileNameBuffer, filenameBuffer, sizeof(saveFileNameBuffer));

  ImGui::SetNextWindowSize(popupSize);
  ImGui::SetNextWindowPos(popupPos);
  if (saveOpenTrigger) {
    ImGui::OpenPopup("Save File");
    saveOpen = true;
    saveOpenTrigger = false;
  }
  if (ImGui::BeginPopupModal("Save File", &saveOpen, popupFlags)) {

    ImGui::InputText("Filename", saveFileNameBuffer,
                     sizeof(saveFileNameBuffer));

    if (ImGui::Button("Save", popupButtonSize)) {
      saveToImageFile(filenameBuffer);
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel", popupButtonSize) || esc_pressed) {
      ImGui::CloseCurrentPopup();
      saveOpen = false;
    }

    ImGui::EndPopup();
  }
}
void WindowClass::drawMenuReadPopup() {
  const bool esc_pressed = ImGui::IsKeyPressed(ImGuiKey_Escape);

  static char readFileNameBuffer[256];
  std::memcpy(readFileNameBuffer, filenameBuffer, sizeof(readFileNameBuffer));

  ImGui::SetNextWindowSize(popupSize);
  ImGui::SetNextWindowPos(
      ImVec2(ImGui::GetIO().DisplaySize.x / 2.0F - popupSize.x / 2.0F,
             ImGui::GetIO().DisplaySize.y / 2.0F - popupSize.y / 2.0F)

  );
  if (readOpenTrigger) {
    ImGui::OpenPopup("Read File");
    readOpen = true;
    readOpenTrigger = false;
  }
  if (ImGui::BeginPopupModal("Read File", &readOpen, popupFlags)) {

    ImGui::InputText("Filename", readFileNameBuffer,
                     sizeof(readFileNameBuffer));

    if (ImGui::Button("Read", popupButtonSize)) {
      loadFromImageFile(filenameBuffer);
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel", popupButtonSize) || esc_pressed) {
      ImGui::CloseCurrentPopup();
      readOpen = false;
    }

    ImGui::EndPopup();
  }
}

void WindowClass::saveToImageFile(std::string_view filename) {
  std::ofstream out = std::ofstream{filename.data()};

  if (!out || !out.is_open()) {
    return;
  }

  const std::size_t pointCount = points.size();
  out.write(reinterpret_cast<const char *>(&pointCount), sizeof(pointCount));

  for (const auto &[point, color, size] : points) {
    out.write(reinterpret_cast<const char *>(&point), sizeof(point));
    out.write(reinterpret_cast<const char *>(&color), sizeof(color));
    out.write(reinterpret_cast<const char *>(&size), sizeof(size));
  }

  out.close();
}
void WindowClass::loadFromImageFile(std::string_view filename) {
  std::ifstream in = std::ifstream{filename.data(), std::ios::binary};

  if (!in || !in.is_open()) {
    return;
  }

  std::size_t pointCount = points.size();
  in.read(reinterpret_cast<char *>(&pointCount), sizeof(pointCount));

  for (std::size_t i = 0; i < pointCount; i++) {
    ImVec2 point;
    ImColor color;
    float size;
    in.read(reinterpret_cast<char *>(&point), sizeof(point));
    in.read(reinterpret_cast<char *>(&color), sizeof(color));
    in.read(reinterpret_cast<char *>(&size), sizeof(size));

    points.push_back(std::make_tuple(point, color, size));
  }

  in.close();
}
void WindowClass::clearCanvas() { points.clear(); }

void render(WindowClass &window_obj) { window_obj.draw("Label"); }
