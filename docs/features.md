# Memory

- With chunk memory, how do we avoid holes? Would a ring buffer help at all?

# Finite State Machine

- [tiny fsm](https://github.com/digint/tinyfsm) - fsm written for low memory

# Noise

- [Simplex Noiise](https://github.com/SRombauts/SimplexNoise)
- [Seed a perlin noise](https://solarianprogrammer.com/2012/07/18/perlin-noise-cpp-11/)
- [Minecraft terrain generator](https://www.youtube.com/watch?v=FE5S2NeD7uU&t=97s) - Really high level overview of what
  happens. not very informative. done in stages: stone, then replace with caves, then ores, then decorations.
- [Open Terrain Generator](https://github.com/PG85/OpenTerrainGenerator) - Fork and more maintained than Terrain Control
- [Terrain control](https://github.com/MCTCP/TerrainControl) - Implementation of terrain generation

# Lighting

- Ambient
- Colored lights
- block light levels

# Serialization

# Threading

SharedArrayBuffer requires cross origin isolation

- [Shared Array Buffer](https://developer.chrome.com/blog/enabling-shared-array-buffer/)
- [cross-origin isolation guide](https://web.dev/cross-origin-isolation-guide/)
- [concurrent queue](https://github.com/cameron314/concurrentqueue)

# Scripting

- [quickjs](https://bellard.org/quickjs/) - es6 js interpreter
- [figma's plugin](https://www.figma.com/blog/how-we-built-the-figma-plugin-system/) - example of a low level interface
  to a jsvm

# Portability

- [wasm edge](https://wasmedge.org/) - wasm edge for server side webassembly
- [wapc](https://wapc.io/) - higher level web assembly interop
- [sokol-gfx](https://github.com/floooh/sokol)
- [Embed wasm in C++](https://github.com/WebAssembly/wasm-c-api) - Might be useful for providing server side plugins
- [Portability Issues according to oryol](https://github.com/floooh/oryol/blob/master/doc/OVERVIEW.md):
    - constexpr isn't supported in Visual Studio,
    - thread_local is a mess
    - chrono::high_resolution_clock isn't high-resolution on Windows
- [Sane C++](http://flohofwoe.blogspot.com/2013/06/sane-c.html) - Opinionated article about what should/should not be
  used in c++ witten almost 10 years ago. Might be outdataed but by the author of oryol.

# Portable Graphics Libraries

- [oryol](https://github.com/floooh/oryol) - NOTMAINTAINED 3d framework in c++ with wasm? Produces small executables (
  emscripten starts at 100k). Referenced on
  the [awesome-wasm page](https://github.com/mbasso/awesome-wasm#non-web-embeddings) under WebGL. Renders to OpenGL or
  D3d.
- [bgfx](https://github.com/bkaradzic/bgfx) - C++ with lots of bindings. Shader language close to glsl. Lots of games
  use it.
- [Babylon native](https://github.com/BabylonJS/BabylonNative#babylon-native) - Babylon.js for native... uses bgfx?
- [toy](https://github.com/hugoam/toy) - built on top of bgfx

# ECS

- [Bounty Hunter Demo](https://github.com/tobias-stein/BountyHunterDemo) - ECS example
- [entity component system](https://indiegamedev.net/2020/05/19/an-entity-component-system-with-data-locality-in-cpp/)

# Multiplayer

- [](https://www.youtube.com/watch?v=uY1Y8C4gmq0) - minecraft world records 2,600 on one server or 4,000 across 64.
- [World QL](https://www.worldql.com/) - spatial database written in rust
- [github worldql](https://github.com/WorldQL/mammoth) - source code for minecraft worldql
