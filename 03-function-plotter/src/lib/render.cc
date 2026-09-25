#include <array>
#include <cmath>
#include <cstddef>
#include <format>
#include <iostream>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "render.h"

void WindowClass::Draw(std::string_view label) {
  constexpr static ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;

  constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
  constexpr static auto window_pos = ImVec2(0.0F, 0.0F);
  ImGui::SetNextWindowSize(window_size);
  ImGui::SetNextWindowPos(window_pos);
  ImGui::Begin(label.data(), nullptr, window_flags);

  DrawSelection();
  DrawPlot();

  ImGui::End();
}

void WindowClass::DrawSelection() {
  for (const std::string_view functionName : functionNames) {
    const Function function = functionNameMapping(functionName);
    bool selected = selectedFunctions.contains(function);

    if (ImGui::Checkbox(functionName.data(), &selected)) {
      if (selected) {
        selectedFunctions.insert(function);
      } else {
        selectedFunctions.erase(function);
      }
    }
  }
}

void WindowClass::DrawPlot() {
  constexpr const int minPoints = 10'000;
  constexpr const double xMin = -100.0;
  constexpr const double xMax = 100.0;
  constexpr const double xStep = (std::abs(xMax) + std::abs(xMin)) / minPoints;

  static auto xs = std::array<double, minPoints>();
  static auto ys = std::array<double, minPoints>();

  if (selectedFunctions.size() == 0 ||
      (selectedFunctions.size() == 1 &&
       *selectedFunctions.begin() == Function::NONE)) {
    ImPlot::BeginPlot("###plot", ImVec2(-1.0F, -1.0F), ImPlotFlags_NoTitle);
    ImPlot::EndPlot();
    return;    
  }

  ImPlot::BeginPlot("###plot", ImVec2(-1.0F, -1.0F), ImPlotFlags_NoTitle);
  for (const auto &function : selectedFunctions) {
    double x = xMin;
    for (std::size_t i = 0; i < minPoints; i++) {
      xs[i] = x;
      ys[i] = evaluateFunction(function, x);
      x += xStep;
    }

    const std::string_view plot_label =
        std::format("##function{}", static_cast<int>(function));
    ImPlot::PlotLine(plot_label.data(), xs.data(), ys.data(), minPoints);
  }
  ImPlot::EndPlot();
}

WindowClass::Function
WindowClass::functionNameMapping(std::string_view functionName) {
  for (size_t functionInt = int(Function::NONE);
       functionInt <= int(Function::COS); functionInt++) {
    if (functionInt < functionNames.size() &&
        functionNames.at(functionInt) == functionName) {
      return static_cast<Function>(functionInt);
    }
  }
  return Function::NONE;
}
double WindowClass::evaluateFunction(Function function, double x) {
  switch (function) {
  case Function::SIN:
    return std::sin(x);
  case Function::COS:
    return std::cos(x);
  case Function::NONE:
  default:
    return 0.0;
  }

  return x;
}

void render(WindowClass &window_obj) { window_obj.Draw("Label"); }
