#include <cstddef>
#include <format>
#include <fstream>
#include <iostream>

#include <fmt/format.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <string>

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
  drawSelection();
  drawDiffView();
  drawStats();
  ImGui::End();
}

void WindowClass::drawSelection() {
  ImGui::InputText("Left", &filePath1);
  ImGui::SameLine();
  if (ImGui::Button("Save##Left")) {
    saveFileContent(filePath1, fileContent1);
  }
  ImGui::InputText("Right", &filePath2);
  ImGui::SameLine();
  if (ImGui::Button("Save##Right")) {
    saveFileContent(filePath2, fileContent2);
  }
  if (ImGui::Button("Compare")) {
    fileContent1 = loadFileContent(filePath1);
    fileContent2 = loadFileContent(filePath2);

    createDiff();
  }
}
void WindowClass::drawDiffView() {
  const ImVec2 parentSize = ImVec2(ImGui::GetContentRegionAvail().x, 500.0F);
  const ImVec2 childSize = ImVec2(parentSize.x / 2.0F - 40.0F, 500.0F);
  const ImVec2 swapSize = ImVec2(40.0F, childSize.y);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0, 0.0));
  ImGui::BeginChild("Parent", parentSize, true);
  if (ImGui::BeginChild("Diff 1", childSize, false)) {
    for (std::size_t i = 0; i < fileContent1.size(); i++) {
      if (!diffResult1[i].empty()) {
        ImGui::TextColored(ImVec4(1.0F, 0, 0, 1.0F), "%s",
                           fileContent1[i].data());
      } else {
        ImGui::Text("%s", fileContent1[i].data());
      }
    }
  }
  ImGui::EndChild();
  ImGui::SameLine();

  const std::size_t lineHeight = ImGui::GetTextLineHeight();
  const ImVec2 buttonSize = ImVec2(15.0F, lineHeight);

  if (ImGui::BeginChild("Swap", swapSize, true)) {
    for (std::size_t i = 0; i < diffResult1.size(); i++) {
      std::string leftLabel = std::format("<##{}", i);
      std::string rightLabel = std::format(">##{}", i);

      if (!diffResult1[i].empty() || diffResult2[i].empty()) {
        if (ImGui::Button(leftLabel.data(), buttonSize)) {
          if (fileContent1.size() > i && fileContent2.size() > i) {
            fileContent1[i] = fileContent2[i];
          } else if (fileContent2.size() > i) {
            fileContent1.push_back(fileContent2[i]);
          }
          createDiff();
        }
        ImGui::SameLine();
        if (ImGui::Button(rightLabel.data(), buttonSize)) {
          if (fileContent1.size() > i && fileContent2.size() > i) {
            fileContent2[i] = fileContent1[i];
          } else if (fileContent1.size() > i) {
            fileContent2.push_back(fileContent1[i]);
          }
          createDiff();
        }
      } else {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + lineHeight);
      }
    }
  }
  ImGui::EndChild();

  ImGui::SameLine();
  if (ImGui::BeginChild("Diff 2", childSize, false)) {
    for (std::size_t i = 0; i < fileContent2.size(); i++) {
      if (!diffResult2[i].empty()) {
        ImGui::TextColored(ImVec4(1.0F, 0, 0, 1.0F), "%s",
                           fileContent2[i].data());
      } else {
        ImGui::Text("%s", fileContent2[i].data());
      }
    }
  }
  ImGui::EndChild();
  ImGui::EndChild();
  ImGui::PopStyleVar();
}
void WindowClass::drawStats() {

  std::size_t diffLinesCount = std::size_t{0};
  for (std::string &line : diffResult1) {
    if (!line.empty()) {
      diffLinesCount++;      
    }      
  }

  ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 20.0F);  
  ImGui::Text("Diff lines count: %zu", diffLinesCount);
}

WindowClass::FileContent
WindowClass::loadFileContent(std::string_view filePath) {
  FileContent fileContent = FileContent{};
  std::ifstream in = std::ifstream{filePath.data()};

  if (in) {
    std::string line = std::string{};
    while (std::getline(in, line)) {
      fileContent.push_back(line);
    }
  }

  in.close();

  return fileContent;
}
void WindowClass::saveFileContent(std::string_view filePath,
                                  FileContent fileContent) {
  std::ofstream out = std::ofstream{filePath.data()};

  if (out) {
    for (const std::string &line : fileContent) {
      out << line << std::endl;
    }
  }

  out.close();
}
void WindowClass::createDiff() {
  diffResult1.clear();
  diffResult2.clear();

  const std::size_t maxNumLines =
      std::max(fileContent1.size(), fileContent2.size());

  for (std::size_t i = 0; i < maxNumLines; i++) {
    const std::string line1 =
        i < fileContent1.size() ? fileContent1[i] : "EMPTY";
    const std::string line2 =
        i < fileContent2.size() ? fileContent2[i] : "EMPTY";
    if (line1 != line2) {
      diffResult1.push_back(line1);
      diffResult2.push_back(line2);
    } else {
      diffResult1.push_back("");
      diffResult2.push_back("");
    }
  }
}

void render(WindowClass &window_obj) { window_obj.draw("Label"); }
