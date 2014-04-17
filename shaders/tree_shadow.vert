#version 120

attribute vec4 aPosition;
attribute vec2 aTexCoord;

uniform mat4 uMCP;

varying vec2 vTexcoord;

void main() {
  vTexcoord = aTexCoord;
  gl_Position = uMCP * aPosition;
}
