#include "activewindow.hpp"
#include <nlohmann/json.hpp>
#include <array>
#include <memory>
#include <string>

using json = nlohmann::json;

std::string hyprctl(std::string command) {
  std::string cmd = "hyprctl -j " + command;
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

ActiveWindow::ActiveWindow() {
  json output = json::parse(hyprctl("activewindow"));
  if (output.empty()) { 
    output = json::parse(hyprctl("activeworkspace")); 
    monitor = output["monitorID"];
    std::string ws_name = output["name"];
    workspace = std::stoi(ws_name.substr(1));
    a[0] = 0;
    a[1] = 0;
    b[0] = 0;
    b[1] = 0;
    return;
  }
  monitor = output["monitor"];
  std::string ws_name = output["workspace"]["name"];
  workspace = std::stoi(ws_name.substr(1));
  int sizex = output["size"][0];
  int sizey = output["size"][1];
  a[0] = output["at"][0];
  a[1] = output["at"][1];
  b[0] = sizex + a[0];
  b[1] = sizey + a[1];
  return;
}
