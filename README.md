# glprofiler

A small reusable GPU profiler for OpenGL projects. Drops into a GL 3.3+ renderer as a git submodule and reports per pass GPU cost.

---

## Usage

Call `beginFrame` at the start of each frame  
Use `push` to mark the start of a region  
Use `pop` to mark the end of a region  
Call `endFrame` to resolve GPU timestamps and retrieve results  

Results can be accessed through the `results` function  

---

## Installation (via Git Submodule)

This profiler is intended to be added to projects as a Git submodule.

### 1. Add the Submodule

    git submodule add https://github.com/AdrianYip1/gl-profiler.git external/gl-profiler

### 2. Initialize Submodule

If cloning a repository that already includes it:

    git submodule update --init --recursive

---

## Build and Link (CMake)

The profiler ships its own `CMakeLists.txt`, which builds the static library
target `glprofiler` (aliased as `glp::profiler`). Adding only the include
directory is **not** enough — `src/profiler.cpp` must be compiled and linked, or
you will get `undefined reference to Profiler::push/pop` at link time.

`glp/profiler.h` includes `<glad/glad.h>`, so the library must be able to find
your GLAD loader headers. Point it at them with the `GLP_GL_INCLUDE_DIRS` cache
variable before adding the subdirectory:

    # Tell the profiler where to find <glad/glad.h>
    set(GLP_GL_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libraries/include CACHE STRING "" FORCE)

    add_subdirectory(external/gl-profiler)

    target_link_libraries(your_target PRIVATE glp::profiler)

Linking the target propagates the public include directory automatically, so no
separate `target_include_directories` call is needed.

Then include it in code:

    #include "glp/profiler.h"

---

## Requirements

OpenGL 3.3 or higher  
GLAD for OpenGL function loading  
A valid OpenGL context created before using the profiler  
Support for OpenGL timestamp queries  

---

## Integration

Include the profiler in your project and link against OpenGL  
Ensure GLAD is initialized before creating the profiler  
Use the profiler inside your render loop after the OpenGL context is active  

---

## Notes

Results depend on GPU completion and may stall if the GPU has not finished executing commands  
All timing is measured on the GPU and reflects actual render time rather than CPU time  
This profiler is intended for single threaded rendering workflows
