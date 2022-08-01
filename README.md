![Halo Decompilation Project](logo.png)

The goal of this project is to study and re-implement the original Xbox launch title [Halo: Combat Evolved](https://en.wikipedia.org/wiki/Halo:_Combat_Evolved).

**Disclaimer:** This project is intended only for educational and research purposes and is not indended to promote piracy or violation of any copyright laws. This repository does not include original executables or any game assets. You will need to provide these files from your own copy of the game. Buy a copy. Heck, buy two!

**Motivation:** Fans of Halo CE will be empowered to customize and enjoy this incredible classic in new ways, to understand the mechanics behind the game, and eventually port the game to run natively on more platforms. Moreover, this game serves as an exciting vehicle to drive advancements in decompilation research and tooling.

**Methodology:** Pieces of the game are slowly being re-implemented in C/C++ source code. These pieces are then compiled and patched into the original executable, such that the re-implemented pieces are used over their original implementation counterparts. This approach fosters incremental development, testing, and debugging, and accelerates the timeline for those who are interested in making game modifications.

Current State
-------------
- Able to patch and run existing game
- Main loop of the game is re-implemented
- Several more functions are implemented
- Long way to go...

Build
-----
First prepare `halo-patched` directory with disc files and original executable.

Build with the Docker container:
* `docker build -t halo .`
* `docker run -it --rm  -u $(id -u):$(id -g) -v $PWD:/work -w /work halo make`

Or build on your host system:
* Install deps: clang, llvm toolchain, python3
* Install Python deps: `pip install -r requirements.txt`
* Have `extract-xiso` in your path
* `make`

Run `halo-patched.iso` in xemu, or on your Xbox. You'll want to set up a debug environment.

Help
----
No support is provided at this time. Please do not ask for files, tools, etc.

Reversing
---------
Interested in reversing the game? PRs are welcome!

The process of adding re-implemented functions is mostly automated and relatively painless:
* Add new function/data declarations to `kb.json` as they are discovered and confirmed. The definitions in `kb.json` are used to automatically generate header files and link the new implementation with the original XBE.
* Implement new functions in the appropriate source file `src/halo/**/*.c`.
* When `build.py` is run, it will automatically pick up the new source files, build them, and patch the XBE with redirects from the original implementation to the re-implementation.
* Naturally, your new code will call some function in the original binary that has not yet been re-implemented. These functions will also be automatically linked correctly, provided the definitions of data and called functions are in `kb.json`.
