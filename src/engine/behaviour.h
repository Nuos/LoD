// Copyright (c) 2014, Tamas Csala

#ifndef ENGINE_BEHAVIOUR_H_
#define ENGINE_BEHAVIOUR_H_

#include "./game_object.h"

namespace engine {

class Behaviour : public GameObject {
 public:
  explicit Behaviour(Scene* scene) : GameObject(scene) {}
  virtual ~Behaviour() {}

  virtual void update() {}
  virtual void keyAction(int key, int scancode, int action, int mods) {}
  virtual void charTyped(unsigned codepoint) {}
  virtual void mouseScrolled(double xoffset, double yoffset) {}
  virtual void mouseButtonPressed(int button, int action, int mods) {}
  virtual void mouseMoved(double xpos, double ypos) {}

  virtual void updateAll() override;
  virtual void keyActionAll(int key, int scancode,
                            int action, int mods) override;
  virtual void charTypedAll(unsigned codepoint) override;
  virtual void mouseScrolledAll(double xoffset, double yoffset) override;
  virtual void mouseButtonPressedAll(int button, int action, int mods) override;
  virtual void mouseMovedAll(double xpos, double ypos) override;
};

}  // namespace engine

#endif
