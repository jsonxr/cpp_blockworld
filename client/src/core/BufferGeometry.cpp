#include "BufferGeometry.h"

#include <bit>
#include <iostream>
#include <vector>

namespace app {

auto sizeofGlType(GLenum t) -> int;

//------------------------------------------------------------------------------
// BufferGeometry
//------------------------------------------------------------------------------
//
// void BufferGeometry::append(const BufferGeometry &v) {
//  int stride = 0;
//  for (auto a : attributes) {
//    stride += a.count;
//  }
//
//  // size=1, index = 1
//  size_t size = vertices.size() / stride;
//  // Add all the vertices
//
//  // int count = 0;
//  for (auto v : v.vertices) {
//    // std::cout << count << ": v=" << v << std::endl;
//    vertices.emplace_back(v);
//    // count++;
//  }
//
//  // Add all the indexes, but they start with size
//  // count = 0;
//  for (auto index : v.elements) {
//    // std::cout << count << ": index=" << (size + index) << std::endl;
//    elements.emplace_back(size + index);
//    // count++;
//  };
//}

//------------------------------------------------------------------------------
// BufferGeometryGfx
//------------------------------------------------------------------------------
BufferGeometryGfx::BufferGeometryGfx(const BufferGeometry &geometry) {
  std::cout << "BufferGeometryGfx()" << std::endl;
  //
  //  int stride = 0;
  //  for (const auto &a : geometry.attributes) {
  //    stride += a.count * sizeofGlType(a.type);
  //  }
  //  // size = total bytes = stride * vertex (3floats-pos and 2floats-uv))
  //  int size = static_cast<GLint>(geometry.vertices.size() * sizeof(GLfloat));
  int stride = sizeof(Vertex);
  int vertex_bytes = static_cast<int>(geometry.vertices.size() * stride);

  glGenVertexArrays(1, &glVao_);
  glBindVertexArray(glVao_);

  glGenBuffers(1, &glVbo_);
  glBindBuffer(GL_ARRAY_BUFFER, glVbo_);
  glBufferData(GL_ARRAY_BUFFER, vertex_bytes,
               geometry.vertices.data(),  // NOLINT
               GL_STATIC_DRAW);

  // Can we pass in 4 bytes as the rgba color for a vertex coordinate to cut
  // down on size? r,g,b,a = 0-255 (1byte)

  int index = 0;
  long offset = 0;
  for (const auto &a : geometry.attributes) {
    const GLvoid *offset_ptr = (GLvoid *)offset;  // NOLINT
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, a.count, a.type, GL_FALSE, stride, offset_ptr);
    cout << "index=" << index << " a.count=" << a.count << " a.type=" << a.type
         << " stride=" << stride << " offset=" << offset_ptr << endl;
    // Ready the state for the next attribute
    offset += static_cast<long>(sizeofGlType(a.type) * a.count);
    index++;
  }
  count_ = geometry.vertices.size();

  if (geometry.isIndexed && !geometry.elements.empty()) {
    glGenBuffers(1, &glEbo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glEbo_);
    elements_size_ = static_cast<GLsizei>(geometry.elements.size());
    auto elements_bytes =
        static_cast<GLsizeiptr>(sizeof(int) * geometry.elements.size());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements_bytes,
                 geometry.elements.data(), GL_STATIC_DRAW);
  }

  glBindVertexArray(NULL);
}

BufferGeometryGfx::BufferGeometryGfx(BufferGeometryGfx &&other) noexcept
    : glVao_{std::exchange(other.glVao_, 0)},
      glVbo_{std::exchange(other.glVbo_, 0)},
      glEbo_{std::exchange(other.glEbo_, 0)},
      elements_size_{std::exchange(other.elements_size_, 0)},
      count_{std::exchange(other.count_, 0)} {
  cout << "move construct!!" << endl;
}  // move

auto BufferGeometryGfx::operator=(BufferGeometryGfx &&other) noexcept
    -> BufferGeometryGfx & {
  std::swap(glVao_, other.glVao_);
  std::swap(glVbo_, other.glVbo_);
  std::swap(glEbo_, other.glEbo_);
  std::swap(count_, other.count_);
  std::swap(elements_size_, other.elements_size_);
  cout << "move assign!!" << endl;
  return *this;
}

BufferGeometryGfx::~BufferGeometryGfx() noexcept {
  cout << "DESTROy!" << endl;
  if (glVao_ > 0) {
    glDeleteVertexArrays(1, &glVao_);
    glVao_ = 0;
  }
  if (glVbo_ > 0) {
    glDeleteBuffers(1, &glVbo_);
    glVbo_ = 0;
  }
  if (glEbo_ > 0) {
    glDeleteBuffers(1, &glEbo_);
  }
}

void BufferGeometryGfx::render() const {
  if (elements_size_ == 0) {
    glBindVertexArray(glVao_);  // Is this even needed?
    glDrawArrays(GL_TRIANGLES, 0, count_);
  } else {
    glBindVertexArray(glVao_);  // Is this even needed?
    // glBindVertexArray(glVbo_);  // Is this even needed?
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glEbo_);
    glDrawElements(GL_TRIANGLES, elements_size_, GL_UNSIGNED_INT, nullptr);
  }
  glBindVertexArray(0);
}

//------------------------------------------------------------------------------
// Helper Functions
//------------------------------------------------------------------------------

auto sizeofGlType(GLenum t) -> int {
  if (t == GL_BYTE) return sizeof(GLbyte);
  if (t == GL_UNSIGNED_BYTE) return sizeof(GLubyte);
  if (t == GL_SHORT) return sizeof(GLshort);
  if (t == GL_UNSIGNED_SHORT) return sizeof(GLushort);
  if (t == GL_INT) return sizeof(GLint);
  if (t == GL_UNSIGNED_INT) return sizeof(GLuint);
  // if (t == GL_FIXED) return sizeof(GLfixed);
  if (t == GL_HALF_FLOAT) return sizeof(GLhalf);
  if (t == GL_FLOAT) return sizeof(GLfloat);
  if (t == GL_DOUBLE) return sizeof(GLdouble);
  return 0;
}

}  // namespace app
