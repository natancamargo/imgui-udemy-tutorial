#pragma once

#include <string>
#include <string_view>
#include <vector>

class WindowClass {
public:
  using FileContent = std::vector<std::string>;

  WindowClass()
      : filePath1("text1.txt"), filePath2("text2.txt"), fileContent1({}),
        fileContent2({}), diffResult1({}), diffResult2({}) {}

  void draw(std::string_view label);

private:
  void drawSelection();
  void drawDiffView();
  void drawStats();

  FileContent loadFileContent(std::string_view filePath);
  void saveFileContent(std::string_view filePath, FileContent fileContent);
  void createDiff();
  
  std::string filePath1;
  std::string filePath2;

  FileContent fileContent1;
  FileContent fileContent2;

  FileContent diffResult1;
  FileContent diffResult2;
};

void render(WindowClass &window_obj);
