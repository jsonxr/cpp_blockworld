#include "Chunk.h"

#include <algorithm>
#include <cstddef>
#include <cmath>
#include <iostream>
#include <memory>
#include <utility>
#include <string>
#include "../core.h"
#include "../core/Camera.h"
#include "../core/BufferGeometry.h"
#include "../core/Material.h"
#include "../core/textures/TextureAtlas.h"
#include "Block.h"
#include "BlockMap.h"
#include "constants.h"
#include "utils/math.h"
#include "vendor/SimplexNoise.h"

namespace app {

namespace {
int count = 0;
}

Chunk::Chunk(vec3 pos) : id_(++count), pos_(pos) {}

auto Chunk::pos() const noexcept -> const vec3 & { return pos_; }
auto Chunk::size() const noexcept -> int { return width_ * width_ * height_; }
auto Chunk::data() const noexcept -> const ChunkData & { return data_; }

auto Chunk::get_id_from_index(int index) const noexcept -> int16 {
  if (index < 0 || static_cast<std::size_t>(index) >= data().size()) {
    return -1;
  }
  return data()[static_cast<std::size_t>(index)];
}

auto Chunk::get_id_from_location(const ivec3 &loc) const noexcept -> int16 {
  auto index = get_index_from_location(loc);
  return get_id_from_index(index);
}

auto Chunk::get_location_from_index(int index) const noexcept -> ivec3 {
  const int y = index / (width_ * width_);
  const int remainder = index % (width_ * width_);
  const int z = remainder / width_;
  const int x = remainder % width_;
  return ivec3{x, y, z};
}

auto Chunk::get_index_from_location(const ivec3 &loc) const noexcept -> int {
  if (loc.x < 0 || loc.y < 0 || loc.z < 0 || loc.x >= width_ ||
      loc.z >= width_ || loc.y >= height_) {
    return -1;
  }
  return (loc.y * (width_ * width_)) + (loc.z * width_) + loc.x;
}

const std::string kGrassBlock = "minecraft:block/grass_block";
const std::string kDirt = "minecraft:block/dirt";
const std::string kAir = "minecraft:block/air";
const std::string kStone = "minecraft:block/stone";
const std::string kBedrock = "minecraft:block/bedrock";
const std::string kBirchWood = "minecraft:block/birch_planks";

// TODO: Generate this based on noise, biome, and coordinates. Make this from
//  javascript
void Chunk::generate(const BlockMap &map) {
  SimplexNoise simplex{};
  simplex.set_seed(kChunkSeed);

  std::cout << "Chunk::generate()" << id_ << '\n';
  data_.resize(size());
  std::ranges::fill(data_, 0);  // Fill with air

  for (int y = 0; y < kChunkHeight; y++) {
    for (int x = 0; x < kChunkWidth; x++) {
      for (int z = 0; z < kChunkWidth; z++) {
        auto i = get_index_from_location(ivec3{x, y, z});
        const auto rando =
            math::mapRange(simplex.fractal(kChunkOctaves,
                                           (pos_.x + static_cast<float>(x)) /
                                               kChunkSmoothing,
                                           (pos_.z + static_cast<float>(z)) /
                                               kChunkSmoothing),
                           -1, 1, 0, 1);  // -1..+1 -> 0..1
        const auto max_height =
            kChunkWaterLine +
            static_cast<int>(std::floor(kChunkHeightVariance * rando));
        if (y == 0) {
          data_[i] = map.get_block_id(kBedrock);
        } else if (y < max_height - 3) {
          data_[i] = map.get_block_id(kStone);
        } else if (y < max_height - 2) {
          data_[i] = map.get_block_id(kDirt);
        } else if (y < max_height - 1) {
          if (x == 0 && z == 0) {
            data_[i] = map.get_block_id(kBirchWood);
          } else {
            data_[i] = map.get_block_id(kGrassBlock);
          }
        } else {
          data_[i] = map.get_block_id(kAir);
        }
      }
    }
  }
  //  for (int i = 0; i < data_.size(); i++) {
  //    auto loc = get_location_from_index(i);
  //    auto max_height = 64 + (5 * SimplexNoise::noise(loc.x, loc.z));
  //
  //    if (loc.y == 0) {
  //      data_[i] = map.get_block_id(kBedrock);
  //    } else if (loc.y == max_height) {
  //      data_[i] = map.get_block_id(kGrassBlock);
  //    } else if (loc.y >= max_height - 1 - kDirtHeight &&
  //               loc.y < max_height - 1) {
  //      data_[i] = map.get_block_id(kDirt);
  //    } else if (loc.y < max_height) {
  //      data_[i] = map.get_block_id(kStone);
  //    } else {
  //      data_[i] = 0;
  //    }
  //  }
  std::cout << "Chunk::generate() - done" << id_ << '\n';
}

// TODO can we remove shared_ptr here?
ChunkGfx::ChunkGfx(std::shared_ptr<Material> material, const Chunk &chunk,
                   const BlockMap &blockMap, const TextureAtlas &atlas)
    : material_(std::move(material)), pos_(chunk.pos()), id_(chunk.id_) {
  BufferGeometry all{
      .isIndexed = false,
      .attributes = vertex_attributes,
  };

  const auto &data = chunk.data();
  all.vertices.reserve(data.size() * 36);
  // all.elements.reserve(data.size() * 36);

  const auto size = data.size();
  for (std::size_t i = 0; i < size; i++) {
    const auto id = chunk.data()[i];
    const auto loc = chunk.get_location_from_index(static_cast<int>(i));
    const auto *block = blockMap.get_block(id);
    if (block) {
      BlockGfx::add(all, chunk, blockMap, atlas, loc, *block,
                    static_cast<uint16>(i));
    }
  }

  buffer_ = std::make_unique<BufferGeometryGfx>(all);
}

void ChunkGfx::render(const Camera &camera) const {
  material_->pos()->set_value(pos_);
  material_->render(camera);
  buffer_->render();
}

}  // namespace app
