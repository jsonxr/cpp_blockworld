cmake_minimum_required(VERSION 3.21)
project(blockworld)

set(CMAKE_CXX_STANDARD 20)
set(CONAN_CMAKE_CXX_STANDARD 20)

set(CONAN_COMPILER "apple-clang")
set(CONAN_COMPILER_VERSION "14.0")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (!EMSCRIPTEN)
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy;--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy;-p ${CMAKE_BINARY_DIR}/compile_commands.json")
endif ()

# Conan package manager include
set(CONAN_COMPILER "cmake")
set(CONAN_COMPILER_VERSION "13.0")

if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else ()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif ()

include_directories(${PROJECT_SOURCE_DIR}/client/include ${PROJECT_SOURCE_DIR}/client/src)

list(APPEND SOURCES
        # src/vendor
        client/src/vendor/stb.cpp
        # src/core/webgl
        client/src/core/webgl/GLProgram.cpp
        client/src/core/webgl/GLShader.cpp
        client/src/core/webgl/GLUniform.cpp
        # src/core/textures
        client/src/core/textures/TextureAtlas.cpp
        # src/core
        client/src/core/Assets.cpp
        client/src/core/BoxGeometry.cpp
        client/src/core/BufferGeometry.cpp
        client/src/core/Camera.cpp
        client/src/core/Input.cpp
        client/src/core/Logger.cpp
        client/src/core/Material.cpp
        client/src/core/Window.cpp
        #src/importer
        client/src/importer/MinecraftImporter.cpp
        #src/vendor
        client/src/vendor/SimplexNoise.cpp
        #src/world
        client/src/world/Chunk.cpp
        client/src/world/BlockMap.cpp
        client/src/world/Block.cpp
        # src
        client/src/main.cpp
        client/src/utils/math.cpp
        client/src/utils/memory.cpp
        client/src/utils/executable.cpp


        client/src/core/textures/Image.cpp
        client/src/world/World.cpp
        client/src/utils/env.cpp

        client/src/core/BlockMaterial.cpp
        client/src/core/Vertex.cpp
        )


if (EMSCRIPTEN)
    add_compile_options("SHELL:-s USE_PTHREADS -pthread")
endif ()

add_executable(blockworld ${SOURCES})
target_link_libraries(blockworld ${CONAN_LIBS})

if (EMSCRIPTEN)
    #set(EMCC_LINKER_FLAGS " -s WASM=1 -O3 -o ../index.js -s --pre-js pre-module.js --post-js post-module.js -s USE_GLFW=3 -s OFFSCREENCANVAS_SUPPORT=1")

    # Optimization flags for emcc
    # https://emscripten.org/docs/optimizing/Optimizing-Code.html
    # -O0 No optimizations (default)
    # -O3 or
    # -Oz   but reduces code size even further, and may take longer to run. This can affect both wasm and JavaScript
    # --bind Embind is used to bind C++ functions and classes to JavaScript, so that the compiled code can be used in a natural way by “normal” JavaScript. Embind also supports calling JavaScript classes from C++.
    # --embed-file <file> Specify a file (with path) to embed inside the generated JavaScript.
    # -gsource-map enerate a source map using LLVM debug information
    # -Wwarn-absolute-paths
    # --cpuprofiler Use this to perform cursory interactive performance profiling
    # --memoryprofiler Embeds a memory allocation tracker onto the generated page. Use this to profile the application usage of the Emscripten HEAP
    # --threadprofiler Embeds a thread activity profiler onto the generated page. Use this to profile the application usage of pthreads when targeting multithreaded builds (-s USE_PTHREADS=1/2)


    # Optimize WebGL
    # https://emscripten.org/docs/optimizing/Optimizing-WebGL.html
    # FULL_ES3= This emulation is expected to hurt performance.  if one needs to render from client side memory, or the use of glMapBuffer*() API is needed

    # Debugging
    # https://emscripten.org/docs/porting/Debugging.html
    # ASSERTIONS=1 if disabled, it also disables Stack_overflow_check
    # DEMANGLE_SUPPORT=1 links in code to automatically demangle stack traces, that is, emit human-readable C++ function names
    # SAFE_HEAP (adds additional memory checks and give clear errors during debug)
    # SAFE_HEAP_LOG (writes safe heap operations
    # STACK_OVERFLOW_CHECK=2 enables slightly more detailed stack guard checks, which can give a more precise callstack at the expense of some performance


    #    list(APPEND FLAGS "-O0")
    #    list(APPEND FLAGS "--cpuprofiler")
    #    list(APPEND FLAGS "--memoryprofiler")
    #    list(APPEND FLAGS "-s EXIT_RUNTIME=1")
    #    list(APPEND FLAGS "--bind")
    #    list(APPEND FLAGS "-s INITIAL_MEMORY=512MB")
    #    list(APPEND FLAGS "-s WASM=1")
    #    list(APPEND FLAGS "-s ALLOW_MEMORY_GROWTH=1")
    #    list(APPEND FLAGS "-s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2")
    #    list(APPEND FLAGS "-s USE_GLFW=3")
    #    list(APPEND FLAGS "-s WASM_BIGINT=1")
    #    list(APPEND FLAGS "-std=c++${CMAKE_CXX_STANDARD}")
    #    list(APPEND FLAGS "--embed-file ${PROJECT_SOURCE_DIR}/client/assets@/assets")
    #    list(APPEND FLAGS "-s ASSERTIONS=1")
    #    list(APPEND FLAGS "-s DEMANGLE_SUPPORT=1")
    #    list(APPEND FLAGS "-s GL_ASSERTIONS=1")
    #
    #    list(JOIN FLAGS " " FLAGS)
    #
    #    set(EMCC_LINKER_FLAGS "${FLAGS}")


    set(EMCC_LINKER_FLAGS "-O0 --cpuprofiler --memoryprofiler -s USE_PTHREADS -pthread -s PTHREAD_POOL_SIZE=4 -s EXIT_RUNTIME=1 --bind -s INITIAL_MEMORY=512MB -s WASM=1  -s ALLOW_MEMORY_GROWTH=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s USE_GLFW=3 -s WASM_BIGINT=1 -std=c++${CMAKE_CXX_STANDARD} --embed-file ${PROJECT_SOURCE_DIR}/client/assets@/assets -s ASSERTIONS=1 -s DEMANGLE_SUPPORT=1 -s GL_ASSERTIONS=1")

    set(CMAKE_EXECUTABLE_SUFFIX .html)
    set_target_properties(blockworld PROPERTIES LINK_FLAGS ${EMCC_LINKER_FLAGS})
else ()
    file(COPY client/assets DESTINATION ${CMAKE_BINARY_DIR}/bin)
endif ()
