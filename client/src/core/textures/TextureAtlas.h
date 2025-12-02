#ifndef APP_TEXTURES_TEXTUREATLAS_H
#define APP_TEXTURES_TEXTUREATLAS_H

#include <optional>
#include <vector>

#include "../../core.h"
#include "Image.h"
// include "Texture.h"
#include "TextureRect.h"

namespace app {

constexpr uint16 kTextureAtlasDefaultSize = 1024;
constexpr uint16 kTextureAtlasMinSize = 16;

using TextureGrid = std::vector<int>;
using TexturePixels = std::vector<Pixel>;  // 4_194_304

class TextureAtlas {
 public:
  TextureAtlas() = default;
  explicit TextureAtlas(const ivec2 &size,
                        uint8 min_size = kTextureAtlasMinSize);

  [[nodiscard]] auto width() const noexcept -> uint16;
  [[nodiscard]] auto height() const noexcept -> uint16;
  [[nodiscard]] auto min_size() const noexcept -> uint16;

  auto pack() -> std::unique_ptr<TextureGrid>;
  auto compile() -> GLuint;
  auto generatePixels() -> std::unique_ptr<TexturePixels>;
  auto add(TextureRect &&region) -> uint16;
 void save(const std::string &filename);
  auto getRectByName(const std::string &name) const noexcept
      -> const TextureRect *;
  void loadFromDirectory(const std::string &prefix, const std::string &filepath,
                         bool sub_folders = false);

 private:
  uint16 min_size_{kTextureAtlasMinSize};
  ivec2 size_{kTextureAtlasDefaultSize, kTextureAtlasDefaultSize};
  std::vector<TextureRect> regions_;
  GLuint handle_{0};
};

}  // namespace app

#endif  // APP_TEXTURES_TEXTUREATLAS_H
