CXXFLAGS = -DWLR_USE_UNSTABLE -shared -fPIC --no-gnu-unique -g -std=c++23 -O2
INCLUDES = $(shell pkg-config --cflags pixman-1 libdrm hyprland)
SRC = ./src/main.cpp
TARGET = ./wflow.so

ifneq ($(shell pkg-config --exists libcanberra && echo yes),)
	CXXFLAGS += $(shell pkg-config --cflags libcanberra)
	CXXFLAGS += -DHAVE_CANBERRA
	INCLUDES += $(shell pkg-config --libs libcanberra)
endif

all:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -o $(TARGET)
clean:
	rm -f $(TARGET)
