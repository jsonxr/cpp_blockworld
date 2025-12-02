//
// Created by Jason Rowland on 2/11/22.
//

#ifndef APP_GLTYPES_H
#define APP_GLTYPES_H

#include <cstdint>

#include "vendor/glfw.h"

namespace app {

enum class GLType : std::uint16_t {
  kUnknown = 0,
  kSampler2d = GL_SAMPLER_2D,
  kMat4f = GL_FLOAT_MAT4,
  kVec2f = GL_FLOAT_VEC2,
  kVec3f = GL_FLOAT_VEC3,
  kVec4f = GL_FLOAT_VEC4,
  kInt = GL_INT,
  kFloat = GL_FLOAT
};

static auto GLTypeFromGlEnum(GLenum value) -> GLType {
  switch (value) {
    case GL_SAMPLER_2D: {
      return GLType::kSampler2d;
    }
    case GL_FLOAT_MAT4: {
      return GLType::kMat4f;
    }
    case GL_FLOAT_VEC2: {
      return GLType::kVec2f;
    }
    case GL_FLOAT_VEC3: {
      return GLType::kVec3f;
    }
    case GL_FLOAT_VEC4: {
      return GLType::kVec4f;
    }
    case GL_INT: {
      return GLType::kInt;
    }
    case GL_FLOAT: {
      return GLType::kFloat;
    }
    default: {
      return GLType::kUnknown;
    }
  }
}

}  // namespace app

#endif  // APP_GLTYPES_H
