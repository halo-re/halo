![Halo Decompilation Project](logo.png)

The goal of this project is to study and re-implement the original Xbox launch title [Halo: Combat Evolved](https://en.wikipedia.org/wiki/Halo:_Combat_Evolved).

**Disclaimer:** This project is intended only for educational and research purposes and is not indended to promote piracy or violation of any copyright laws. This repository does not include original executables or any game assets. You will need to provide these files from your own copy of the game. Buy a copy. Heck, buy two!

**Motivation:** Fans of Halo CE will be empowered to customize and enjoy this incredible classic in new ways, to understand the mechanics behind the game, and eventually port the game to run natively on more platforms. Moreover, this game serves as an exciting vehicle to drive advancements in decompilation research and tooling.

**Methodology:** Pieces of the game are slowly being re-implemented in C/C++ source code. These pieces are then compiled and patched into the original executable, such that the re-implemented pieces are used over their original implementation counterparts. This approach fosters incremental development, testing, and debugging, and accelerates the timeline for those who are interested in making game modifications.

Community
---------
There is a Discord server for the project: https://discord.gg/z4wjN2rQ

Current State
-------------
* Able to patch and run existing game
* Main loop of the game is re-implemented
* Several more functions are implemented
* Long way to go...

Build
-----
You can build with Visual Studio or clang, on Windows/macOS/Linux. You'll also need Python 3.

* First prepare `halo-patched` directory with disc files and original executable
  * Retail disc game files
  * Original executable version `01.10.12.2276` (MD5: `c7869590a1c64ad034e49a5ee0c02465`) as file `cachebeta.xbe`
* You can build in a Docker container, or outside of a container with system tools (e.g. clang, MSVC).
* If not using a container to build, install system deps:
  * Ensure Python 3 is installed.
  * On Windows you can use Visual Studio (MSVC).
  * On Linux/macOS/WSL you can use clang:
    * `sudo apt install cmake clang lld python3-pip`
  * Install Python deps: `pip install --user -r requirements.txt`

### Build options

Build with the Docker container:
```
docker build -t halo .
docker run -it --rm  -u $(id -u):$(id -g) -v $PWD:/work -w /work halo /bin/bash -c "cmake -Bbuild -S. -DCMAKE_C_COMPILER=clang && cmake --build build"
```

Build with CMake (Linux, clang):
```
cmake -Bbuild -S. -DCMAKE_C_COMPILER=clang
cmake --build build
```

Build with CMake (Windows, MSVC):
```
cmake -AWin32 -Bbuild -S.
cmake --build build
```

When the build is complete, the original game with re-implementation patched in will be at `halo-patched/default.xbe`. Use `extract-xiso` to create an ISO from your `halo-patched` directory, then run `halo-patched.iso` in xemu, or on your Xbox.

You'll want to set up a debug environment.

Reversing
---------
Interested in reversing the game? PRs are welcome!

The process of adding re-implemented functions is mostly automated and relatively painless:
* Add new function/data declarations to `kb.json` as they are discovered and confirmed. The definitions in `kb.json` are used to automatically generate header files and link the new implementation with the original XBE.
* Implement new functions in the appropriate source file `src/halo/**/*.c`. Add new source files to src/CMakeLists.txt.
* The build system will compile and patch the XBE with redirects from the original implementations to the re-implementations.
* Naturally, your new code will call some function in the original binary that has not yet been re-implemented. These functions will also be automatically linked correctly, provided the definitions of data and called functions are in `kb.json`.
