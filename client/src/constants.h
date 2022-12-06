#ifndef APP_CONSTANTS_H
#define APP_CONSTANTS_H

namespace app {

constexpr int kChunkRadius = 0;
constexpr int kChunkHeightVariance = 20;
constexpr int kChunkWaterLine = 64;
constexpr size_t kChunkOctaves = 7;
constexpr float kChunkSmoothing = 50.0f;
constexpr int kChunkSeed = 1;

constexpr float kPlayerHeight = 75.6;
constexpr float kPlayerSpeed = 0.8f;

}  // namespace app
#endif  // APP_CONSTANTS_H
