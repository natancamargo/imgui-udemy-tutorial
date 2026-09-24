#pragma once

#include <cstdint>
#include <iostream>
#include <string_view>
#include <filesystem>
#include <cstring>
#include <format>
#include <cstdlib>

#include <imgui.h>
#include <implot.h>

namespace fs = std::filesystem;

class WindowClass
{
public:
  WindowClass();
  
  void Draw(std::string_view label);
private:
  void DrawMenu();
  void DrawContent();
  void DrawActions();
  void DrawFilter();
  void openFileWhitDefaultEditor(fs::path path);
  void renameFilePopup();
  void deleteFilePopup();
  bool renameFile(fs::path oldPath, fs::path newPath);  
  bool deleteFile(fs::path path);  
private:
  fs::path currentPath;
  fs::path selectedPath;

  bool renameDialogOpen = false;
  bool deleteDialogOpen = false;
};

void render(WindowClass &window_obj);

