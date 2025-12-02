#include "Window.h"

#include <iostream>

#ifndef __EMSCRIPTEN__
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>

namespace app {

//------------------------------------------------------------------------------
// glfw Callbacks
//------------------------------------------------------------------------------

namespace {

void windowsize_callback(GLFWwindow *native_window, int width, int height) {
  auto *window = static_cast<Window *>(glfwGetWindowUserPointer(native_window));
  if (window == nullptr) {
    return;
  }
  window->onResize(width, height);
}

void key_callback(GLFWwindow *native_window, int key, int scancode, int action,
                  int mods) {
  auto *window = static_cast<Window *>(glfwGetWindowUserPointer(native_window));
  if (window == nullptr) {
    return;
  }
  window->input().key_callback(key, scancode, action, mods);
}

void mouse_callback(GLFWwindow *native_window, double xPos, double yPos) {
  auto *window = static_cast<Window *>(glfwGetWindowUserPointer(native_window));
  if (window == nullptr) {
    return;
  }
  window->input().mouse_callback(xPos, yPos);
}

void mouse_button_callback(GLFWwindow *native_window, int button, int action,
                           int mods) {
  auto *window = static_cast<Window *>(glfwGetWindowUserPointer(native_window));
  if (window == nullptr) {
    return;
  }
  window->input().mouse_button_callback(button, action, mods);
}

}  // namespace

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
Window::Window() : Window("OpenGL Window", WindowSize{}, false) {}
Window::Window(const char *title, WindowSize size, bool fullScreenMode) noexcept
    : size_(size) {
  // Highest OpenGL version Mac supports is 4.1
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // Is primaryMonitor leaking memory?
  primary_monitor_ = fullScreenMode ? glfwGetPrimaryMonitor() : nullptr;
  native_window_ = glfwCreateWindow(size.width, size.height, title,
                                    primary_monitor_, nullptr);

  if (native_window_ == nullptr) {
    std::cerr << "Failed to create GLFW window\n";
    return;
  }

  glfwMakeContextCurrent(native_window_);

#ifndef __EMSCRIPTEN__
  auto loader =
      gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
  if (loader == 0) {
    std::cerr << "Failed to initialize GLAD." << '\n';
    return;
  }
#endif

  // Handle window resize
  glfwSetWindowUserPointer(native_window_, this);
  glfwSetWindowSizeCallback(native_window_, windowsize_callback);
  // Input
  glfwSetKeyCallback(native_window_, key_callback);
  glfwSetCursorPosCallback(native_window_, mouse_callback);
  glfwSetMouseButtonCallback(native_window_, mouse_button_callback);

  glfwSetInputMode(native_window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

//------------------------------------------------------------------------------
// Copy/Move/Destruct
//------------------------------------------------------------------------------
Window::~Window() {
  if (native_window_ != nullptr) {
    close();
    native_window_ = nullptr;
  }
  if (primary_monitor_) {
    primary_monitor_ = nullptr;
  }
}

//------------------------------------------------------------------------------
// Events
//------------------------------------------------------------------------------
void Window::onResize(int width, int height) {
  this->size_.width = width;
  this->size_.height = height;
  glViewport(0, 0, width, height);
  const float aspect = static_cast<float>(width) / static_cast<float>(height);
  auto options = camera_.options();
  options.aspect = aspect;
  camera_.set_options(options);
}

//------------------------------------------------------------------------------
// Properties
//------------------------------------------------------------------------------
auto Window::shouldClose() const noexcept -> bool {
  return glfwWindowShouldClose(native_window_) == GLFW_TRUE;
}

//------------------------------------------------------------------------------
// Methods
//------------------------------------------------------------------------------
void Window::close() noexcept {
  glfwSetWindowShouldClose(native_window_, GLFW_TRUE);
  glfwDestroyWindow(native_window_);
}
void Window::process(double deltaTime) {
  input_.process(camera_, deltaTime);
  // camera_.set_position
}

[[maybe_unused]] void Window::setCursorMode(CursorMode cursorMode) {
  int glfw_cursor_mode = GLFW_CURSOR_DISABLED;
  if (cursorMode == CursorMode::kNormal) {
    glfw_cursor_mode = GLFW_CURSOR_NORMAL;
  } else if (cursorMode == CursorMode::kLocked) {
    glfw_cursor_mode = GLFW_CURSOR_DISABLED;
  }
  glfwSetInputMode(native_window_, GLFW_CURSOR, glfw_cursor_mode);
}

void Window::pollEvents() { glfwPollEvents(); }

}  // namespace app
