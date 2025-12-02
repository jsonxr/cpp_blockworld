//
// Created by Jason Rowland on 2/3/22.
//

#include "Block.h"

#include "../core/BoxGeometry.h"
#include "../core/BufferGeometry.h"
#include "../core.h"
#include "../core/textures/TextureAtlas.h"
#include "Chunk.h"
#include "BlockMap.h"
#include <string>

namespace app {

namespace {

struct BlockOffset {
  ivec3 add{};
  int offset;
};

constexpr ivec3 kSize{1, 1, 1};
constexpr vec4 kZero{};
constexpr BlockOffset kBlockXp{.add = {+1, 0, 0}, .offset = -1};
constexpr BlockOffset kBlockXn{.add = {-1, 0, 0}, .offset = 1};
constexpr BlockOffset kBlockYp{.add = {0, +1, 0}, .offset = -kChunkHeight};
constexpr BlockOffset kBlockYn{.add = {0, -1, 0}, .offset = kChunkHeight};
constexpr BlockOffset kBlockZp{.add = {0, 0, +1}, .offset = -kChunkWidth};
constexpr BlockOffset kBlockZn{.add = {0, 0, -1}, .offset = kChunkWidth};

auto getTextureForFace(const TextureAtlas &atlas, const std::string &name)
    -> vec4 {
  vec4 uv{};
  const auto *xp = atlas.getRectByName(name);

  uv = xp ? xp->uv : kZero;

  return uv;
}

auto adjacent(const Chunk &chunk, const BlockMap &map,
              const TextureAtlas &atlas, const ivec3 &pos,
              const BlockOffset &adj, const Face &face) -> vec4 {
  //  auto uv = getTextureForFace(atlas, face.texture);
  //  return uv;

  auto adj_loc = pos + adj.add;
  auto id = chunk.get_id_from_location(adj_loc);
  const auto *adj_block = map.get_block(id);
  if (!adj_block || adj_block->is_transparent) {
    auto uv = getTextureForFace(atlas, face.texture);
    return uv;
  }

  return kZero;
}

}  // namespace

void BlockGfx::add(BufferGeometry &geometry, const Chunk &chunk,
                   const BlockMap &map, const TextureAtlas &atlas,
                   const ivec3 &pos, const Block &block, uint16 /*index*/) {
  // cout << pos.x << "," << pos.y << "," << pos.z << " index=" << index <<
  // endl;

  if (block.type != BlockType::kNull) {
    box_geometry::add_cube(
        geometry,
        {
            .p1 = pos,
            .p2 = pos + kSize,
            .xp = adjacent(chunk, map, atlas, pos, kBlockXp, block.xp),
            .xn = adjacent(chunk, map, atlas, pos, kBlockXn, block.xn),
            .yp = adjacent(chunk, map, atlas, pos, kBlockYp, block.yp),
            .yn = adjacent(chunk, map, atlas, pos, kBlockYn, block.yn),
            .zp = adjacent(chunk, map, atlas, pos, kBlockZp, block.zp),
            .zn = adjacent(chunk, map, atlas, pos, kBlockZn, block.zn),
        });
  }
}

}  // namespace app
