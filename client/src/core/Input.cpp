#include "Input.h"

#include <glm/common.hpp>
#include <glm/geometric.hpp>

#include <GLFW/glfw3.h>

#include "../vendor/glm.h"
#include "Camera.h"
#include "constants.h"

namespace app {

void Input::key_callback(int key, int /*scancode*/, int action, int /*mods*/) {
  if (key >= 0 && key < GLFW_KEY_LAST) {
    key_pressed_data_.at(key) = action == GLFW_PRESS || action == GLFW_REPEAT;
  }
}

void Input::mouse_callback(double xPos, double yPos) {
  x_pos_ = static_cast<int>(xPos);
  y_pos_ = static_cast<int>(yPos);

  if (firstMouse_) {
    lastMouseX_ = x_pos_;
    lastMouseY_ = y_pos_;
    firstMouse_ = false;
  }

  deltaMouseX_ = x_pos_ - lastMouseX_;
  deltaMouseY_ = lastMouseY_ - y_pos_;
  lastMouseX_ = x_pos_;
  lastMouseY_ = y_pos_;
}

void Input::mouse_button_callback(int button, int action, int /*mods*/) {
  if (button >= 0 && button < GLFW_MOUSE_BUTTON_LAST) {
    mouse_button_pressed_data_.at(button) = action == GLFW_PRESS;
  }
}

void Input::process(Camera &camera, double /*deltaTime*/) {
  const float mx = sensitivity_ * static_cast<float>(deltaMouseX_);
  const float my = sensitivity_ * static_cast<float>(deltaMouseY_);

  auto options = camera.options();

  options.yaw += mx;
  options.pitch += my;

  const vec3 local_right = glm::cross(camera.forward(), glm::vec3(0, 1, 0));
  const vec3 local_up = glm::cross(local_right, camera.forward());

  vec3 xy{0.0, 0.0, 0.0};
  if (Input::isKeyPressed(GLFW_KEY_W)) {
    xy += camera.forward() * kPlayerSpeed;
  }
  if (Input::isKeyPressed(GLFW_KEY_S)) {
    xy -= camera.forward() * kPlayerSpeed;
  }
  if (Input::isKeyPressed(GLFW_KEY_A)) {
    xy -= local_right * kPlayerSpeed;
  }
  if (Input::isKeyPressed(GLFW_KEY_D)) {
    xy += local_right * kPlayerSpeed;
  }

  // Normalize the xz space, not the up/down
  const vec3 n = glm::normalize(xy);
  xy = glm::isnan(n.x) ? xy : n;  // Use the normalized vector if it's not zero
  xy *= kPlayerSpeed;             // Go speed in the normalized direction

  if (Input::isKeyPressed(GLFW_KEY_SPACE)) {
    xy += local_up * kPlayerSpeed;
  }
  if (Input::isKeyPressed(GLFW_KEY_F)) {
    xy -= local_up * kPlayerSpeed;
  }

  options.pos += xy;

  camera.set_options(options);
  deltaMouseX_ = 0;
  deltaMouseY_ = 0;

  //  if (Input::isKeyPressed(GLFW_KEY_W)) {
  //    xy += camera.forward() * kPlayerSpeed;
  //    options.pos += xy;
  //  }
  //  if (Input::isKeyPressed(GLFW_KEY_S)) {
  //    xy -= camera.forward() * kPlayerSpeed;
  //    options.pos += xy;
  //  }
  //  if (Input::isKeyPressed(GLFW_KEY_A)) {
  //    xy -= local_right * kPlayerSpeed;
  //    // camera.set_position(camera.position() - local_right * kPlayerSpeed);
  //  }
  //  if (Input::isKeyPressed(GLFW_KEY_D)) {
  //    xy += local_right * kPlayerSpeed;
  //    // camera.set_position(camera.position() + local_right * kPlayerSpeed);
  //  }
  //  if (Input::isKeyPressed(GLFW_KEY_SPACE)) {
  //    z += local_up * kPlayerSpeed;
  //    // camera.set_position(camera.position() + local_up * kPlayerSpeed);
  //  }
  //  if (Input::isKeyPressed(GLFW_KEY_F)) {
  //    z -= local_up * kPlayerSpeed;
  //    // camera.set_position(camera.position() - local_up * kPlayerSpeed);
  //  }
  //
  //  options.pos = camera.position() + glm::normalize(xy) + glm::normalize(z);
  //  camera.set_options(options);
}

}  // namespace app
