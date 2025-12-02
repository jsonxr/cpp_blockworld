#ifndef MINECRAFT_WINDOW_H
#define MINECRAFT_WINDOW_H
#include "../core.h"
#include <cstdint>
#include "Camera.h"
#include "constants.h"
#include "Input.h"

namespace app {

constexpr int kDefaultWindowWidth = 1280;
constexpr int kDefaultWindowHeight = 720;

struct WindowSize {
  int width{kDefaultWindowWidth};
  int height{kDefaultWindowHeight};
};

enum class CursorMode : std::uint8_t { kLocked, kNormal };

// struct Destroy_GLFWwindow {
//   void operator()(GLFWwindow* ptr) { glfwDestroyWindow(ptr); }
// };

// using Smart_GLFWwindow = std::unique_ptr<GLFWwindow, Destroy_GLFWwindow>;

class Window {
 public:
  //----------------------------------------------------------------------------
  // Constructors
  //----------------------------------------------------------------------------
  Window();
  explicit Window(const char *title, WindowSize size,
                  bool fullScreenMode = false) noexcept;
  //----------------------------------------------------------------------------
  // Copy/Move/Destruct
  //----------------------------------------------------------------------------
  Window(const Window &other) = delete;          // copy constructor
  auto operator=(const Window &other) = delete;  // copy assignment
  Window(Window &&other) noexcept
      : size_(other.size_),
        native_window_(std::exchange(other.native_window_, nullptr)),
        primary_monitor_(std::exchange(other.primary_monitor_,
                                       nullptr)){};  // move constructor
  auto operator=(Window &&other) noexcept -> Window & {
    size_ = other.size_;
    std::swap(native_window_, other.native_window_);
    std::swap(primary_monitor_, other.primary_monitor_);
    return *this;
  }
  ~Window();

  //----------------------------------------------------------------------------
  // Properties
  //----------------------------------------------------------------------------
  auto camera() const -> const Camera & { return camera_; }
  auto input() -> Input & { return input_; }
  auto shouldClose() const noexcept -> bool;
  auto nativeWindow() const noexcept -> GLFWwindow * { return native_window_; }

  //----------------------------------------------------------------------------
  // Methods
  //----------------------------------------------------------------------------
  void close() noexcept;
  void onResize(int width, int height);
  void process(double deltaTime);
  [[maybe_unused]] void setCursorMode(CursorMode cursorMode);
  static void pollEvents();

 private:
  GLFWwindow *native_window_ = nullptr;
  GLFWmonitor *primary_monitor_ = nullptr;
  WindowSize size_{};
  Camera camera_{{
      .pos = vec3{0, kPlayerHeight, 0},
      .yaw = 270.0,
      .pitch = -15,
  }};
  Input input_{};
};

}  // namespace app

#endif
