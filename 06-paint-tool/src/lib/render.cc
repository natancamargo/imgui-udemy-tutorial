#include <iostream>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "render.h"

void WindowClass::draw(std::string_view label) {
  constexpr static ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar |
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground;

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
  const bool l_pressed = ImGui::IsKeyPressed(ImGuiKey_L);

  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("Save")) {
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Read")) {
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Clear")) {
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  if (ImGui::Button("Save") || (ctrl_pressed && s_pressed)) {
    ImGui::OpenPopup("Save File");
  }
  ImGui::SameLine();
  if (ImGui::Button("Read") || (ctrl_pressed && l_pressed)) {
    ImGui::OpenPopup("Read File");
  }
  ImGui::SameLine();
  if (ImGui::Button("Clear")) {
    clearCanvas();
  }

  drawMenuSavePopup();
  drawMenuReadPopup();
}
void WindowClass::drawCanvas() {}
void WindowClass::drawColorButtons() {}
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
