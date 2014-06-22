// Copyright (c) 2014, Tamas Csala

#ifndef LOD_TREE_H_
#define LOD_TREE_H_

#include <vector>

#include "engine/oglwrap_config.h"
#include "engine/game_object.h"
#include "engine/shader_manager.h"
#include "engine/mesh/mesh_renderer.h"
#include "engine/height_map_interface.h"

#include "./charmove.h"
#include "./skybox.h"
#include "./shadow.h"

class Tree : public engine::GameObject {
  static constexpr int kTreeTypeNum = 2;
  engine::MeshRenderer mesh_[kTreeTypeNum];
  engine::ShaderProgram prog_, shadow_prog_;

  gl::LazyUniform<glm::mat4> uProjectionMatrix_, uModelCameraMatrix_;
  gl::LazyUniform<glm::mat3> uNormalMatrix_;
  gl::LazyUniform<glm::mat4> shadow_uMCP_;

  Shadow *shadow_;

  struct TreeInfo {
    int type;
    glm::mat4 mat;
    glm::vec4 bsphere;
    engine::BoundingBox bbox;
  };

  std::vector<TreeInfo> trees_;

 public:
  Tree(engine::ShaderManager *manager, Shadow *shadow,
       const engine::HeightMapInterface& height_map);
  virtual ~Tree() {}
  virtual void shadowRender(const engine::Scene& scene) override;
  virtual void render(const engine::Scene& scene) override;
};

#endif  // LOD_TREE_H_
