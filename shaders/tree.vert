#version 120

attribute vec4 aPosition;
attribute vec2 aTexCoord;
attribute vec3 aNormal;

uniform mat4 uModelCameraMatrix, uProjectionMatrix;
uniform mat3 uNormalMatrix;

varying vec3 c_vPos;
varying vec3 w_vNormal;
varying vec2 vTexcoord;

void main() {
  w_vNormal = aNormal * uNormalMatrix;
  vTexcoord = aTexCoord;

  vec4 c_pos = uModelCameraMatrix * aPosition;
  c_vPos = vec3(c_pos);

  gl_Position = uProjectionMatrix * c_pos;
}
