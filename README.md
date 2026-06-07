
# 3DPhysics

## Prerequisites

This project is built for Linux environments and requires a standard C++ build toolchain along with GLFW, OpenGL, and GLM.

To install all required dependencies on Debian/Ubuntu-based systems, run:

```bash
sudo apt update
sudo apt install build-essential libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev libglm-dev
```

## Building and Running

1. **Compile the engine:**
Navigate to the project root directory and run:
```bash
make
```


2. **Launch the simulator:**
Once compilation is complete, execute the binary:
```bash
./simulator
```


3. **Clean the build:**
To remove compiled object files and the executable, run:
```bash
make clean
```

## Controls

The engine features a free-fly camera and an integrated Dear ImGui overlay for debugging and object summoning.

| Key / Action | Function |
| --- | --- |
| **`W` `A` `S` `D`** | Move the camera (Forward, Left, Backward, Right) |
| **`SPACE` / `L-SHIFT`** | Move the camera (Up / Down) |
| **Mouse Movement** | Look around (when cursor is locked to the window) |
| **`ESC`** | Unlock the mouse cursor |
| **`Left Click`** | Lock the mouse cursor to the game window |
| **`ENTER`** | Toggle Pause / Play simulation |
| **`TAB`** | Toggle the "Summon Object" GUI Menu |