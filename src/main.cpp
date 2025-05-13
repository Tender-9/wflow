#include <cstddef>
#include <hyprland/src/SharedDefs.hpp>
#include <hyprland/src/helpers/Monitor.hpp>
#include <hyprland/src/plugins/PluginAPI.hpp>
#include <hyprland/src/Compositor.hpp>
#include <hyprlang.hpp>
#include <unistd.h>

#ifdef HAVE_CANBERRA
#include <canberra.h>
#include <chrono>
#include <thread>
#include <atomic>
#endif

inline HANDLE PHANDLE = nullptr;

#ifdef HAVE_CANBERRA
static std::atomic<int> bellThreadCount(0);
const int               maxbellThreads = 3;

void canberraBell() {
    ca_context* context;
    ca_context_create(&context);
    ca_context_play(context, 0, CA_PROP_EVENT_ID, "bell-terminal", CA_PROP_EVENT_DESCRIPTION, "bounds", NULL);
    int playing;
    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ca_context_playing(context, 0, &playing);
    } while (playing == 1);
    ca_context_destroy(context);
    bellThreadCount.fetch_sub(1, std::memory_order_release);
    return;
}

void playBell() {
    if (bellThreadCount.load(std::memory_order_acquire) < maxbellThreads) {
        bellThreadCount.fetch_add(1, std::memory_order_acq_rel);
        std::thread canberra_thread(canberraBell);
        canberra_thread.detach();
    }
    return;
}

#else
void playBell() {
    write(STDOUT_FILENO, "\a", 1);
    return;
}
#endif


std::string getMotion(const char direction) {
    static auto const* ws_per_mon     = (Hyprlang::INT* const*)HyprlandAPI::getConfigValue(PHANDLE, "plugin:wflow:workspaces_per_monitor")->getDataStaticPtr();
    auto               monitors       = g_pCompositor->m_monitors;
    std::size_t        monitor_count  = monitors.size();
    CMonitor*          active_monitor = g_pCompositor->m_lastMonitor.get();
    PHLWINDOW          active_window  = g_pCompositor->m_lastWindow.lock();
    int                workspace_ID   = active_monitor->activeWorkspaceID();
    CBox               mon_bounds     = active_monitor->logicalBox();
    CBox               win_bounds;
        
    if (!active_window)
        win_bounds = mon_bounds;
    else
        win_bounds = active_window->getWindowIdealBoundingBoxIgnoreReserved();

    switch (direction) {
        case 'u':
            if (win_bounds.y > mon_bounds.y)
                return "win";
            if ((workspace_ID + **ws_per_mon - 1) % **ws_per_mon != 0)
                return "wks";
            break;
        case 'd':
            if (win_bounds.y + win_bounds.h < mon_bounds.y + mon_bounds.h)
                return "win";
            if (workspace_ID % **ws_per_mon != 0)
                return "wks";
            break;
        case 'l':
            if (win_bounds.x > mon_bounds.x)
                return "win";
            if (active_monitor -> m_id > 0)
                return "wks";
            break;
        case 'r':
            if (win_bounds.x + win_bounds.w < mon_bounds.x + mon_bounds.w)
                return "win";
            if (active_monitor -> m_id + 1 < monitor_count)
                return "wks";
        default: Debug::log(ERR, "[wflow] Invalid directoin"); break;
    }
    return "";
}


void wflow(const char mode, std::string direction) {
    const char  dir    = std::tolower(direction[0]);
    std::string motion = getMotion(dir);
    if (motion == "") {
        static auto const* enable_bell = (Hyprlang::STRING const*)HyprlandAPI::getConfigValue(PHANDLE, "plugin:wflow:enable_bell")->getDataStaticPtr();
        if (std::string(*enable_bell) == "true")
            playBell();
        return;
    }
    std::string args;
    switch (mode) {
        case 'l':
            if (motion == "win")
                args = "movefocus " + std::string(1, dir);
            else if (direction == "u")
                args = "workspace r-1";
            else if (direction == "d")
                args = "workspace r+1";
            else
                args = "movefocus " + direction;
            break;
        case 'm':
            if (motion == "win")
                args = "movewindow " + std::string(1, dir);
            else if (direction == "u")
                args = "movetoworkspace r-1";
            else if (direction == "d")
                args = "movetoworkspace r+1";
            else
                args = "movewindow " + direction;
            break;
        default: break;
    }
    HyprlandAPI::invokeHyprctlCommand("dispatch", args);
}


SDispatchResult dispatchLook(std::string args) {
    SDispatchResult result;
    result.passEvent = true;
    wflow('l', args);
    return result;
}

SDispatchResult dispatchMove(std::string args) {
    SDispatchResult result;
    result.passEvent = true;
    wflow('m', args);
    return result;
}

SDispatchResult dispatchBell(std::string args) {
    SDispatchResult result;
    result.passEvent = true;
    playBell();
    return result;
}


APICALL EXPORT std::string PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE = handle;

    // ALWAYS add this to your plugins. It will prevent random crashes coming from
    // mismatched header versions.
    const std::string HASH = __hyprland_api_get_hash();
    if (HASH != GIT_COMMIT_HASH) {
        throw std::runtime_error("[wflow] Version mismatch");
    }

    HyprlandAPI::addConfigValue(PHANDLE, "plugin:wflow:enable_bell", Hyprlang::STRING{"false"});
    HyprlandAPI::addConfigValue(PHANDLE, "plugin:wflow:workspaces_per_monitor", Hyprlang::INT{4});

    HyprlandAPI::addDispatcherV2(PHANDLE, "wflow:look", dispatchLook);
    HyprlandAPI::addDispatcherV2(PHANDLE, "wflow:move", dispatchMove);
    HyprlandAPI::addDispatcherV2(PHANDLE, "wflow:bell", dispatchBell);

    HyprlandAPI::reloadConfig();

    return {"wflow", "A workflow plugin", "Tender-9", "1.0"};
}

APICALL EXPORT void PLUGIN_EXIT() {
    // ...
}
