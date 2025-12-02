#ifndef APP_BLOCKMAP_H
#define APP_BLOCKMAP_H

#include <map>

#include "Block.h"
#include "core.h"

namespace app {

class BlockMap {
 public:
  BlockMap();
  void add_block(Block block);
 auto get_block(int16 id) const noexcept -> const Block *;
  auto get_block_id(const std::string &name) const noexcept -> int16;

 private:
  std::vector<Block> blocks_;
  std::map<std::string, int16> by_name_;
};

}  // namespace app

#endif  // APP_BLOCKMAP_H
