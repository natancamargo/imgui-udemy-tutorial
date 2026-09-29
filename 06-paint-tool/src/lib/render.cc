#include <iostream>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>
#include <memory>

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
        ImGui::OpenPopup("Save File");
      }
      if (ImGui::MenuItem("Read", "Ctrl+o") || (ctrl_pressed && o_pressed)) {
        ImGui::OpenPopup("Read File");
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
void WindowClass::drawCanvas() {}
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
    if (ImGui::BeginPopup("Color Picker")) {
      ImGui::ColorPicker3("##Color", reinterpret_cast<float *>(&drawColor));
      ImGui::EndPopup();
    }
  }
  if (none_preset_color) {
    ImGui::PopStyleColor();
  }
}
void WindowClass::drawSizeSettings() {
  ImGui::Text("Draw Size");
  ImGui::SameLine(120);
  ImGui::PushItemWidth(canvasSize.x - ImGui::GetCursorPosX());
  ImGui::SliderFloat("##drawSize", &pointDrawSize, 1.0F, 10.0F);
  ImGui::PopItemWidth();
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
      saveToImageFile(filenameBuffer);
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
      loadFromImageFile(filenameBuffer);
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel", popupButtonSize) || esc_pressed) {
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}

void WindowClass::saveToImageFile(std::string_view filename) {}
void WindowClass::loadFromImageFile(std::string_view filename) {}
void WindowClass::clearCanvas() {}

void render(WindowClass &window_obj) { window_obj.draw("Label"); }
