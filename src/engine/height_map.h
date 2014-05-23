// Copyright (c) 2014, Tamas Csala

#ifndef ENGINE_BASIC_HEIGHT_MAP_H_
#define ENGINE_BASIC_HEIGHT_MAP_H_

#include "height_map_interface.h"
#include "texture.h"

namespace engine {

template<typename DATA_TYPE>
class HeightMap : public HeightMapInterface {
  Texture<DATA_TYPE, 1> tex_;
public:
  // Loads in a texture from a file
  // Set integer to true, if you want to fetch texture values as integers
  HeightMap(const std::string& file_name,
            const std::string& format_string = "R",
            bool integer = false)
  : tex_(file_name, format_string, integer) {}

  // The width and height of the texture
  virtual int w() const override { return tex_.w(); }
  virtual int h() const override { return tex_.h(); }

  // Returns if the coordinates are valid
  virtual bool valid(double x, double y) const override { return tex_.valid(x, y); }

  virtual double heightAt(int x, int y) const override { return tex_(x, y)[0]; }

  virtual double heightAt(double x, double y) const override {
    /*
     * fx, cy -- cx, cy
     *    |        |
     *    |        |
     * fx, fy -- cx, fy
     */

    double fx = floor(x), cx = ceil(x);
    double fy = floor(y), cy = ceil(y);

    double fh = glm::mix(double(tex_(fx, fy)[0]), double(tex_(cx, fy)[0]), x-fx);
    double ch = glm::mix(double(tex_(fx, cy)[0]), double(tex_(cx, cy)[0]), x-fx);

    return glm::mix(fh, ch, y-fy);
  }

  virtual oglwrap::PixelDataFormat format() const override {
    return tex_.format();
  }

  virtual oglwrap::PixelDataType type() const override {
    return tex_.type();
  }

  virtual void upload(oglwrap::Texture2D& tex) const {
    tex_.upload(tex);
  }
};


}


#endif