#include "Image.h"

#include <filesystem>
#include <utility>

#include "Pixel.h"

namespace app {

Image::Image(std::filesystem::path path)
    : path_{std::move(path)},
      pixels_{
          stbi_load(path_.c_str(), &width_, &height_, &channels_, kChannels)} {}

Image::~Image() {
  channels_ = 0;
  height_ = 0;
  width_ = 0;
  if (pixels_ != nullptr) {
    stbi_image_free(pixels_);
    pixels_ = nullptr;
  }
}

[[maybe_unused]] [[nodiscard]] auto Image::channels() const -> int {
  return channels_;
}

[[maybe_unused]] [[nodiscard]] auto Image::path()
    -> const std::filesystem::path & {
  return path_;
}
[[maybe_unused]] [[nodiscard]] auto Image::height() const -> int {
  return height_;
}
[[maybe_unused]] [[nodiscard]] auto Image::width() const -> int {
  return width_;
}
[[maybe_unused]] [[nodiscard]] auto Image::pixels() const -> unsigned char * {
  return pixels_;
}
[[maybe_unused]] [[nodiscard]] auto Image::get_size() const -> int {
  return width_ * height_ * kChannels;
}

// Copy: not supported
Image::Image(Image &&other) noexcept
    : path_{std::exchange(other.path_, std::filesystem::path{})},
      pixels_{std::exchange(other.pixels_, nullptr)},
      channels_{std::exchange(other.channels_, 0)},
      height_{std::exchange(other.height_, 0)},
      width_{std::exchange(other.width_, 0)} {}

auto Image::operator=(Image &&other) noexcept -> Image & {
  std::swap(path_, other.path_);
  std::swap(pixels_, other.pixels_);
  std::swap(channels_, other.channels_);
  std::swap(height_, other.height_);
  std::swap(width_, other.width_);
  return *this;
}

}  // namespace app
