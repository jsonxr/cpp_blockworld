/**
 * @file ImportedBlock.h
 * @author Jason Rowland
 * @copyright Copyright (c) 2022
 *
 * https://minecraft.fandom.com/wiki/Model#Block_models
 *
 */
#ifndef APP_WORLD_BLOCK_H
#define APP_WORLD_BLOCK_H

#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

#include "../core.h"

using json = nlohmann::json;

namespace importer {

//----------------------------------------------------------
// BlockDisplay
//----------------------------------------------------------

struct BlockDisplay {
  std::vector<double> rotation;
  std::vector<double> translation;
  std::vector<double> scale;
};
[[maybe_unused]] void to_json(json &j, const BlockDisplay &b);
[[maybe_unused]] void from_json(const json &j, BlockDisplay &b);

//----------------------------------------------------------
// BlockFace
//----------------------------------------------------------

struct BlockFace {
  std::string texture;
  std::string cullface;
  std::vector<double> uv;
  std::optional<double> rotation;
  int tintindex{-1};
};
[[maybe_unused]] void to_json(json &j, const BlockFace &b);
[[maybe_unused]] void from_json(const json &j, BlockFace &b);

//----------------------------------------------------------
// BlockElementRotation
//----------------------------------------------------------

struct BlockElementRotation {
  std::vector<double> origin;
  std::string axis;
  double angle;
  std::optional<bool> rescale;
};
[[maybe_unused]] void to_json(json &j, const BlockElementRotation &b);
[[maybe_unused]] void from_json(const json &j, BlockElementRotation &b);

//----------------------------------------------------------
// BlockElement
//----------------------------------------------------------

struct BlockElement {
  std::vector<double> from;
  std::vector<double> to;
  std::optional<BlockElementRotation> rotation;
  std::optional<bool> shade;
  std::map<std::string, BlockFace> faces;
  std::string comment;
};
[[maybe_unused]] void to_json(json &j, const BlockElement &b);
[[maybe_unused]] void from_json(const json &j, BlockElement &b);

//----------------------------------------------------------
// BlockModel
//----------------------------------------------------------

struct BlockModel {
 public:
  std::string name;
  std::string parent;
  std::map<std::string, std::string> textures;
  std::vector<BlockElement> elements;
  std::optional<bool> ambientocclusion;
  std::string gui_light;
  std::map<std::string, BlockDisplay> display;
};
[[maybe_unused]] void to_json(json &j, const BlockModel &b);
[[maybe_unused]] void from_json(const json &j, BlockModel &b);

//----------------------------------------------------------
// BlockVariant
//----------------------------------------------------------

struct BlockVariant {
  std::string model;
  std::optional<int> x;
  std::optional<int> y;
  std::optional<bool> uvlock;
  std::optional<int> weight;
};
[[maybe_unused]] void to_json(json &j, const BlockVariant &b);
[[maybe_unused]] void from_json(const json &j, BlockVariant &b);

//----------------------------------------------------------
// BlockMultipart
//----------------------------------------------------------
using Dictionary = std::map<std::string, std::string>;
struct BlockMultipart {
  std::vector<Dictionary> when;
  std::vector<BlockVariant> apply;
};
[[maybe_unused]] void to_json(json &j, const BlockMultipart &b);
[[maybe_unused]] void from_json(const json &j, BlockMultipart &b);

//----------------------------------------------------------
// BlockState
//----------------------------------------------------------

struct BlockStates {
  std::string name;
  std::map<std::string, std::vector<BlockVariant>> variants;
  std::vector<BlockMultipart> multipart;
};
[[maybe_unused]] void to_json(json &j, const BlockStates &b);
[[maybe_unused]] void from_json(const json &j, BlockStates &b);

//----------------------------------------------------------
// ImportedBlock
//----------------------------------------------------------

struct ImportedBlock {
  std::map<std::string, BlockModel> models;
  BlockStates states;
};
[[maybe_unused]] void to_json(json &j, const ImportedBlock &b);
[[maybe_unused]] void from_json(const json &j, ImportedBlock &b);

//----------------------------------------------------------
// MinecraftImporter
//----------------------------------------------------------

class MinecraftImporter {
 public:
  void load();
  auto get_block(const std::string &name) -> ImportedBlock;
  auto get_block_states(const std::string &name) -> BlockStates;
  auto get_block_model(const std::string &name) -> BlockModel;

 private:
  std::map<std::string, json> models_;
  std::map<std::string, json> states_;
};

[[maybe_unused]] void debug_block_map(bool reparse = false);
void debug_missing_models();

}  // namespace importer

#endif
