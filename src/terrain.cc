// Copyright (c) 2014, Tamas Csala

#include "./terrain.h"
#include <string>

#include "engine/scene.h"

const engine::Transform& Terrain::initTransform() {
  //  transform.local_scale() = glm::vec3(4, 1, 4); TODO
  //  transform.local_pos() = glm::vec3(-2048, 0, -2048); // TODO
  return transform;
}

Terrain::Terrain(engine::Scene* scene)
    : engine::GameObject(scene)
    , height_map_("terrain/output.png", initTransform())
    , mesh_(scene->shader_manager(), height_map_)
    , prog_(scene->shader_manager()->get("terrain.vert"),
            scene->shader_manager()->get("terrain.frag"))
    , uProjectionMatrix_(prog_, "uProjectionMatrix")
    , uCameraMatrix_(prog_, "uCameraMatrix")
    , uModelMatrix_(prog_, "uModelMatrix")
    , uShadowCP_(prog_, "uShadowCP")
    , uNumUsedShadowMaps_(prog_, "uNumUsedShadowMaps")
    , uShadowAtlasSize_(prog_, "uShadowAtlasSize") {
  prog_.use();
  mesh_.setup(prog_, 1);
  gl::UniformSampler(prog_, "uGrassMap0").set(2);
  gl::UniformSampler(prog_, "uGrassMap1").set(3);
  for (int i = 0; i < 2; ++i) {
    grassMaps_[i].bind();
    // no alpha channel here
    grassMaps_[i].loadTexture(
      i == 0 ? "textures/grass.jpg" : "textures/grass_2.jpg", "CSRGB");
    grassMaps_[i].generateMipmap();
    grassMaps_[i].maxAnisotropy();
    grassMaps_[i].minFilter(gl::kLinearMipmapLinear);
    grassMaps_[i].magFilter(gl::kLinear);
    grassMaps_[i].wrapS(gl::kRepeat);
    grassMaps_[i].wrapT(gl::kRepeat);
  }

  gl::UniformSampler(prog_, "uGrassNormalMap").set(4);
  grassNormalMap_.bind();
  {
    // the normal map doesn't have an alpha channel, and is not is srgb space
    grassNormalMap_.loadTexture("textures/grass_normal.jpg", "CRGB");
    grassNormalMap_.generateMipmap();
    grassNormalMap_.minFilter(gl::kLinearMipmapLinear);
    grassNormalMap_.magFilter(gl::kLinear);
    grassNormalMap_.wrapS(gl::kRepeat);
    grassNormalMap_.wrapT(gl::kRepeat);
  }

  gl::UniformSampler(prog_, "uShadowMap").set(5);

  prog_.validate();
}

void Terrain::render() {
  const engine::Camera& cam = *scene_->camera();
  const Shadow *shadow = scene_->shadow();

  prog_.use();
  prog_.update();
  uCameraMatrix_ = cam.matrix();
  uProjectionMatrix_ = cam.projectionMatrix();
  uModelMatrix_ = transform.matrix();
  if (shadow) {
    for (size_t i = 0; i < shadow->getDepth(); ++i) {
      uShadowCP_[i] = shadow->shadowCPs()[i];
    }
    uNumUsedShadowMaps_ = shadow->getDepth();
    uShadowAtlasSize_ = shadow->getAtlasDimensions();
  }

  grassMaps_[0].bind(2);
  grassMaps_[1].bind(3);
  grassNormalMap_.bind(4);
  if (shadow) { shadow->shadowTex().bind(5); }

  mesh_.render(cam);

  if (shadow) { shadow->shadowTex().unbind(5); }
  grassNormalMap_.unbind(4);
  grassMaps_[1].unbind(3);
  grassMaps_[0].unbind(2);
}


