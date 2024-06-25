# Supertris

A cross-platform super stacker game inspired by Tetris!

Written in C++ using the SFML library.

<!-- TODO: screenshot -->
<!-- TODO: controls and config info -->

## Project Goals

This project is far from complete, and is currently mostly a concept.

- Build a fully-featured, mostly Tetris-guideline-compatible stacker game
- Allow configurations for advanced gameplay (DAS, ARR, etc.)
- Extend this game with more modes, larger boards, special features, etc.
- Add graphical effects while focusing on high performance
- Allow to be built and played cross-platform

## Building

This project's CMakeLists was based on the [Cmake SFML Project Template](https://github.com/SFML/cmake-sfml-project). Here is how to build:

For a single-configuration generator (typically the case on Linux and macOS):
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

For a multi-configuration generator (typically the case on Windows):
```
cmake -S . -B build
cmake --build build --config Release
```
