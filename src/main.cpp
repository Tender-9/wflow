#include <hyprland/src/managers/XWaylandManager.hpp>
#include <hyprland/src/plugins/PluginAPI.hpp>
#include <hyprland/src/Compositor.hpp>
#include <hyprutils/math/Box.hpp>
#include <hyprlang.hpp>

inline HANDLE PHANDLE = nullptr;

std::string getMotion(std::string direction){
  
  static auto const* ws_per_mon = (Hyprlang::INT* const*)HyprlandAPI::getConfigValue(PHANDLE, "plugin:wflow:workspaces_per_monitor")->getDataStaticPtr();
  CMonitor* active_monitor = g_pCompositor -> m_pLastMonitor.get();
  PHLWINDOW active_window = g_pCompositor -> m_pLastWindow.lock();
  int workspace_ID = active_monitor->activeWorkspaceID();
  CBox mon_bounds = active_monitor->logicalBox();
  CBox win_bounds;  
  

  if (!active_window) win_bounds = mon_bounds;
  else win_bounds = active_window->getWindowIdealBoundingBoxIgnoreReserved();  
  
  // This next bit is a little messy, yeah
  
  if (direction == "u") {
    if (win_bounds.y > mon_bounds.y) return "win";
    if ((workspace_ID + **ws_per_mon - 1) % **ws_per_mon != 0) return "wks";
  }

  else if (direction == "d") {
    if (win_bounds.y+win_bounds.h < mon_bounds.y+mon_bounds.h) return "win";
    if (workspace_ID % **ws_per_mon != 0) return "wks";
  }

  else if (direction == "l") {
    if (win_bounds.x > mon_bounds.x) return "win";
    if (workspace_ID > **ws_per_mon) return "wks";
  }
  else if (direction == "r") {
    if (win_bounds.x + win_bounds.w < mon_bounds.x + mon_bounds.w) return "win";
    if (workspace_ID < **ws_per_mon) return "wks";
  }
  return "";
}

 
void dispatchLook(std::string direction) {
  std::string motion = getMotion(direction);
  if (motion == "") return;
  
  std::string args;
  if (motion == "win") args = "movefocus " + direction;
  else if (direction == "u") args = "workspace r-1";
  else if (direction == "d") args = "workspace r+1";
  else args = "movefocus " + direction;

  HyprlandAPI::invokeHyprctlCommand("dispatch", args);
  return;
}

void dispatchMove(std::string direction) {
  std::string motion = getMotion(direction);
  if (motion == "") return;

  std::string args;
  if (motion == "win") args = "movewindow " + direction;
  else if (direction == "u") args = "movetoworkspace r-1";
  else if (direction == "d") args = "movetoworkspace r+1";
  else args = "movewindow " + direction;
  HyprlandAPI::invokeHyprctlCommand("dispatch", args);
  return;
}

APICALL EXPORT std::string PLUGIN_API_VERSION() {
  return HYPRLAND_API_VERSION;
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE = handle;
    
    // ALWAYS add this to your plugins. It will prevent random crashes coming from
    // mismatched header versions.
    const std::string HASH = __hyprland_api_get_hash();
    if (HASH != GIT_COMMIT_HASH){
        throw std::runtime_error("[wflow] Version mismatch");
    }
    HyprlandAPI::addConfigValue(PHANDLE, "plugin:wflow:workspaces_per_monitor", Hyprlang::INT(4));
    HyprlandAPI::addDispatcher(PHANDLE, "wflow:look", dispatchLook);
    HyprlandAPI::addDispatcher(PHANDLE, "wflow:move", dispatchMove);

    HyprlandAPI::reloadConfig();
    return {"wflow", "Workflow plugin", "Tender-9", "1.0"};
}

APICALL EXPORT void PLUGIN_EXIT() {
    // ...
}
