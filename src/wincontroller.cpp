#include "wincontroller.hpp"
#include "activewindow.hpp"
#include <stdexcept>
#include <string>
#include <map>

WinController::WinController(ActiveWindow window) : window(window){  
  look_commands["u_win"] = "hyprctl dispatch movefocus u";
  look_commands["u_wks"] = "hyprctl dispatch workspace r-1";
  look_commands["d_win"] = "hyprctl dispatch movefocus d";
  look_commands["d_wks"] = "hyprctl dispatch workspace r+1";
  look_commands["l_win"] = "hyprctl dispatch movefocus l";
  look_commands["l_wks"] = "hyprctl dispatch movefocus l";
  look_commands["r_win"] = "hyprctl dispatch movefocus r";
  look_commands["r_wks"] = "hyprctl dispatch movefocus r";

  move_commands["u_win"] = "hyprctl dispatch movewindow u";
  move_commands["u_wks"] = "hyprctl dispatch movetoworkspace r-1";
  move_commands["d_win"] = "hyprctl dispatch movewindow d";
  move_commands["d_wks"] = "hyprctl dispatch movetoworkspace r+1";
  move_commands["l_win"] = "hyprctl dispatch movewindow l";
  move_commands["l_wks"] = "hyprctl dispatch movewindow l";
  move_commands["r_win"] = "hyprctl dispatch movewindow r";
  move_commands["r_wks"] = "hyprctl dispatch movewindow r";
  boarder = 20;
}

void WinController::move(char direction) {
  std::string motion = get_motion(direction);
  if (motion == "") throw std::runtime_error("bounds");
  std::string dir(1, direction);
  std::string com = move_commands[dir + "_" + motion]; 
  std::system(com.c_str());
  return;
}

void WinController::look(char direction) {
  std::string motion = get_motion(direction);
  if (motion == "") throw std::runtime_error("bounds");
  std::string dir(1, direction);
  std::string com = look_commands[dir + "_" + motion];
  std::system(com.c_str());
  return;
}


std::string WinController::get_motion(char direction){
  if (direction == 'u') {
    if (window.a[1] > boarder) return "win";
    else if (window.workspace > 1) return "wks";
    else return "";
  }
  else if (direction == 'd') {
    if (window.b[1] < 1080 - boarder) return "win";
    else if (window.workspace != 4) return "wks";
  }
  else if (direction == 'l') {
    if (window.a[0] > boarder) return "win";
    else if (window.monitor > 0) return "wks";
  }
  else if (direction == 'r') {
    if (window.b[0] < 1920*2 - boarder) return "win";
    else if (window.monitor < 1) return "wks";
  }
  return "";
}
