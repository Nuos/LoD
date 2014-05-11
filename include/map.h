/**
 * @file map.hpp
 * @brief Renders the map (toggle it with 'm').
 */
#ifndef MAP_H_
#define MAP_H_

#include "oglwrap_config.h"
#include "oglwrap/shader.h"
#include "oglwrap/uniform.h"

#include "oglwrap/shapes/fullScreenRect.h"
#include "oglwrap/textures/texture_2D.h"

#include "../engine/gameobject.h"

class Map : public engine::GameObject {
	bool open_;
	glm::vec2 terrain_size_;
	oglwrap::Texture2D tex_;
	oglwrap::FullScreenRectangle rect_;

	oglwrap::Program prog_;

public:
	Map(glm::vec2 terrain_size)
			: open_(false)
			, terrain_size_(terrain_size)
	{
		oglwrap::VertexShader vs("map.vert");
		oglwrap::FragmentShader fs("map.frag");
		prog_ << vs << fs;
		prog_.link().use();

		tex_.active(0);
		tex_.bind();
		tex_.loadTexture("textures/map.png");
		tex_.minFilter(oglwrap::MinFilter::Linear);
		tex_.magFilter(oglwrap::MagFilter::Linear);
		tex_.unbind();

		oglwrap::UniformSampler(prog_, "uTex").set(0);

		prog_.validate();

		rect_.setupPositions(prog_ | "aPosition");
		rect_.setupTexCoords(prog_ | "aTexcoord");
	}

	virtual ~Map() {}

	void toggle() {
		open_ = !open_;
	}

	struct MapMark {
		oglwrap::Program prog_;
		oglwrap::Texture2D tex_;
		oglwrap::FullScreenRectangle rect_;
		oglwrap::LazyUniform<glm::mat3> uModelMatrix_;

		MapMark() : uModelMatrix_(prog_, "uModelMatrix") {
			oglwrap::VertexShader vs("mapmark.vert");
			oglwrap::FragmentShader fs("mapmark.frag");
			prog_ << vs << fs;
			prog_.link().use();

			tex_.active(0);
			tex_.bind();
			tex_.loadTexture("textures/map_mark.png");
			tex_.minFilter(oglwrap::MinFilter::Linear);
			tex_.magFilter(oglwrap::MagFilter::Linear);
			tex_.unbind();

			oglwrap::UniformSampler(prog_, "uTex").set(0);

			prog_.validate();

			rect_.setupPositions(prog_ | "aPosition");
			rect_.setupTexCoords(prog_ | "aTexcoord");
		}

		glm::mat3 getModelMatrix(glm::vec2 pos, glm::vec2 forward) {
			float angle = atan2(forward.y, forward.x);
			float x_scale = 0.05 * 126.0 / 190.0;
			float y_scale = 0.05;
			return glm::mat3(
				cos(angle),  sin(angle),  0,
				-sin(angle), cos(angle),  0,
				pos.y,			 -pos.x,			1
			) * glm::mat3(
				x_scale, 0, 0,
				0, y_scale, 0,
				0, 0, 1
			);
		}

		void render(glm::vec2 pos, glm::vec2 forward) {
			prog_.use();
			tex_.active(0);
			tex_.bind();

			uModelMatrix_ = getModelMatrix(pos, forward);
			rect_.render();

			tex_.unbind();
		}

	} mark_;

	glm::vec2 getMarkPos(const engine::Camera& cam) {
		glm::vec3 camTarget = cam.getParent()->pos();
		glm::vec2 cam_target = glm::vec2(camTarget.x, camTarget.z);
		return cam_target / terrain_size_ * 0.6f * 2.0f;
	}

	glm::vec2 getMarkOrientation(const engine::Camera& cam) {
		glm::vec3 camTarget = cam.getParent()->pos();
		return glm::vec2(camTarget.x, camTarget.z);
	}

	virtual void render(float time, const engine::Camera& cam) override {
		using namespace oglwrap;

		if (open_) {
			prog_.use();
			tex_.active(0);
			tex_.bind();

			auto capabilies = Context::TemporarySet({
				{Capability::Blend, true},
				{Capability::CullFace, false},
				{Capability::DepthTest, false}
			});
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			rect_.render();
			tex_.unbind();

			mark_.render(getMarkPos(cam), getMarkOrientation(cam));
		}
	}

};

#endif // MAP_H_