# Supertris

A cross-platform super stacker game inspired by Tetris!

Written in C++ using the SFML library.

This project is far from complete, and is currently just a concept.

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
