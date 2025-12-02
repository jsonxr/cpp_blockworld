//
// Created by Jason Rowland on 2/3/22.
//

#ifndef APP_BLOCK_H
#define APP_BLOCK_H

#include "../core/BufferGeometry.h"
#include "../core/textures/TextureAtlas.h"
#include "Chunk.h"
#include "core.h"

namespace app {

struct Face {
 public:
  std::string texture;
};

enum class BlockType : std::uint8_t {
  kNull,
  kSolid,
  // kTransparent,
};

struct Block {
 public:
  BlockType type{BlockType::kSolid};
  std::string name;
  bool is_transparent{};
  Face xp{};
  Face xn{};
  Face yp{};
  Face yn{};
  Face zp{};
  Face zn{};
};

class BlockGfx {
 public:
  static void add(BufferGeometry &geometry, const Chunk &chunk,
                  const BlockMap &map, const TextureAtlas &atlas,
                  const ivec3 &pos, const Block &block, uint16 index);
};

}  // namespace app

#endif  // APP_BLOCK_H
