#ifndef APP_BUFFER_GEOMETRY_H
#define APP_BUFFER_GEOMETRY_H

#include <array>
#include <vector>

#include "../core.h"
#include "Vertex.h"

namespace app {

struct BufferAttribute {
  GLenum type{GL_FLOAT};
  short count{};
};

static std::vector<BufferAttribute> vertex_attributes = {{
    {.type = GL_FLOAT, .count = 3},  // position
    {.type = GL_FLOAT, .count = 3},  // normal
    {.type = GL_FLOAT, .count = 2}   // uv
}};

// TODO Decouple Vertex from BufferGeometry
//  there is a flaw here in that we have a flexible buffer but a Vertex with
//  very fixed position and uv, would like to make the BufferGeometry flexible
//  with just bytes and attributes and agnostic to what the vertex is
struct BufferGeometry {
  bool isIndexed{};
  std::vector<BufferAttribute> attributes;
  std::vector<Vertex> vertices;
  std::vector<int> elements;
};

class BufferGeometryGfx {
 public:
  explicit BufferGeometryGfx(const BufferGeometry &geometry);
  ~BufferGeometryGfx() noexcept;
  BufferGeometryGfx(const BufferGeometryGfx &other) = delete;
  auto operator=(const BufferGeometryGfx &other) = delete;
  BufferGeometryGfx(BufferGeometryGfx &&other) noexcept;
  auto operator=(BufferGeometryGfx &&other) noexcept -> BufferGeometryGfx &;

  void render() const;

  // private:
  GLuint glVao_{0};
  GLuint glVbo_{0};
  GLuint glEbo_{0};
  int count_{0};
  GLsizei elements_size_{0};
};

}  // namespace app

#endif  // APP_BUFFER_GEOMETRY_H
