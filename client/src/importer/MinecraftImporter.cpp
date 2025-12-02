#include "MinecraftImporter.h"

#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include "../core/Assets.h"

using Assets = app::Assets;

namespace importer {

// https://github.com/nlohmann/json/issues/2046
// json{} vs json(json::value_t::object);

//------------------------------------------------------------------------------
// BlockVariant
//------------------------------------------------------------------------------

[[maybe_unused]] void to_json(json &j, const BlockVariant &b) {
  j = json(json::value_t::object);
  j["model"] = b.model;
  if (b.x) {
    j["x"] = *b.x;
  }
  if (b.y) {
    j["y"] = *b.y;
  }
  if (b.uvlock) {
    j["uvlock"] = *b.uvlock;
  }
  if (b.weight) {
    j["weight"] = *b.weight;
  }
}
[[maybe_unused]] void from_json(const json &j, BlockVariant &b) {
  j.at("model").get_to(b.model);
  if (j.contains("x")) {
    b.x = j["x"].get<int>();
  }
  if (j.contains("y")) {
    b.y = j["y"].get<int>();
  }
  if (j.contains("uvlock")) {
    b.uvlock = j["uvlock"].get<bool>();
  }
  if (j.contains("weight")) {
    b.weight = j["weight"].get<int>();
  }
}

//------------------------------------------------------------------------------
// BlockMultipart
//------------------------------------------------------------------------------

[[maybe_unused]] [[maybe_unused]] void to_json(json &j,
                                               const BlockMultipart &b) {
  j = json(json::value_t::object);
  if (!b.when.empty()) {
    if (b.when.size() == 1) {
      j["when"] = b.when.at(0);
    } else {
      j["when"]["OR"] = b.when;
    }
  }
  if (!b.apply.empty()) {
    if (b.apply.size() == 1) {
      j["apply"] = b.apply.at(0);
    } else {
      j["apply"] = b.apply;
    }
  }
}
[[maybe_unused]] void from_json(const json &j, BlockMultipart &b) {
  if (j.contains("when")) {
    const auto &value = j["when"];
    if (value.contains("OR")) {
      b.when = value["OR"].get<std::vector<Dictionary>>();
    } else {
      b.when = std::vector<Dictionary>{value.get<Dictionary>()};
    }
  }
  if (j.contains("apply")) {
    const auto &value = j["apply"];
    if (!value.is_array()) {
      b.apply = std::vector<BlockVariant>{value.get<BlockVariant>()};
    } else {
      b.apply = value.get<std::vector<BlockVariant>>();
    }
  } else {
    std::cout << j << '\n';
  }
}

//------------------------------------------------------------------------------
// BlockState
//------------------------------------------------------------------------------

// struct BlockStates {
//   std::string name;
//   std::map<std::string, std::vector<BlockVariant>> variants;
// };
[[maybe_unused]] void to_json(json &j, const BlockStates &b) {
  j = json(json::value_t::object);
  if (!b.variants.empty()) {
    for (const auto &obj : b.variants) {
      auto key = obj.first;
      auto list = obj.second;
      if (list.size() == 1) {
        j["variants"][key] = list.at(0);
      } else {
        j["variants"][key] = list;
      }
    }
  }
  if (!b.multipart.empty()) {
    j["multipart"] = b.multipart;
  }
}
[[maybe_unused]] void from_json(const json &j, BlockStates &b) {
  if (j.contains("variants")) {
    const auto variants = j.at("variants").get<json::object_t>();
    for (const auto &obj : variants) {
      const auto &key = obj.first;
      const auto &value = obj.second;
      if (!value.is_array()) {
        b.variants[key] = std::vector<BlockVariant>{value.get<BlockVariant>()};
      } else {
        b.variants[key] = value.get<std::vector<BlockVariant>>();
      }
    }
  }
  if (j.contains("multipart")) {
    j.at("multipart").get_to(b.multipart);
  }
}

//------------------------------------------------------------------------------
// BlockDisplay
//------------------------------------------------------------------------------

[[maybe_unused]] void to_json(json &j, const BlockDisplay &b) {
  j = json(json::value_t::object);
  if (!b.rotation.empty()) {
    j["rotation"] = b.rotation;
  }
  if (!b.translation.empty()) {
    j["translation"] = b.translation;
  }
  if (!b.scale.empty()) {
    j["scale"] = b.scale;
  }
}
[[maybe_unused]] void from_json(const json &j, BlockDisplay &b) {
  if (j.contains("rotation")) {
    j.at("rotation").get_to(b.rotation);
  }
  if (j.contains("translation")) {
    j.at("translation").get_to(b.translation);
  }
  if (j.contains("scale")) {
    j.at("scale").get_to(b.scale);
  }
}

//------------------------------------------------------------------------------
// BlockFace
//------------------------------------------------------------------------------

[[maybe_unused]] void to_json(json &j, const BlockFace &b) {
  j = json(json::value_t::object);
  if (!b.texture.empty()) {
    j["texture"] = b.texture;
  }
  if (!b.cullface.empty()) {
    j["cullface"] = b.cullface;
  }
  if (!b.uv.empty()) {
    j["uv"] = b.uv;
  }
  if (b.rotation) {
    j["rotation"] = *b.rotation;
  }
  if (b.tintindex != -1) {
    j["tintindex"] = b.tintindex;
  }
}
[[maybe_unused]] void from_json(const json &j, BlockFace &b) {
  if (j.contains("texture")) {
    j.at("texture").get_to(b.texture);
  }
  if (j.contains("cullface")) {
    j.at("cullface").get_to(b.cullface);
  }
  if (j.contains("uv")) {
    j.at("uv").get_to(b.uv);
  }
  if (j.contains("rotation")) {
    b.rotation = j["rotation"].get<double>();
  }
  if (j.contains("tintindex")) {
    j.at("tintindex").get_to(b.tintindex);
  }
}

//------------------------------------------------------------------------------
// BlockElementRotation
//------------------------------------------------------------------------------

[[maybe_unused]] void to_json(json &j, const BlockElementRotation &b) {
  j = json(json::value_t::object);
  if (!b.origin.empty()) {
    j["origin"] = b.origin;
  }
  if (!b.axis.empty()) {
    j["axis"] = b.axis;
  }
  j["angle"] = b.angle;
  if (b.rescale) {
    j["rescale"] = *b.rescale;
  }
}
[[maybe_unused]] void from_json(const json &j, BlockElementRotation &b) {
  if (j.contains("origin")) {
    j.at("origin").get_to(b.origin);
  }
  if (j.contains("axis")) {
    j.at("axis").get_to(b.axis);
  }
  if (j.contains("angle")) {
    j.at("angle").get_to(b.angle);
  }
  if (j.contains("rescale")) {
    b.rescale = j["rescale"].get<bool>();
  }
}

//------------------------------------------------------------------------------
// BlockElement
//------------------------------------------------------------------------------

[[maybe_unused]] void to_json(json &j, const BlockElement &b) {
  j = json(json::value_t::object);
  if (!b.from.empty()) {
    j["from"] = b.from;
  }
  if (!b.to.empty()) {
    j["to"] = b.to;
  }
  if (b.rotation) {
    j["rotation"] = *b.rotation;
  }
  if (b.shade) {
    j["shade"] = *b.shade;
  }
  if (!b.faces.empty()) {
    j["faces"] = b.faces;
  }
  if (!b.comment.empty()) {
    j["__comment"] = b.comment;
  }
}
[[maybe_unused]] void from_json(const json &j, BlockElement &b) {
  if (j.contains("from")) {
    j.at("from").get_to(b.from);
  }
  if (j.contains("to")) {
    j.at("to").get_to(b.to);
  }
  if (j.contains("rotation")) {
    b.rotation = j["rotation"].get<BlockElementRotation>();
  }
  if (j.contains("shade")) {
    b.shade = j["shade"].get<bool>();
  }
  if (j.contains("faces")) {
    j.at("faces").get_to(b.faces);
  }
  if (j.contains("__comment")) {
    j.at("__comment").get_to(b.comment);
  }
}

//------------------------------------------------------------------------------
// BlockModel
//------------------------------------------------------------------------------

[[maybe_unused]] void to_json(json &j, const BlockModel &b) {
  j = json(json::value_t::object);
  if (!b.parent.empty()) {
    j["parent"] = b.parent;
  }
  if (!b.elements.empty()) {
    j["elements"] = b.elements;
  }
  if (!b.textures.empty()) {
    j["textures"] = b.textures;
  }
  if (b.ambientocclusion) {
    j["ambientocclusion"] = *b.ambientocclusion;
  }
  if (!b.gui_light.empty()) {
    j["gui_light"] = b.gui_light;
  }
  if (!b.display.empty()) {
    j["display"] = b.display;
  }
}

[[maybe_unused]] void from_json(const json &j, BlockModel &b) {
  if (j.contains("parent")) {
    j.at("parent").get_to(b.parent);
  }
  if (j.contains("elements")) {
    j.at("elements").get_to(b.elements);
  }
  if (j.contains("textures")) {
    j.at("textures").get_to(b.textures);
  }
  if (j.contains("ambientocclusion")) {
    b.ambientocclusion = j["ambientocclusion"].get<bool>();
  }
  if (j.contains("gui_light")) {
    j.at("gui_light").get_to(b.gui_light);
  }
  if (j.contains("display")) {
    j.at("display").get_to(b.display);
  }
}

//------------------------------------------------------------------------------
// ImportedBlock
//------------------------------------------------------------------------------

[[maybe_unused]] void to_json(json &j, const ImportedBlock &b) {
  j["states"] = b.states;
  j["models"] = b.models;
}
[[maybe_unused]] void from_json(const json &j, ImportedBlock &b) {
  j.at("states").get_to(b.states);
  j.at("models").get_to(b.models);
}

//------------------------------------------------------------------------------
// Private Functions
//------------------------------------------------------------------------------

namespace {

auto read_json(const std::string &path, const std::string &prefix)
    -> std::map<std::string, json> {
  std::map<std::string, json> map{};
  Assets::forEachFile(
      path, ".+\\.json",
      [&map, &path, &prefix](const std::filesystem::path &filepath) -> void {
        const std::string asset_name =
            path + std::string{"/"} + filepath.filename().string();
        const std::string str = Assets::loadString(asset_name);
        const auto src = json::parse(str);
        const auto name = prefix + filepath.stem().string();  // minecraft:block/dirt
        map.emplace(name, src);
      });
  return map;
}

}  // namespace

void MinecraftImporter::load() {
  states_ = read_json("/minecraft/blockstates", "minecraft:block/");
  models_ = read_json("/minecraft/models/block", "minecraft:block/");
}

auto MinecraftImporter::get_block_states(const std::string &name)
    -> BlockStates {
  auto j = json(json::value_t::object);

  auto search = states_.find(name);
  if (search != states_.end()) {
    j = search->second;
  }

  BlockStates block_state = j;
  return block_state;
}

auto MinecraftImporter::get_block_model(const std::string &name) -> BlockModel {
  auto j = json(json::value_t::object);

  auto search = models_.find(name);

  while (search != models_.end()) {
    auto obj = search->second;
    j.merge_patch(obj);

    if (obj.contains("parent")) {
      auto parent = obj["parent"].get<std::string>();
      if (!parent.starts_with("minecraft:")) {
        parent.insert(0, "minecraft:");
      }

      search = models_.find(parent);
    } else {
      search = models_.end();
    }
  }

  BlockModel block = j;
  return block;
}

auto MinecraftImporter::get_block(const std::string &name) -> ImportedBlock {
  ImportedBlock block{};
  auto states = get_block_states(name);
  if (states.variants.begin() == states.variants.end()) {
    return block;
  }

  auto models = std::map<std::string, BlockModel>();

  for (const auto &variants_pair : states.variants) {
    const auto list = variants_pair.second;
    for (const auto &variant : list) {
      const auto model_name = variant.model;
      if (!models.contains(model_name)) {
        const BlockModel model = get_block_model(model_name);
        models.emplace(model_name, model);
      }
    }
  }

  block.states = std::move(states);
  block.models = std::move(models);

  const json j2 = block;
  std::cout << j2 << '\n';
  return block;
}

//------------------------------------------------------------------------------
// Debug functions
//------------------------------------------------------------------------------

void debug_missing_models() {
  MinecraftImporter blocks{};
  blocks.load();

  auto states = read_json("/minecraft/blockstates", "minecraft:block/");
  auto models = read_json("/minecraft/models/block", "minecraft:block/");

  // Show block states with a non-matching model
  std::map<std::string, std::string> referenced_models{};
  for (const auto &s : states) {
    std::cout << s.first << '\n';

    const BlockStates blockstate = s.second;
    for (const auto &v : blockstate.variants) {
      for (const auto &variant : v.second) {
        auto name = variant.model;
        if (!name.empty()) {
          if (!name.starts_with("minecraft:")) {
            name.insert(0, "minecraft:");
          }
          referenced_models.emplace(name, s.first);
          std::cout << "      variant=" << name << '\n';
        }
      }
    }
    for (const auto &m : blockstate.multipart) {
      for (const auto &variant : m.apply) {
        auto name = variant.model;
        if (!name.empty()) {
          if (!name.starts_with("minecraft:")) {
            name.insert(0, "minecraft:");
          }
          referenced_models.emplace(name, s.first);
          std::cout << "    multipart=" << name << '\n';
        }
      }
    }
  }

  for (const auto &s : referenced_models) {
    const auto model = blocks.get_block_model(s.first);
    if (model.elements.empty()) {
      std::cout << s.first << "  <--  " << s.second << '\n';
    }
  }
}

/**
 * @brief This will load all the block states in a folder and warn if we are
 * missing attributes
 *
 * @return std::unique_ptr<std::map<std::string, BlockStates>>
 */
namespace {

auto debug_read_block_states(bool reparse = false)
    -> std::unique_ptr<std::map<std::string, BlockStates>> {
  auto map = std::make_unique<std::map<std::string, BlockStates>>();
  Assets::forEachFile(
      "/minecraft/blockstates", ".+\\.json",
      [&map, reparse](const std::filesystem::path &filepath) -> void {
        const std::string asset_name = std::string{"/minecraft/blockstates/"} +
                                       filepath.filename().string();
        const std::string str = Assets::loadString(asset_name);
        const auto src = json::parse(str);
        auto obj = src.get<BlockStates>();

        obj.name = std::string{"minecraft:block/"} + filepath.stem().string();
        map->emplace(obj.name, obj);

        if (reparse) {
          const json j2 = obj;
          const auto patch = json::diff(j2, src);
          if (!patch.empty()) {
            std::cout << "Warning: " << filepath.string() << '\n';
          }
        }
      });
  return map;
}

/**
 * @brief This will load all the block models in a folder and warn if we are
 * missing attributes
 *
 * @return std::unique_ptr<std::map<std::string, BlockModel>>
 */
auto debug_read_block_models(bool reparse = false)
    -> std::unique_ptr<std::map<std::string, BlockModel>> {
  auto map = std::make_unique<std::map<std::string, BlockModel>>();
  Assets::forEachFile(
      "/minecraft/models/block", ".+\\.json",
      [&map, reparse](const std::filesystem::path &filepath) -> void {
        const std::string asset_name = std::string{"/minecraft/models/block/"} +
                                       filepath.filename().string();
        const std::string str = Assets::loadString(asset_name);
        const auto src = json::parse(str);
        auto obj = src.get<BlockModel>();
        obj.name = std::string{"minecraft:block/"} + filepath.stem().string();
        map->emplace(obj.name, obj);

        if (reparse) {
          const json j2 = obj;
          const auto patch = json::diff(j2, src);
          if (!patch.empty()) {
            std::cout << "Warning: " << filepath.string() << '\n';
          }
        }
      });
  return map;
}

[[maybe_unused]] void debug_block_map(bool reparse) {
  debug_read_block_states(reparse);
  debug_read_block_models(reparse);
  debug_missing_models();
}

}  // namespace

}  // namespace importer
