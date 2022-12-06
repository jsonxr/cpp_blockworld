#ifndef APP_CORE_H
#define APP_CORE_H

#include <array>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "nonstd/expected.hpp"
#include "vendor/glfw.h"
#include "vendor/glm.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

using namespace nonstd;
using namespace std::literals;

using uint64 = uint64_t;
using uint32 = uint32_t;
using uint16 = uint16_t;
using uint8 = uint8_t;
using uint = uint32_t;

using int64 = int64_t;
using int32 = int32_t;
using int16 = int16_t;
using int8 = int8_t;

constexpr int kMaxLogLength = 1024;

#ifdef __EMSCRIPTEN__
constexpr bool kIsEmScripten = true;
#else
constexpr bool kIsEmScripten = false;
#endif

#endif  // APP_CORE_H
