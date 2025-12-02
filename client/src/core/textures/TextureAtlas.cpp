#include "TextureAtlas.h"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "../../core.h"
#include "../../vendor/glfw.h"
#include "../Assets.h"
#include "Image.h"
#include "TextureRect.h"
#include "Pixel.h"

namespace app {

//------------------------------------------------------------------------------
// TexturePacker
//------------------------------------------------------------------------------
// Perhaps, do this more efficiently by using rectangles of available pixels...
// https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Rectangle_difference

namespace {

using Position = ivec2;

void sortTextureRects(std::vector<TextureRect> &rects) {
  // Sort largest to smallest since larger are harder to place
  std::ranges::sort(rects, [](const TextureRect &lhs, const TextureRect &rhs) -> bool {
    if (lhs.height != rhs.height) {
      return lhs.height > rhs.height;
    }
    if (lhs.width != rhs.width) {
      return lhs.width > rhs.width;
    }
    return lhs.path.compare(rhs.path) < 0;
  });
}

auto checkPosition(const TextureAtlas &atlas, const TextureRect &texture,
                   const TextureGrid &used, const Position &pos) -> bool {
  const auto pos_x = static_cast<uint16>(pos.x);
  const auto pos_y = static_cast<uint16>(pos.y);
  const auto out_x_slots = static_cast<uint16>(atlas.width() / atlas.min_size());
  const auto out_y_slots = static_cast<uint16>(atlas.height() / atlas.min_size());

  const auto y_slots = static_cast<uint16>((texture.height / atlas.min_size()) +
                   std::min(1, texture.height % atlas.min_size()));
  const auto x_slots = static_cast<uint16>((texture.width / atlas.min_size()) +
                   std::min(1, texture.width % atlas.min_size()));

  if ((pos_x + x_slots > out_x_slots) || (pos_y + y_slots > out_y_slots)) {
    return false;
  }

  bool open = true;
  for (uint16 y = 0; y < y_slots; y++) {
    for (uint16 x = 0; x < x_slots; x++) {
      const int offset = ((pos_y + y) * out_x_slots) + (pos_x + x);
      open = open && (used.at(offset) == 0);
    }
  }
  return open;
}

void markPosition(const TextureAtlas &atlas, TextureRect &texture,
                  TextureGrid &used, const Position &pos) {
  const auto y_slots = static_cast<uint16>((texture.height / atlas.min_size()) +
                   std::min(1, texture.height % atlas.min_size()));
  const auto x_slots = static_cast<uint16>((texture.width / atlas.min_size()) +
                   std::min(1, texture.width % atlas.min_size()));

  const auto pos_x = static_cast<uint16>(pos.x);
  const auto pos_y = static_cast<uint16>(pos.y);

  const auto out_x_slots = static_cast<uint16>(atlas.width() / atlas.min_size());
  // uint16 out_y_slots = atlas.height() / atlas.min_size();

  for (uint16 y = 0; y < y_slots; y++) {
    for (uint16 x = 0; x < x_slots; x++) {
      const int offset = ((pos_y + y) * out_x_slots) + (pos_x + x);
      used.at(offset) = texture.id;
    }
  }
}

auto findPosition(const TextureAtlas &atlas, const TextureRect &texture,
                  const TextureGrid &used) -> std::optional<Position> {
  const auto output_y_slots = static_cast<uint16>((atlas.height() / atlas.min_size()) +
                          std::min(1, atlas.height() % atlas.min_size()));
  const auto output_x_slots = static_cast<uint16>((atlas.width() / atlas.min_size()) +
                          std::min(1, atlas.width() % atlas.min_size()));

  for (uint16 y = 0; y < output_y_slots; y++) {
    for (uint16 x = 0; x < output_x_slots; x++) {
      Position pos{x, y};
      if (checkPosition(atlas, texture, used, pos)) {
        return pos;
      }
    }
  }

  return std::nullopt;
}

auto setPositionsForRegions(const TextureAtlas &atlas,
                            std::vector<TextureRect> &regions)
    -> std::unique_ptr<TextureGrid> {
  const int slots = (atlas.width() * atlas.height()) / atlas.min_size();
  auto used = std::make_unique<TextureGrid>(slots);
  // TextureGrid used(slots);

  for (auto &r : regions) {
    auto pos = findPosition(atlas, r, *used);
    if (!pos) {
      std::cout << "Could not find room for " << r.path.filename().string()
                << '\n';
      continue;
    }

    r.x = pos->x * atlas.min_size();
    r.y = pos->y * atlas.min_size();
    const float ux = static_cast<float>(r.x) / static_cast<float>(atlas.width());
    const float uy = static_cast<float>(r.y) / static_cast<float>(atlas.height());
    const float vx =
        static_cast<float>(r.x + r.width) / static_cast<float>(atlas.width());
    const float vy =
        static_cast<float>(r.y + r.height) / static_cast<float>(atlas.height());
    r.uv = {ux, uy, vx, vy};
    markPosition(atlas, r, *used, *pos);
  }

  return used;
}

void copyPixels(const TextureAtlas &atlas, const TextureRect &texture,
                TexturePixels &pixels) {
  // Image
  auto image = std::make_unique<Image>(texture.path);

  for (uint16 y = 0; y < texture.height; y++) {
    const int offset = static_cast<int>(y) * texture.width * kChannels;
    const unsigned char *current_raw_pixel = &image->pixels()[offset];

    const int calc_y = (texture.y) + y;
    const int y_offset = calc_y * atlas.width();
    const int x_offset = texture.x;
    const int byte_offset = y_offset + x_offset;

    if (byte_offset < pixels.size()) {
      Pixel &current_pixel = pixels[byte_offset];

      // DANGER... Bug prone memcpy here...
      memcpy(&current_pixel, current_raw_pixel, sizeof(Pixel) * texture.width);
    } else {
      std::cout << "out of range" << '\n';
    }
  }
}

}  // namespace

// fmt::format causes emscripten to fail to link
//
// void outputTextureGrid(TextureAtlas &atlas) {
//   auto used = atlas.pack();
//   std::string display;

//   uint16 output_y_slots =
//       atlas.get_height() / atlas.get_min_size() +
//       std::min(1, atlas.get_height() % atlas.get_min_size());
//   uint16 output_x_slots = atlas.get_width() / atlas.get_min_size() +
//                           std::min(1, atlas.get_width() %
//                           atlas.get_min_size());

//   for (uint16 i = 0; i < output_y_slots; i++) {
//     for (uint16 j = 0; j < output_x_slots; j++) {
//       int offset = (i * output_x_slots) + j;
//       if (used->at(offset) == 0) {
//         display += fmt::format("{:>4} ", "");
//       } else {
//         display += fmt::format("{:>4} ", used->at(offset));
//       };
//     }
//     display += "\n";
//   }

//   std::cout << display << std::endl;
// }

//------------------------------------------------------------------------------
// TextureAtlas
//------------------------------------------------------------------------------

TextureAtlas::TextureAtlas(const ivec2 &size, uint8 min_size)
    : size_{size}, min_size_{min_size} {
  if (!isPowerOf2(size.x) || !isPowerOf2(size.y)) {
    std::cout << "Warning!! not a power of 2" << '\n';
  }
}

[[nodiscard]] auto TextureAtlas::width() const noexcept -> uint16 {
  return size_.x;
}
[[nodiscard]] auto TextureAtlas::height() const noexcept -> uint16 {
  return size_.y;
}
[[nodiscard]] auto TextureAtlas::min_size() const noexcept -> uint16 {
  return min_size_;
}

auto TextureAtlas::add(TextureRect &&region) -> uint16 {
  region.id = regions_.size();
  regions_.emplace_back(region);
  return region.id;
}

auto TextureAtlas::pack() -> std::unique_ptr<TextureGrid> {
  sortTextureRects(regions_);
  auto used = setPositionsForRegions(*this, regions_);
  return used;
}

auto TextureAtlas::generatePixels() -> std::unique_ptr<TexturePixels> {
  auto used = pack();
  const int area = size_.x * size_.y;
  auto pixels = std::make_unique<TexturePixels>(area);

  // Copy the pixels from the source to the dest
  for (auto const &r : regions_) {
    copyPixels(*this, r, *pixels);
  }
  return pixels;
}

auto TextureAtlas::compile() -> GLuint {
  auto pixels = this->generatePixels();
  // glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &handle_);
  glBindTexture(GL_TEXTURE_2D, handle_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size_.x, size_.y, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, pixels->data());
  glBindTexture(GL_TEXTURE_2D, 0);  // reset
  return handle_;
}

void TextureAtlas::save(const std::string &filename) {
  auto pixels = this->generatePixels();
  const int stride_in_bytes = size_.x * static_cast<int>(sizeof(Pixel));
  stbi_write_png(filename.c_str(), size_.x, size_.y, 4, pixels->data(),
                 stride_in_bytes);
}

auto TextureAtlas::getRectByName(const std::string &name) const noexcept
    -> const TextureRect * {
  const auto it = std::ranges::find_if(
      regions_, [&name](const TextureRect &rect) -> bool { return rect.name == name; });
  if (it == regions_.end()) {
    return nullptr;
  }
  return &(*it);
}

// TODO - Implement sub_folder recursion
void TextureAtlas::loadFromDirectory(const std::string &prefix,
                                     const std::string &path,
                                     bool /*sub_folders*/) {
  Assets::forEachFile(
      path, ".+\\.png", [this, &prefix](const std::filesystem::path &filepath) -> void {
        auto image = std::make_unique<app::Image>(filepath);
        const std::string name = prefix + image->path().stem().string();
        app::TextureRect rect{};
        rect.name = name;
        rect.path = image->path();
        rect.width = image->width();
        rect.height = image->height();
        this->add(std::move(rect));
      });
  this->generatePixels();
}

}  // namespace app
