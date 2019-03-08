#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "util/scad.h"
#include "util/transform.h"

namespace kb {

// All sizes in mm.
const double kSwitchWidth = 14.4;
const double kSwitchThickness = 4;
const double kWallWidth = 2;

const double kDsaHeight = 8;
const double kDsaTopSize = 13.2;     // 0.5 * kMmPerInch;
const double kDsaBottomSize = 18.4;  // 0.725 * kMmPerInch;
const double kDsaHalfSize = 16.2;

const double kSwitchOffset = kSwitchWidth / 2 + kWallWidth;

Shape GetCapsuleConnector();
Shape GetPostConnector();

struct Key {
 public:
  Key() {
  }

  Key(double x, double y, double z) {
    t().x = x;
    t().y = y;
    t().z = z;
  }

  TransformList local_transforms;
  Key* parent = nullptr;

  double extra_height_top = 0;
  double extra_height_bottom = 0;
  double extra_width_left = 0;
  double extra_width_right = 0;
  double extra_z = 0;

  bool add_side_nub = true;

  Transform& t() {
    return local_transforms.mutable_front();
  }

  Transform& AddTransform() {
    return local_transforms.AddTransformFront();
  }

  TransformList GetTransforms() const;
  TransformList GetSwitchTransforms() const;

  Shape GetSwitch() const;
  Shape GetCap() const;

  TransformList GetTopRight() const;
  TransformList GetTopLeft() const;
  TransformList GetBottomRight() const;
  TransformList GetBottomLeft() const;

 private:
  TransformList GetTopRightInternal() const;
  TransformList GetTopLeftInternal() const;
  TransformList GetBottomRightInternal() const;
  TransformList GetBottomLeftInternal() const;
};

Shape ConnectVertical(const Key& top, const Key& bottom, Shape connector = GetPostConnector());
Shape ConnectHorizontal(const Key& left, const Key& right, Shape connector = GetPostConnector());
Shape ConnectDiagonal(const Key& top_left,
                      const Key& top_right,
                      const Key& bottom_right,
                      const Key& bottom_left,
                      Shape connector = GetPostConnector());

}  // namespace kb
