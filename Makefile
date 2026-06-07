# compiler and flags
CXX = g++
CXXFLAGS = -Wall -g -I. -I./imgui -DGLM_ENABLE_EXPERIMENTAL
LIBS = -lGL -lGLU -lglfw

#source files etc
IMGUI_SOURCES = imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/imgui_impl_glfw.cpp imgui/imgui_impl_opengl3.cpp
SOURCES = simulator.cpp engine.cpp objects.cpp camera.cpp $(IMGUI_SOURCES)
EXECUTABLE = simulator


# default
all: check_libs ${EXECUTABLE}

# compile rule
$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE) $(LIBS)

# for cleaning
clean:
	rm -f $(EXECUTABLE) *.o


# checking if the libraries are installed
check_libs:
	@if ! ldconfig -p | grep -q libGL.so; then \
		echo "Installing missing libraries..." \
		sudo apt-get install -y libgl1-mesa-dev libglu1-mesa-dev libglfw3-dev; \
	fi

.PHONY: