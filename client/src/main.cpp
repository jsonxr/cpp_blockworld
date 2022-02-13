#include <set>
#include <thread>

#include "constants.h"
#include "core.h"
#include "core/Assets.h"
#include "core/Camera.h"
#include "core/Material.h"
#include "core/textures/TextureAtlas.h"
#include "core/Window.h"
#include "importer/MinecraftImporter.h"
#include "utils/env.h"
#include "utils/memory.h"
#include "utils/Tracer.h"
#include "world/BlockMap.h"
#include "world/Chunk.h"

using namespace app;

constexpr auto kWindowTitle = "BlockWorld";

auto exitWithError(const char *msg) -> int {
  std::cerr << msg << std::endl;
  glfwTerminate();
  std::cerr << "goodbye" << std::endl;
  return EXIT_FAILURE;
}

static void error_callback(int error, const char *description) {
  std::cerr << "Error: " << error << "\n" << description << std::endl;
}

std::function<void()> loop;
void main_loop() { loop(); }

auto main2() -> int {  // NOLINT(bugprone-exception-escape)
  if (glfwInit() == 0) {
    return EXIT_FAILURE;
  }

  std::string path = Assets::getRootPath();
  std::cout << "path: " << path.c_str() << std::endl;

  glfwSetErrorCallback(error_callback);

  Window window{kWindowTitle, WindowSize{app::kDefaultWindowWidth,
                                         app::kDefaultWindowHeight}};

  if (window.nativeWindow() == nullptr) {
    return exitWithError("Failed to create GLFW window\n");
  }

  auto atlas = std::make_shared<TextureAtlas>(ivec2{1024, 1024});
  atlas->loadFromDirectory("minecraft:block/", "/minecraft/textures/block");
  atlas->loadFromDirectory("textures:block/", "/textures/block");
  atlas->save("test.png");

  auto block_map = std::make_shared<BlockMap>();
  auto material = std::make_shared<Material>(atlas);

  std::vector<std::shared_ptr<Chunk>> chunks{};
  std::vector<ChunkGfx> chunks_gfx{};

  for (int x = -kChunkRadius; x < +kChunkRadius + 1; x++) {
    for (int z = -kChunkRadius; z < +kChunkRadius + 1; z++) {
      cout << x << "," << z << endl;
      auto chunk =
          std::make_shared<Chunk>(vec3{x * kChunkWidth, 0, z * kChunkWidth});
      chunk->generate(*block_map);
      ChunkGfx chunk_gfx{material, *chunk, *block_map, *atlas};

      cout << "emplace chunk..." << endl;
      chunks.emplace_back(std::move(chunk));
      cout << "emplace chunkgfx..." << endl;
      chunks_gfx.emplace_back(std::move(chunk_gfx));
    }
  }

  // timing
  GLdouble delta_time{};
  GLdouble last_frame{};

  loop = [&] {
    // per-frame time logic
    // --------------------
    GLdouble current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    // input
    // -----
    app::Window::pollEvents();
    window.process(delta_time);

    glClearColor(0.F, 0.F, 0.F, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    for (const auto &c : chunks_gfx) {
      c.render(window.camera());
    }

    glfwSwapBuffers(window.nativeWindow());
  };

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(main_loop, 0, true);
#else
  while (!window.shouldClose() &&
         !window.input().isKeyPressed(GLFW_KEY_ESCAPE)) {
    loop();
  }
#endif

  window.close();
  glfwTerminate();
  return EXIT_SUCCESS;
}

[[maybe_unused]] auto main_import() -> int {
  importer::MinecraftImporter map{};
  map.load();
  map.get_block("minecraft:block/oak_stairs");

  return 0;
}

void print(int n, const std::string &str) {
  std::string msg = std::to_string(n) + " : " + str;
  std::cout << msg << std::endl;
}

auto main(int /*argc*/, char ** /*argv*/, char **envp) -> int {
  std::vector<std::string> s = {"Educative.blog", "Educative", "courses",
                                "are great"};
  std::vector<std::thread> threads{};

  for (int i = 0; i < s.size(); i++) {
    threads.push_back(std::thread(print, i, s[i]));
  }

  for (auto &th : threads) {
    th.join();
  }

  //  // utils::dump_env(envp);
  //  // utils::display_sizeof_values();
  //  auto total_memory = utils::getTotalSystemMemory();
  //  std::cout << "memory: " << utils::prettyBytes(total_memory) << "("
  //            << total_memory << ")" << std::endl;
  //  // test_memory();
  //  //  main_import();
  return main2();
  // return 0;
}
