# Raytracing Experiments

This is my collection of experiments with raytracing and conventional rasterizing pipelines. The aim is to create a small app that can load a GLTF scene and display it using different rendering methods and pipelines, and compare the results side by side.

This is not a game engine. This isn't even a rendering engine as its asset management is very *very* basic. It's just my personal sandbox to use while learning ins and outs of graphics programming.

## How to build

You will need Vulkan SDK, CMake, a C++20 compatible compiler and a recent version of the drivers for your GPU. After all of that is ready and you have cloned the repository run:

```
mkdir build
cd build
cmake .. -GNinja
ninja
```

## License

Every part of the source code that was created by me is under the MIT license. For third party libraries that are included directly in the source tree and not as submodules please refer to their license files and/or headers at the top of corresponding files.
