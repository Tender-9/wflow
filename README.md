# wflow
A tiny Hyprland plugin for navigating windows and workspaces

## Configuration

### Options:
| Option | Description | Default |
| -      | -           | -       |
| enable_bell | Enable terminal bell on bounds | false |
| workspaces_per_monitor | The number of workspaces per monitor | 4 |

### Dispatchers:
| Dispatcher | Description | Params |
| -          | -           | -      |
| wflow:look  | Replacement for movefocus & workspace | u, d, l, r|
| wflow:move  | Replacement for movewindow & movetoworkspace | u, d, l, r |

### Example Config:
```
plugin {
  wflow {
    enable_bell = false
    workspaces_per_monitor = 4
  }
}

animations {
    animation = workspaces, 1, 4, default, slidevert
}

workspace = 1, monitor:$sideMonitor
workspace = 2, monitor:$sideMonitor
workspace = 3, monitor:$sideMonitor
workspace = 4, monitor:$sideMonitor

workspace = 5, monitor:$mainMonitor
workspace = 6, monitor:$mainMonitor
workspace = 7, monitor:$mainMonitor
workspace = 8, monitor:$mainMonitor

bind = ALT SHIFT, K, wflow:move, u
bind = ALT SHIFT, J, wflow:move, d
bind = ALT SHIFT, H, wflow:move, l
bind = ALT SHIFT, L, wflow:move, r

bind = ALT, K, wflow:look, u
bind = ALT, J, wflow:look, d
bind = ALT, H, wflow:look, l
bind = ALT, L, wflow:look, r
```
