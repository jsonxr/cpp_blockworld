# BlockWorld clone

Following along with the [YouTube series](https://www.youtube.com/watch?v=HhH_9DnMBgw).

# Setup

Install

- direnv `brew install direnv`
- cmake `brew install cmake`
- [emscripten](https://emscripten.org/)

  ```sh
  # Install
  cd /opt
  git clone https://github.com/emscripten-core/emsdk.git
  cd /opt/emsdk
  ```

  ```sh
  # Update to latest version
  cd /opt/emsdk
  git pull
  ./emsdk install latest
  ./emsdk activate latest
  ```

  ```sh
  # .zprofile
  export EMSDK_HOME=/opt/emsdk
  source $EMSDK_HOME/emsdk_env.sh > /dev/null 2>&1
  ```

- [Conan package manager](https://conan.io/) - `brew install conan`
- Copy minecraft files...

  ```shell
  # Show available versions of minecraft on a mac
  bin/minecraft-extract
  # Extract specific version of minecraft
  bin/minecraft-extract 1.18.2 extracted
  ```

# Compile

```sh
# libc++ vs stdlibc++ might depend on platform...
# https://stackoverflow.com/questions/14972425/should-i-use-libc-or-libstdc
# Install
conan install . -s build_type=Debug -s compiler.libcxx=libc++ -s cppstd=20 --install-folder=build --build missing
cd build && cmake .. && cmake --build .

# optional...
# This will use the assets in the project instead of relying on cmake to copy.
# This avoids being confused when you change a shader and nothing happens.
# BLOCKWORLD_ASSETS_PATH=$(pwd)/client/assets

# Run app...
bin/blockworld

# Release dependencies
# conan install . -s build_type=Release -s compiler.libcxx=libc++ -s cppstd=20 --install-folder=cmake-build-release --build missing
```

# Wasm

Uses Experimental Features: [WebAssembly Roadmap](https://webassembly.org/roadmap/)

- Exceptions (Chrome, In progress: Firefox, No: Safari) - We just have to deal with crashes until this is supported
- Threads and atomics (Chrome, Firefox, In progress: Safari) - Not used yet, but we will want to...
- WebGPU

```sh
conan install . -s build_type=Debug -s compiler.libcxx=libc++ -s cppstd=20 --install-folder=build-wasm --build missing
cd build-wasm && emcmake cmake .. && cmake --build .

# Run app...
npx http-server bin  # This assumes node is installed...
open http://127.0.0.1:8080/blockworld.html

#emcc src/main.cpp -o cmake-build-wasm/index.html -Iinclude -s USE_WEBGL2=1 -s USE_GLFW=3 -s WASM=1 -std=c++20 -fwasm-exceptions
```

[wasm cmake dependencies](https://stackoverflow.com/questions/55869531/how-to-use-emscripten-with-cmake-for-project-dependencies)

### Exceptions

- [roadmap](https://webassembly.org/roadmap/)
- [zero cost exceptions](https://github.com/WebAssembly/design/issues/1078)
- [-fwasm-exceptions (enable wasm exceptions)](https://emscripten.org/docs/porting/exceptions.html)
- [-s DISABLE_EXCEPTION_CATCHING=0 (is this old?)](https://brionv.com/log/2019/10/24/exception-handling-in-emscripten-how-it-works-and-why-its-disabled-by-default/)

# CLion

- [CLion IDE](https://www.jetbrains.com/clion/)
- [CLion Conan plugin](https://intellij-support.jetbrains.com/hc/en-us/community/posts/360008851580-How-to-use-Conan-Package-Manager-for-C-on-CLion-Mac)
- [GLSL Support plugin](https://plugins.jetbrains.com/plugin/6993-glsl-support)

```sh
# Install conan dependencies
conan install . -s build_type=Debug --install-folder=build-cmake-debug --build missing
# dont do below, click on that conan install at the bottom and then reload the cmake project in the ide
#cd cmake-build-debug && cmake ..
```

# VSCode

Install the following plugins, then follow compile directions.

- [C/C++ for VSCode](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
- [CMake Tools for VSCode](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
- [Clang-Tidy for VSCode](https://marketplace.visualstudio.com/items?itemName=notskm.clang-tidy)
- [Conan for VSCode](https://marketplace.visualstudio.com/items?itemName=disroop.conan)
- [Visual Studio Code - Get started with CMake Tools](https://code.visualstudio.com/docs/cpp/CMake-linux)
- clang-tidy, clang-format: `brew install llvm`

# Resources

- [memory](https://www.gamedev.net/articles/programming/general-and-gameplay-programming/c-custom-memory-allocation-r3010/)
  - Efficient memory allocation in games
- [optick](https://optick.dev/) - super-lightweight c++ profiler for games
- [setup on mac. need to code sign?](https://giovanni.codes/opengl-setup-in-macos/)
- [Better way to handle opengl resources](https://github.com/polytypic/gl.cpp)
- [glTF 2.0 Overview](https://kcoley.github.io/glTF/specification/2.0/figures/gltfOverview-2.0.0a.png)
- [Gaffer on Games - Fix your timestep](https://gafferongames.com/post/fix_your_timestep/)
- [Minecraft Clone](https://github.com/codingminecraft/StreamMinecraftClone)
- [Learn OpenGL](https://learnopengl.com)
- [WebGL2 fundamentals](https://webgl2fundamentals.org/)
- [clang-tidy list of checks](https://clang.llvm.org/extra/clang-tidy/checks/list.html)
- [Use turns, not radians...](https://www.computerenhance.com/p/turns-are-better-than-radians)
- [better than jwt](https://paseto.io/)

# C++ Standard

https://en.cppreference.com/w/cpp/compiler_support

# Libraries

- [boost/1.78.0](https://www.boost.org/) - Boost library. BSL-1.0 License
- [entt](https://github.com/skypjack/entt) - Entity Component System. MIT License
- [expected-lite](https://github.com/martinmoene/expected-lite) - cpp23 std::expected implementation. BSL-1.0 License
- [glad](https://github.com/Dav1dde/glad) - GL bootstrap. MIT License
- [glfw](https://www.glfw.org/) - GL windowing library. zlib/libpng License
- [glm](https://github.com/g-truc/glm) - GL Math. The Happy Bunny License or MIT License
- [nlohmann_json/3.10.5](https://github.com/nlohmann/json) - JSON parsing library. MIT License
- [stb](https://github.com/nothings/stb) - image processing MIT License or Public Domain
- [tinygltf](https://github.com/syoyo/tinygltf) - GLTF file loading. MIT License

### Under consideration multithreading...

- [Bungee's multithreading talk](https://www.youtube.com/watch?v=v2Q_zHG3vqg)
- [Ron Fosner's Task-Based Multithreading](https://www.gdcvault.com/play/1012321/Task-based-Multithreading-How-to) Job's are ideally between 0.5-2 miliseconds so kinda big...
- [taskflow](https://github.com/taskflow/taskflow) - Task system for multi-threading. MIT License.

# Tools

- [block bench](https://www.blockbench.net/downloads) - low polygon editor

# TODO

- Modify cmake to recompile for asset change
  - [blog article](https://jeremimucha.com/2021/05/cmake-managing-resources/)
  - [gist](https://gist.github.com/jamcodes/f79b3be24ed40c6c224cc9e91d3061cc)
- per vertex data... make vertex color use rgba=4bytes instead of 4 floats
- [Faster compression than zlib](https://github.com/ebiggers/libdeflate)

# WebGL2

- TextureArray (Bug in m1 mac that causes it to fail)
