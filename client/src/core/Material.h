#ifndef APP_MATERIAL_H
#define APP_MATERIAL_H

#include "Camera.h"
#include "textures/TextureAtlas.h"
#include "webgl/GLProgram.h"

namespace app {

class Material {
 public:
  explicit Material(std::shared_ptr<TextureAtlas> textures);
  void render(const Camera &camera) const;

  auto pos() -> GLUniform * { return u_chunk_offset_; }
  //auto model() -> GLUniform * {return location_model_;}

 private:
  GLProgram program_;
  GLuint texture_{0};
  std::shared_ptr<TextureAtlas> textures_;
  GLUniform *u_model_{};
  GLUniform *u_texture0_{};
  GLUniform *u_view_{};
  GLUniform *u_projection_{};
  GLUniform *u_chunk_offset_{};
};

}  // namespace app

#endif  // APP_MATERIAL_H
