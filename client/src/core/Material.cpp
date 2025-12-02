#include "Material.h"

#include <iostream>
#include <memory>
#include <utility>

#include "../vendor/glfw.h"
#include "../vendor/glm.h"
#include "Camera.h"
#include "textures/TextureAtlas.h"


namespace app {

Material::Material(std::shared_ptr<TextureAtlas> textures)
    : textures_{std::move(textures)} {
  program_ =
      GLProgram::create("/shaders/vertex.glsl", "/shaders/fragment.glsl");

  // Get locations of uniforms for vertex/fragment shaders
  u_chunk_offset_ = program_.get_uniform("ChunkOffset");
  u_model_ = program_.get_uniform("model");
  u_model_->set_value(mat4{1.0F});
  u_texture0_ = program_.get_uniform("texture0");
  u_view_ = program_.get_uniform("view");
  u_projection_ = program_.get_uniform("projection");

  texture_ = textures_->compile();
}

void Material::render(const Camera &camera) const {
  if (!program_.use()) {
    std::cerr << "WebGLMaterial::use program not available" << '\n';
    return;
  }

  if (texture_ != 0) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_);
    u_texture0_->set_value(0);
  }

  if (u_view_) {
    auto view = camera.view_matrix();
    u_view_->set_value(view);
  }

  if (u_projection_) {
    auto projection = camera.projection_matrix();
    u_projection_->set_value(projection);
  }

  program_.apply_uniforms();
}

}  // namespace app
