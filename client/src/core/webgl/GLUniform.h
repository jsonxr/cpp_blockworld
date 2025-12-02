#ifndef APP_CORE_WEBGL_H_GLUNIFORM_H
#define APP_CORE_WEBGL_H_GLUNIFORM_H

#include <string>
#include <variant>
#include "core.h"
#include "GLTypes.h"

namespace app {

using GlValue = std::variant<mat4, vec4, vec3, int>;
struct GLUniform {
  int location;
  std::string name;
  GLType type;
  GLint size;

  GlValue value_;
  GlValue current_value_;

  void apply();

  void set_value(const mat4 &value) { value_ = value; }
  void set_value(const vec3 &value) { value_ = value; }
  // void set_value(const vec4 &value) { value_ = value; }
  void set_value(const int value) { value_ = value; }
};

}

#endif  // APP_CORE_WEBGL_H_GLUNIFORM_H
