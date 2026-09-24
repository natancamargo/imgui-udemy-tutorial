#include "render.h"

WindowClass::WindowClass()
    : currentPath(fs::current_path()), selectedPath(fs::path{}) {}

void WindowClass::Draw(std::string_view label) {
  constexpr static ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;

  constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
  constexpr static auto window_pos = ImVec2(0.0F, 0.0F);
  ImGui::SetNextWindowSize(window_size);
  ImGui::SetNextWindowPos(window_pos);
  ImGui::Begin(label.data(), nullptr, window_flags);

  DrawMenu();
  ImGui::Separator();
  DrawContent();
  ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 80.0F);
  ImGui::Separator();
  DrawActions();
  ImGui::Separator();
  DrawFilter();

  ImGui::End();
}

void WindowClass::DrawMenu() {
  if (ImGui::Button("Go Up")) {
    if (currentPath.has_parent_path()) {
      currentPath = currentPath.parent_path();
    }
  }
  ImGui::SameLine();
  ImGui::Text("Current directory: %s", currentPath.string().c_str());
}

void WindowClass::DrawContent() {
  ImGui::BeginChild("ScrollingRegion",
                    ImVec2(0, ImGui::GetWindowHeight() - 160.0F),
                    true); // Border=true, AutoResize=false
  for (const auto &entry : fs::directory_iterator(currentPath)) {
    const auto is_selected = entry.path() == selectedPath;
    const auto is_directory = entry.is_directory();
    const auto is_file = entry.is_regular_file();
    auto entry_name = entry.path().filename().string();

    if (is_directory) {
      entry_name = "[D] " + entry_name;
    } else if (is_file) {
      entry_name = "[F] " + entry_name;
    }

    if (ImGui::Selectable(entry_name.c_str(), is_selected)) {
      if (is_directory) {
        currentPath = currentPath / entry.path().filename();
      }

      selectedPath = entry.path();
    }
  }
  ImGui::EndChild();
}

void WindowClass::DrawActions() {
  if (fs::is_directory(selectedPath)) {
    ImGui::Text("Selected dir: %s", selectedPath.c_str());
  } else if (fs::is_regular_file(selectedPath)) {
    ImGui::Text("Selected file: %s", selectedPath.c_str());
  } else {
    ImGui::Text("Nothing selected!");
  }

  if (fs::is_regular_file(selectedPath) && ImGui::Button("Open")) {
    openFileWhitDefaultEditor(selectedPath);
  }

  ImGui::SameLine();

  if (ImGui::Button("Rename")) {
    renameDialogOpen = true;
    ImGui::OpenPopup("Rename file");
  }

  ImGui::SameLine();

  if (ImGui::Button("Delete")) {
    deleteDialogOpen = true;
    ImGui::OpenPopup("Delete file");
  }

  renameFilePopup();
  deleteFilePopup();
}

void WindowClass::DrawFilter() {
  static char extensionFilter[16] = {'\0'};

  ImGui::Text("Filter by extension");
  ImGui::SameLine();
  ImGui::InputText("###inputFilter", extensionFilter, sizeof(extensionFilter));

  if (std::strlen(extensionFilter) == 0) {
    return;
  }

  auto filteredFileCount = std::size_t{0};
  for (const auto &entry : fs::directory_iterator(currentPath)) {
    if (!entry.is_regular_file()) {
      continue;
    }

    if (entry.path().extension().string() == extensionFilter) {
      filteredFileCount++;
    }
  }

  ImGui::Text("Number of files: %zu", filteredFileCount);
}

void render(WindowClass &window_obj) { window_obj.Draw("Label"); }

void WindowClass::renameFilePopup() {
  if (ImGui::BeginPopupModal("Rename file", &renameDialogOpen)) {
    static char buffer[16] = {'\0'};

    ImGui::Text("New name: ");
    ImGui::SameLine();
    ImGui::InputText("###newName", buffer, sizeof(buffer));

    if (ImGui::Button("Rename")) {
      fs::path newPath = selectedPath.parent_path() / buffer;
      if (renameFile(selectedPath, newPath)) {
        renameDialogOpen = false;
        selectedPath = newPath;
        std::memset(buffer, 0, sizeof(buffer));
      }
    }

    if (ImGui::Button("Cancel")) {
      renameDialogOpen = false;
    }
    ImGui::EndPopup();
  }
}

void WindowClass::deleteFilePopup() {
  if (ImGui::BeginPopupModal("Delete file", &deleteDialogOpen)) {
    ImGui::Text("Are you sure you want to delete %s?",
                selectedPath.filename().c_str());

    if (ImGui::Button("Yes")) {
      if (deleteFile(selectedPath)) {
        selectedPath.clear();
      }
      deleteDialogOpen = false;
    }

    ImGui::SameLine();

    if (ImGui::Button("No")) {
      deleteDialogOpen = false;
    }

    ImGui::EndPopup();
  }
}

void WindowClass::openFileWhitDefaultEditor(fs::path path) {
#ifdef _WIN32
  const auto command = std::format("start \"\" \"{}\"", path.string());
#elif __APPLE__
  const auto command = std::format("open \"{}\"", path.string());
#else
  const auto command = std::format("xdg-open \"{}\"", path.string());
#endif
  std::system(command.c_str());
}

bool WindowClass::renameFile(fs::path oldPath, fs::path newPath) {
  try {
    fs::rename(oldPath, newPath);
    return true;
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
    return false;
  }
}

bool WindowClass::deleteFile(fs::path path) {
  try {
    fs::remove(path);
    return true;
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
    return false;
  }
}
