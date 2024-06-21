all:
	$(CXX) -DWLR_USE_UNSTABLE -shared -fPIC --no-gnu-unique src/main.cpp src/globals.hpp src/activewindow.cpp src/wincontroller.cpp -o my-plugin.so -g `pkg-config --cflags pixman-1 libdrm hyprland pangocairo libinput libudev wayland-server xkbcommon` -std=c++2b -O2
clean:
	rm ./my-plugin.so
