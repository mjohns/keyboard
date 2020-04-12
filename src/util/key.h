#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "scad.h"
#include "transform.h"

namespace scad {

// All sizes in mm.
const double kSwitchWidth = 14.4;
const double kSwitchThickness = 4;
const double kWallWidth = 2;

const double kDsaHeight = 8;
const double kDsaTopSize = 13.2;     // 0.5 * kMmPerInch;
const double kDsaBottomSize = 18.4;  // 0.725 * kMmPerInch;
const double kDsaHalfSize = 16.2;

const double kSwitchHorizontalOffset = kSwitchWidth / 2 + kWallWidth;

Shape GetCapsuleConnector();
Shape GetSphereConnector();
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

  TransformList parent_transforms;
  TransformList local_transforms;

  double extra_height_top = 0;
  double extra_height_bottom = 0;
  double extra_width_left = 0;
  double extra_width_right = 0;
  // This makes the switch taller (moves the connectors down and increases wall height) while
  // leaving the top plate at the same point.
  double extra_z = 0;

  bool add_side_nub = true;
  // The distance back that the switch should be placed. By default the origin is at the top of the
  // dsa key cap.
  double switch_z_offset = kDsaHeight + 6.4;

  Key& SetPosition(double x, double y, double z);
  Key& SetParent(const Key& key);
  Key& SetParent(const TransformList& transforms);

  Transform& t() {
    return local_transforms.mutable_front();
  }

  Transform& AddTransform() {
    return local_transforms.AddTransformFront();
  }

  TransformList GetTransforms() const;
  TransformList GetSwitchTransforms() const;

  Shape GetSwitch() const;
  Shape GetInverseSwitch() const;
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
Shape Tri(const TransformList& t1,
          const TransformList& t2,
          const TransformList& t3,
          Shape connector = GetPostConnector());

}  // namespace scad
