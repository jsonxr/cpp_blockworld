#include "World.h"

#include "../core.h"
#include "Chunk.h"

namespace app {

auto World::getBlock(ivec3 loc) -> Block * {
  if (chunks_.empty()) {
    return nullptr;
  }
  const auto chunk_loc =
      ivec3{loc.x * kChunkWidth, loc.y * kChunkHeight, loc.z * kChunkWidth};
  (void)chunk_loc;
  return nullptr;
}

}  // namespace app
