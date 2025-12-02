#ifndef APP_BOXGEOMETRY_H
#define APP_BOXGEOMETRY_H

#include <array>
#include <optional>
#include <utility>

#include "../core.h"
#include "BufferGeometry.h"

/*
                              +y
                               |
                               |
                               F--------E
 A=p1.x,p2.y,p2.z             /        /|
 B=p1.x,p1.y,p2.z            /        / |
 C=p2.x,p1.y,p2.z       (u) A--------D  | <-  p2
 D=p2                       |  |     |  |
 E=p2.x,p2.y,p1.z     p1->  |  G --  |  H- - - - +x
 F=p1.x,p2.y,p1.z           | /      | /
 G=p1                       |        |/
 H=p2.x,p1.y,p1.z           B--------C (v)
                           /
                         +z
*/

namespace app::box_geometry {
struct BoxOptions {
  vec3 p1{0.0, 0.0, 0.0};
  vec3 p2{1.0, 1.0, 1.0};
  vec4 xp{};
  vec4 xn{};
  vec4 yp{};
  vec4 yn{};
  vec4 zp{};
  vec4 zn{};
};

void add_quad(BufferGeometry &geometry, const std::array<vec3, 4> &vertices,
              const vec4 &uv);
void add_cube(BufferGeometry &bufferGeometry, const BoxOptions &options);

}  // namespace app::box_geometry

#endif  // APP_BOXGEOMETRY_H
