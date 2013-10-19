#ifndef LOD_SHADOW_HPP_
#define LOD_SHADOW_HPP_

#include "oglwrap/oglwrap.hpp"

class Shadow {
    oglwrap::Texture2D_Array tex_;
    std::vector<oglwrap::Framebuffer> fbo_;
    std::vector<glm::mat4> cp_matrices_;

    size_t w_, h_, size_, curr_depth_, max_depth_;
public:

    Shadow(size_t shadowMapSize, size_t depth)
        : fbo_(depth), cp_matrices_(depth), size_(shadowMapSize)
        , curr_depth_(0), max_depth_(depth) {
      using namespace oglwrap;

      // Setup the texture array that will serve as storage.
      tex_.bind();
      tex_.upload(
          PixelDataInternalFormat::DepthComponent,
          size_, size_, depth,
          PixelDataFormat::DepthComponent,
          PixelDataType::Float, nullptr
      );
      tex_.minFilter(MinFilter::Linear);
      tex_.magFilter(MagFilter::Linear);
      tex_.wrapS(Wrap::ClampToEdge);
      tex_.wrapT(Wrap::ClampToEdge);
      tex_.compareFunc(Enums::CompFunc::LEqual);
      tex_.compareMode(CompMode::CompareRefToTexture);

      // Setup the FBOs
      for(int i = 0; i < depth; ++i) {
        fbo_[i].bind();
        fbo_[i].attachTextureLayer(FboAttachment::Depth, tex_, 0, i);
        gl(DrawBuffer(GL_NONE)); // No color output in the bound framebuffer, only depth.
        fbo_[i].validate();
      }

      oglwrap::Framebuffer::Unbind();
    }

    void resize(size_t width, size_t height) {
      w_ = width;
      h_ = height;
    }

    glm::mat4 projMat(float size) const {
      return glm::ortho<float>(-size, size, -size, size, 0, 2*size);
    }

    glm::mat4 camMat(glm::vec3 lightSrcPos, glm::vec4 targetBSphere) const {
      return glm::lookAt(glm::vec3(targetBSphere) + glm::normalize(lightSrcPos) * targetBSphere.w,
                         glm::vec3(targetBSphere), glm::vec3(0, 1, 0));
    }

    glm::mat4 modelCamProjMat(glm::vec3 lightSrcPos, glm::vec4 targetBSphere, glm::mat4 modelMatrix) {
      // [-1, 1] -> [0, 1] convert
      glm::mat4 biasMatrix(
          0.5, 0.0, 0.0, 0.0,
          0.0, 0.5, 0.0, 0.0,
          0.0, 0.0, 0.5, 0.0,
          0.5, 0.5, 0.5, 1.0
      );

      glm::mat4 projMatrix = projMat(targetBSphere.w);
      glm::vec4 offseted_targetBSphere =
        glm::vec4(glm::vec3(modelMatrix * glm::vec4(glm::vec3(targetBSphere), 1)), targetBSphere.w);

      glm::mat4 pc = projMatrix * camMat(lightSrcPos, offseted_targetBSphere);

      cp_matrices_[curr_depth_] = biasMatrix * pc;

      return pc * modelMatrix;
    }

    const std::vector<glm::mat4>& shadowCPs() const {
      return cp_matrices_;
    }

    const oglwrap::Texture2D_Array& shadowTex() const {
      return tex_;
    }

    void begin() {
      fbo_[0].bind();
      curr_depth_ = 0;
      glViewport(0, 0, size_, size_);
      glClear(GL_DEPTH_BUFFER_BIT);
    }

    void push() {
      if(curr_depth_ + 1 < max_depth_) {
        fbo_[++curr_depth_].bind();
        glClear(GL_DEPTH_BUFFER_BIT);
      } else {
        throw std::overflow_error("ShadowMap stack overflow.");
      }
    }

    size_t getDepth() const {
      return curr_depth_;
    }

    size_t getMaxDepth() const {
      return max_depth_;
    }

    void end() {
      oglwrap::Framebuffer::Unbind();
      glViewport(0, 0, w_, h_);
    }
};

#endif // LOD_SHADOW_HPP_