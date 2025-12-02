#include "env.h"

#include <cstdlib>
#include <iostream>
#include <string>

#include "executable.h"

namespace utils {

void dump_env(char **envp) {
  if (envp == nullptr) {
    return;
  }
  for (char *const *env = envp; *env != nullptr; env++) {
    std::cout << *env << '\n';
  }
}

auto get_env_var(std::string const &key) -> std::string {
  const char *val = std::getenv(key.c_str());
  return val == nullptr ? std::string{} : std::string{val};
}

auto get_assets_path() -> std::string {
  auto val = get_env_var("BLOCKWORLD_ASSETS_PATH");
  if (!val.empty()) {
    return val;
  }

#ifdef __EMSCRIPTEN__
  const std::string k_dir = "";
#else
  const std::string k_dir = get_executable_dir();
#endif
  return k_dir + "/assets";
}

}  // namespace utils
