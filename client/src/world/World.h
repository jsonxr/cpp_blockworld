//
// Created by Jason Rowland on 2/7/22.
//

#ifndef BLOCKWORLD_WORLD_H
#define BLOCKWORLD_WORLD_H

#include "core.h"

namespace app {

// Forward Declarations
class Block;
class Chunk;

class World {
 public:
  auto getBlock(ivec3 loc) -> Block *;

 private:
  std::vector<std::unique_ptr<Chunk>> chunks_;
};

}  // namespace app

#endif  // BLOCKWORLD_WORLD_H
