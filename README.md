<div align="center">
</div>

<br>

This repository is a Cube manipulation with OpenGL

## Usage

    - Add cube to plane
    - Cube texture brick mode enabled with 'T'
    - Change cube face color with key '1'-'6'
    - Auto rotation with key 'A', speed mod with wheel
    - Manual rotation with directional key UP/DONW/LEFT/RIGHT
    - Fancy rotation with mode Auto and press directional key

## Building

To configure and build the project, do the following from the root directory of the repository:

```bash
mkdir build
cd build
cmake ..
```

This will download necessary libraries, compiles and links to our CubeManipulator program

## Third Party Library Support

By default, the project will link with:
- GLAD
- GLFW
- OpenGL
- IrrXML
- stbimage

Make sure the system is equiped with the following 

1. Make sure you have a working build system with cmake
   
    ```bash
    sudo apt-get install -y cmake gcc build-essential
    ```

2. X11 may need to be installed for OpenGL window creation:

    ```bash
    sudo apt-get install libx11-dev xorg-dev libglu1-mesa-dev
    ```
