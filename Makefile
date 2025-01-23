# compiler and flags
CXX = g++
CXXFLAGS = -Wall -g
LIBS = -lGL -lGLU -lglfw

#source files etc
SOURCES = simulator.cpp engine.cpp objects.cpp camera.cpp
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