#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "util/frame.h"
#include "util/scad.h"

namespace kb {

Shape GetConnector();

struct Key {
 public:
  Key() {
  }

  Key(double x, double y, double z) {
    f.x = x;
    f.y = y;
    f.z = z;
  }

  Frame f;
  Key* parent = nullptr;

  Frame GetAbsoluteFrame() const;
  Frame GetSwitchFrame() const;
  Shape GetSwitch() const;
  Shape GetCap() const;

  Frame GetTopRight() const;
  Frame GetTopLeft() const;
  Frame GetBottomRight() const;
  Frame GetBottomLeft() const;
};

}  // namespace kb
