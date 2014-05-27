// Copyright (c) 2014, Tamas Csala

#ifndef ENGINE_HEIGHT_MAP_INTERFACE_H_
#define ENGINE_HEIGHT_MAP_INTERFACE_H_

#include "../lod_oglwrap_config.h"
#include "../oglwrap/textures/texture_2D.h"

namespace engine {

// An interface to get data from a heightmap
class HeightMapInterface {
public:
  virtual ~HeightMapInterface() {}

  // The width and height of the texture
  virtual int w() const = 0;
  virtual int h() const = 0;

  // Returns if the coordinates are valid
  virtual bool valid(double x, double y) const = 0;

  // Simple texture fetch
  virtual double heightAt(int x, int y) const = 0;

  // Texture fetch with interpolation
  virtual double heightAt(double x, double y) const = 0;

  // Returns the format of the height data
  virtual gl::PixelDataFormat format() const = 0;

  // Returns the type of the height data
  virtual gl::PixelDataType type() const = 0;

  // Uploads the heightmap to a texture object
  virtual void upload(gl::Texture2D& tex) const = 0;

  // Returns dvec2{min, max} of area between (x-w/2, y-h/2) and (x+w/2, y+h/2)
  // it returns {0, 0} if the area requested doesn't contain a single valid value
  virtual glm::dvec2 getMinMaxOfArea(int x, int y, int w, int h) const;
};


}


#endif
