#pragma once

#include <array>
#include <cstdint>
#include <set>
#include <string_view>

class WindowClass {
public:
  constexpr static std::array<std::string_view, 3> functionNames =
      std::array<std::string_view, 3>{"unk", "sin(x)", "cos(x)"};
  enum class Function { NONE = 0, SIN = 1, COS = 2};
  std::set<Function> selectedFunctions;

  WindowClass() : selectedFunctions({}){};

  void Draw(std::string_view label);
  void DrawSelection();
  void DrawPlot();
private:
  Function functionNameMapping(std::string_view functionName);
  double evaluateFunction(Function function, double x);
};

void render(WindowClass &window_obj);
