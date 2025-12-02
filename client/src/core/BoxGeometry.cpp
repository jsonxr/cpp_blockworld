#include "BoxGeometry.h"

#include <array>
#include <glm/gtx/normal.hpp>
#include <glm/geometric.hpp>

#include "BufferGeometry.h"
#include "Vertex.h"
#include "../vendor/glm.h"

namespace app::box_geometry {

// float: 4 bytes
// int: 4 bytes
//
// vertex = 5 floats (x,y,z, u.x, u.v) = 20 bytes
// 36vertex = 36 * 20 = 720
// ...720 bytes
//
// Indexed:

// 4verts for each face = 24 verts = 480 bytes
// int elements = 36 = 144 bytes
// ...624 bytes

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

void add_quad(BufferGeometry &geometry, const std::array<vec3, 4> &vertices,
              const vec4 &uv) {
  auto v1 = vertices[0];
  auto v2 = vertices[1];
  auto v3 = vertices[2];
  auto v4 = vertices[3];

  auto normal = glm::normalize(glm::triangleNormal(v1, v2, v3));
  if (geometry.isIndexed) {
    // TODO: Should be number of vertices... count()?
    const int size = static_cast<int>(geometry.vertices.size());
    geometry.vertices.insert(geometry.vertices.end(),
                             {
                                 Vertex{.pos = vec3{v1.x, v1.y, v1.z},
                                        .normal = normal,
                                        .uv = vec2{uv.x, uv.y}},  // a
                                 Vertex{.pos = vec3{v2.x, v2.y, v2.z},
                                        .normal = normal,
                                        .uv = vec2{uv.x, uv.w}},  // b
                                 Vertex{.pos = vec3{v3.x, v3.y, v3.z},
                                        .normal = normal,
                                        .uv = vec2{uv.z, uv.w}},  // c
                                 Vertex{.pos = vec3{v4.x, v4.y, v4.z},
                                        .normal = normal,
                                        .uv = vec2{uv.z, uv.y}},  // d
                             });
    geometry.elements.insert(
        geometry.elements.end(),
        {size + 0, size + 1, size + 2, size + 2, size + 3, size + 0});
  } else {
    geometry.vertices.insert(geometry.vertices.end(),
                             {
                                 Vertex{.pos = vec3{v1.x, v1.y, v1.z},
                                        .normal = normal,
                                        .uv = vec2{uv.x, uv.y}},  // a
                                 Vertex{.pos = vec3{v2.x, v2.y, v2.z},
                                        .normal = normal,
                                        .uv = vec2{uv.x, uv.w}},  // b
                                 Vertex{.pos = vec3{v3.x, v3.y, v3.z},
                                        .normal = normal,
                                        .uv = vec2{uv.z, uv.w}},  // c
                                 Vertex{.pos = vec3{v3.x, v3.y, v3.z},
                                        .normal = normal,
                                        .uv = vec2{uv.z, uv.w}},  // c
                                 Vertex{.pos = vec3{v4.x, v4.y, v4.z},
                                        .normal = normal,
                                        .uv = vec2{uv.z, uv.y}},  // d
                                 Vertex{.pos = vec3{v1.x, v1.y, v1.z},
                                        .normal = normal,
                                        .uv = vec2{uv.x, uv.y}},  // a
                             });
  }
}

namespace {
constexpr vec4 kVec4Zero{};
}  // namespace

void add_cube(BufferGeometry &bufferGeometry, const BoxOptions &options) {
  auto p1 = options.p1;  // vec3{-x_size, -y_size, -z_size};
  auto p2 = options.p2;  // vec3{+x_size, +y_size, +z_size};

  auto a = vec3{p1.x, p2.y, p2.z};
  auto b = vec3{p1.x, p1.y, p2.z};
  auto c = vec3{p2.x, p1.y, p2.z};
  auto d = p2;
  auto e = vec3{p2.x, p2.y, p1.z};
  auto f = vec3{p1.x, p2.y, p1.z};
  auto g = p1;
  auto h = vec3{p2.x, p1.y, p1.z};

  if (options.xp != kVec4Zero) {
    add_quad(bufferGeometry, {d, c, h, e}, options.xp);  // right
  }
  if (options.xn != kVec4Zero) {
    add_quad(bufferGeometry, {f, g, b, a}, options.xn);  // left
  }
  if (options.yp != kVec4Zero) {
    add_quad(bufferGeometry, {f, a, d, e}, options.yp);  // top
  }
  if (options.yn != kVec4Zero) {
    add_quad(bufferGeometry, {b, g, h, c}, options.yn);  // bottom
  }
  if (options.zp != kVec4Zero) {
    add_quad(bufferGeometry, {a, b, c, d}, options.zp);  // front
  }
  if (options.zn != kVec4Zero) {
    add_quad(bufferGeometry, {e, h, g, f}, options.zn);  // back
  }
}

}  // namespace app::box_geometry
