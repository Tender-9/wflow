#include <hyprland/src/plugins/PluginAPI.hpp>
#include <nlohmann/json.hpp>
#include "wincontroller.hpp"
#include "activewindow.hpp"
#include <hyprlang.hpp>
#include "globals.hpp"
#include <stdexcept>
#include <string>

using json = nlohmann::json; 

WinController::WinController(ActiveWindow window) : window(window){  
  look_commands["u_win"] = "movefocus u";
  look_commands["u_wks"] = "workspace r-1";
  look_commands["d_win"] = "movefocus d";
  look_commands["d_wks"] = "workspace r+1";
  look_commands["l_win"] = "movefocus l";
  look_commands["l_wks"] = "movefocus l";
  look_commands["r_win"] = "movefocus r";
  look_commands["r_wks"] = "movefocus r";

  move_commands["u_win"] = "movewindow u";
  move_commands["u_wks"] = "movetoworkspace r-1";
  move_commands["d_win"] = "movewindow d";
  move_commands["d_wks"] = "movetoworkspace r+1";
  move_commands["l_win"] = "movewindow l";
  move_commands["l_wks"] = "movewindow l";
  move_commands["r_win"] = "movewindow r";
  move_commands["r_wks"] = "movewindow r";
  
  // Yeah I am cheating here
  // But at least it's not a magic number
  std::string option_str = HyprlandAPI::invokeHyprctlCommand("getoption", "general:gaps_out", "-j");
  json option = json::parse(option_str);
  
  std::string gaps_out = option["custom"];
  int gaps = std::stoi(std::string(1, gaps_out[0]));
  
  option_str = HyprlandAPI::invokeHyprctlCommand("getoption", "general:border_size", "-j");
  option = json::parse(option_str);
  
  int border_size = option["int"];
  border = gaps + border_size;
     
}

void WinController::move(std::string direction) {
  std::string motion = get_motion(direction);
  if (motion == "") throw std::runtime_error("bounds");
  std::string args = move_commands[direction + "_" + motion]; 
  HyprlandAPI::invokeHyprctlCommand("dispatch", args);
  return;
}

void WinController::look(std::string direction) {
  std::string motion = get_motion(direction);
  if (motion == "") throw std::runtime_error("bounds");
  std::string args = look_commands[direction + "_" + motion];
  HyprlandAPI::invokeHyprctlCommand("dispatch", args);
  return;
}


std::string WinController::get_motion(std::string direction){
  if (direction == "u") {
    if (window.a[1] > border) return "win";
    else if (window.workspace > 1) return "wks";
    else return "";
  }
  else if (direction == "d") {
    if (window.b[1] < 1080 - border) return "win";
    else if (window.workspace != 4) return "wks";
  }
  else if (direction == "l") {
    if (window.a[0] > border) return "win";
    else if (window.monitor > 0) return "wks";
  }
  else if (direction == "r") {
    if (window.b[0] < 1920*2 - border) return "win";
    else if (window.monitor < 1) return "wks";
  }
  return "";
}
