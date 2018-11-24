#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "util/scad.h"
#include "util/transform.h"

namespace kb {

Shape GetConnector();

struct Key {
 public:
  Key() {
  }

  Key(double x, double y, double z) {
    t.x = x;
    t.y = y;
    t.z = z;
  }

  Transform t;
  Key* parent = nullptr;

  Transforms GetTransforms() const;
  Transforms GetSwitchTransforms() const;

  Shape GetSwitch() const;
  Shape GetCap() const;

  Transforms GetTopRight() const;
  Transforms GetTopLeft() const;
  Transforms GetBottomRight() const;
  Transforms GetBottomLeft() const;
};

}  // namespace kb
