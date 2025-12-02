#ifndef APP_WEBGLSHADER_H
#define APP_WEBGLSHADER_H

#include <string>
#include <utility>

#include "../../core.h"

namespace app {

enum class WebGLShaderType : std::uint16_t {
  kFragment = GL_FRAGMENT_SHADER,
  kVertex = GL_VERTEX_SHADER,
};

class GLShader {
 public:
  GLShader() = default;
  GLShader(WebGLShaderType type, const char *source);
  ~GLShader();
  [[nodiscard]] auto handle() const -> GLuint { return this->handle_; };

  // Copy: not supported
  GLShader(const GLShader &other) = delete;
  auto operator=(const GLShader &other) = delete;
  // Move
  GLShader(GLShader &&other) noexcept
      : handle_(std::exchange(other.handle_, NULL)){};
  auto operator=(GLShader &&other) noexcept -> GLShader & {
    std::swap(handle_, other.handle_);
    return *this;
  }

 private:
  GLuint handle_{0};
};

}  // namespace app

#endif  // APP_WEBGLSHADER_H
