#ifndef APP_WORLD_CHUNK_H
#define APP_WORLD_CHUNK_H

#include "../core.h"
#include "../core/BufferGeometry.h"
#include "../core/Material.h"
#include "../core/textures/TextureAtlas.h"

namespace app {

class BlockMap;

constexpr int kChunkWidth = 16;
constexpr int kChunkHeight = 256;

using ChunkData = std::vector<int16>;

class Chunk {
 public:
  Chunk(vec3 pos);
  auto pos() const noexcept -> const vec3 &;
  auto data() const noexcept -> const ChunkData &;
  auto size() const noexcept -> int;
  void generate(const BlockMap &map);
  auto get_id_from_index(int index) const noexcept -> int16;
  auto get_id_from_location(const ivec3 &loc) const noexcept -> int16;
  auto get_location_from_index(int index) const noexcept -> ivec3;
  auto get_index_from_location(const ivec3 &loc) const noexcept -> int;
  int id_{};

 private:
  vec3 pos_{};
  int width_{kChunkWidth};
  int height_{kChunkHeight};
  ChunkData data_;
};

class ChunkGfx {
 public:
  ChunkGfx(std::shared_ptr<Material> material, const Chunk &chunk,
           const BlockMap &blockMap, const TextureAtlas &atlas);
  void render(const Camera &camera) const;
  auto pos() const -> const vec3 & { return pos_; }
  int id_{};

 private:
  vec3 pos_{};
  std::unique_ptr<BufferGeometryGfx> buffer_;
  std::shared_ptr<Material> material_;
};

}  // namespace app

#endif  // APP_WORLD_CHUNK_H
