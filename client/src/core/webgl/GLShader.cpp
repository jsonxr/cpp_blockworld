#include "GLShader.h"

#include <array>
#include <iostream>

#include "../../core.h"
#include "../../vendor/glfw.h"

namespace app {

GLShader::~GLShader() {
  std::cout << "~WebGLShader " << handle_ << '\n';
  if (handle_ > 0) {
    glDeleteShader(handle_);
  }
}

GLShader::GLShader(WebGLShaderType type, const char *source)
    : handle_{glCreateShader(static_cast<GLenum>(type))} {
  std::cout << "WebGLShader " << handle_ << '\n';

  glShaderSource(this->handle_, 1, &source, nullptr);
  glCompileShader(this->handle_);

  // Check to see if it succeeded
  GLint is_compiled = GL_FALSE;
  glGetShaderiv(this->handle_, GL_COMPILE_STATUS, &is_compiled);
  if (is_compiled == GL_FALSE) {
    std::array<GLchar, kMaxLogLength> log{};
    glGetShaderInfoLog(handle_, sizeof(log), nullptr, log.data());
    std::cerr << "ERROR::SHADER::"
              << "::COMPILATION_FAILED\n"
              << log.data() << '\n';

    glDeleteShader(handle_);
    handle_ = 0;
  }
}

}  // namespace app
