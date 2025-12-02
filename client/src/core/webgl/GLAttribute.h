#ifndef APP_WEBGL_GLATTRIBUTE_H
#define APP_WEBGL_GLATTRIBUTE_H

#include <array>
#include <map>
#include <string>
#include <variant>

#include "GLShader.h"
#include "GLTypes.h"
#include "GLUniform.h"
#include "IntResource.h"

namespace app {

struct GLAttribute {
  int location;
  std::string name;
  GLType type;
  GLint size;
};

}  // namespace app

#endif  // APP_WEBGL_GLATTRIBUTE_H
