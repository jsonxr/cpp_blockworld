#ifndef APP_WEBGLPROGRAM_H
#define APP_WEBGLPROGRAM_H

#include <array>
#include <map>
#include <string>

#include "IntResource.h"
#include "GLShader.h"
#include "GLUniform.h"
#include "GLAttribute.h"
#include <variant>

namespace app {

// This did not work for emscripten...
//using GLProgramResource = IntResource<GLuint, glDeleteProgram>;

struct GLProgramResource {
 public:
  GLProgramResource(): handle_{glCreateProgram()} {}
  ~GLProgramResource() { if (handle_ > 0) {  glDeleteProgram(handle_); } }
  GLProgramResource(const GLProgramResource &other) = delete;
  auto operator=(const GLProgramResource &other) = delete;
  GLProgramResource(GLProgramResource &&other) noexcept
      : handle_(std::exchange(other.handle_, NULL)){};
  auto operator=(GLProgramResource &&other) noexcept -> GLProgramResource & {
    std::swap(handle_, other.handle_);
    return *this;
  }
  auto get() const -> GLuint { return handle_; };
 private:
  GLuint handle_;
};

class GLProgram {
 public:
  GLProgram() = default;
  GLProgram(const GLShader &vertexShader,
               const GLShader &fragmentShader);
  auto handle() const -> GLuint { return handle_.get(); };
  [[nodiscard]] auto use() const noexcept -> bool;
  void apply_uniforms() const;

  void debug();
 //auto get_uniform(const std::string &name) const -> const GLUniform *;
  auto get_uniform(const std::string &name) -> GLUniform *;
  static auto create (const std::string &vertex, const std::string &fragment) -> GLProgram;
 private:
  GLProgramResource handle_;
  std::map<std::string, GLUniform> var_uniforms_;
  std::map<std::string, GLAttribute> var_attributes_;
};

}  // namespace app

#endif  // APP_WEBGLPROGRAM_H
