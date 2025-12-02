#ifndef APP_TEXTURES_TEXTURE_RECT_H
#define APP_TEXTURES_TEXTURE_RECT_H

#include <filesystem>
#include <string>
#include <vector>

#include "../../core.h"

namespace app {

struct TextureRect {
 public:
  uint16 id{};
  std::string name;
  uint16 x{};
  uint16 y{};
  uint16 width{};
  uint16 height{};
  std::filesystem::path path;
  vec4 uv{};
};

}  // namespace app

#endif  // APP_TEXTURES_TEXTURE_RECT_H
