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

  TransformList local_transforms;
  Key* parent = nullptr;

  Transform& t() {
    return local_transforms.mutable_front();
  }

  TransformList GetTransforms() const;
  TransformList GetSwitchTransforms() const;

  Shape GetSwitch() const;
  Shape GetCap() const;

  TransformList GetTopRight() const;
  TransformList GetTopLeft() const;
  TransformList GetBottomRight() const;
  TransformList GetBottomLeft() const;
};

}  // namespace kb
