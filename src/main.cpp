#include "activewindow.hpp"
#include "globals.hpp"
#include "wincontroller.hpp"
#include <hyprland/src/plugins/PluginAPI.hpp>
#include <hyprlang.hpp>
#include <string>

APICALL EXPORT std::string PLUGIN_API_VERSION() {
  return HYPRLAND_API_VERSION;
}

void look(std::string args){
  ActiveWindow window = ActiveWindow();
  WinController controller = WinController(window);
  controller.look(args);
  return;
}
void move(std::string args){
  ActiveWindow window = ActiveWindow();
  WinController controller = WinController(window);
  controller.move(args);
  return;
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE = handle;
    
    // ALWAYS add this to your plugins. It will prevent random crashes coming from
    // mismatched header versions.
    const std::string HASH = __hyprland_api_get_hash();
    if (HASH != GIT_COMMIT_HASH){
        throw std::runtime_error("[wflow] Version mismatch");
    }
    HyprlandAPI::addDispatcher(PHANDLE, "wflow:look", look); 
    HyprlandAPI::addDispatcher(PHANDLE, "wflow:move", move);    
    HyprlandAPI::reloadConfig();
    return {"wflow", "Workflow plugin", "Tender-9", "1.0"};
}

APICALL EXPORT void PLUGIN_EXIT() {
    // ...
}
