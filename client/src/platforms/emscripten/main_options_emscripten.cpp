#include <emscripten/threading.h>

#include "Application.h"
#include "core.h"

namespace app {

auto parseApplicationOptions(int argc, char **argv, char **envp)
    -> expected<ApplicationOptions, string> {
  ApplicationOptions options{};
  options.headless = false;
  options.threads = emscripten_num_logical_cores();
  return options;
}

}  // namespace app
