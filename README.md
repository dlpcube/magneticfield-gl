# magneticfield-gl

A 3D magnetic field visualizer made with an OpenGL backend. Implements lighting and vectors to show the direction and strength of the field at different cartesian coordinates. Field strength is also contingent on the object shape and material just like in real life. Also implements a 3D camera for user control.

## Building

Run all commands from the repository root. The Makefile detects macOS and Windows/MinGW automatically.

### macOS

Install Xcode Command Line Tools and GLFW:

```sh
xcode-select --install
brew install glfw pkg-config
```

Then build and run:

```sh
make clean
make
./project
```

### Windows

Install MSYS2, open the **UCRT64** terminal, and install the compiler, GLFW, and Make:

```sh
pacman -Syu
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-glfw make
```

From the repository root in that terminal:

```sh
make clean
make
./project.exe
```

The program must be launched from the repository root because shaders and images use paths relative to it.
