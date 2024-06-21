#include "activewindow.hpp"
#include "globals.hpp"
#include <hyprland/src/debug/Log.hpp>
#include <hyprland/src/plugins/PluginAPI.hpp>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

ActiveWindow::ActiveWindow() {
  std::string active = HyprlandAPI::invokeHyprctlCommand("activewindow", "", "-j");
  json output = json::parse(active);
  
  if (output.empty()) {
    active = HyprlandAPI::invokeHyprctlCommand("activeworkspace", "", "-j");
    output = json::parse(active); 
    monitor = output["monitorID"];
    std::string ws_name = output["name"];
    workspace = std::stoi(ws_name.substr(1));
    a[0] = 0;
    a[1] = 0;
    b[0] = 1920*(workspace+1);
    b[1] = 1080;
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
