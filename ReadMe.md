# Raylib Noise Grid v0.2

A raylib application that generates a grid of noise using the FastNoise library.

Simply put, a 3D noise generator that can be edited in real-time.

__Features__

- Grid generation using FastNoise
- GUI using raygui
- Live editing and preview of noise
- the missing line from the DrawMeshInstanced() examples

(C) 2026 WLGfx - Carl S Norwood (Sun 25 Jan 2026 13:11 GMT)

https://github.com/wlgfx/rlNoiseGrid (When it's uploaded)

__License__

MIT License

Just for fun, no warranty of any kind.

__Building__

- VSCode or similar IDE with CMake Tools extension
- Raylib 5.5 is included for now in the source (no need to install it)
- GCC or Clang to build
- open the ${rlNoiseGrid}/ directory in your IDE
- CMake Tools should pick up the CMakeLists.txt file
- If it doesn't, try deleting the build directory and then building again.
- Again, Delete the build directory before building if you get any errors

If the CMake Tools extension is working and picks up the CMakeLists.txt file, it should build automatically. If it doesn't, try deleting the build directory and then building again.

On the bottom of my screen I have the 'build' and 'play' buttons.

Good Luck...

## Next steps

- Better materials and lighting
- Multiple noise maps using merging and/or blending
- still thinking...
- actual flight controls to make the feel of flight
- marching squares/cubes

__Done__

- Revamped UI handling (Tue 27 Jan 2026 19:51 GMT - Carl)

