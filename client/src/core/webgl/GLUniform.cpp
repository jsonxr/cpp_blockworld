//
// Created by Jason Rowland on 2/11/22.
//

#include "GLUniform.h"

#include <glm/gtc/type_ptr.hpp>

#include "../../vendor/glfw.h"
#include "../../vendor/glm.h"
#include "GLTypes.h"

namespace app {

void GLUniform::apply() {
  // Don't apply the uniform if it is what it is
  if (current_value_ == value_) {
    return;
  }

  // Uniform types
  // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetActiveUniform.xhtml
  switch (type) {
    case GLType::kSampler2d: {
      if (current_value_ != value_) {
      }
      break;
    }
    case GLType::kMat4f: {
      const auto &value = std::get<mat4>(value_);
      glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
      break;
    }
    case GLType::kVec3f: {
      const auto &value = std::get<vec3>(value_);
      glUniform3f(location, value[0], value[1], value[2]);
      break;
    }
    case GLType::kVec4f: {
      const auto &value = std::get<vec4>(value_);
      glUniform4f(location, value[0], value[1], value[2], value[3]);
      break;
    }
    case GLType::kInt: {
      auto value = std::get<int>(value_);
      glUniform1i(location, value);
    }
    case GLType::kFloat:
    default:
      return;
  }

  current_value_ = value_;
}

}  // namespace app
