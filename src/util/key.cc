#include "key.h"

#include <cassert>
#include <memory>
#include <unordered_set>

#include "scad.h"
#include "transform.h"

namespace scad {
namespace {

const double kDsaSwitchZOffset = kDsaHeight + 6.4;
const double kSaSwitchZOffset = kSaHeight + 6.4;

Shape MakeSwitch(bool add_side_nub) {
  std::vector<Shape> shapes;
  Shape top_wall = Cube(kSwitchWidth + kWallWidth * 2, kWallWidth, kSwitchThickness)
                       .Translate(0, kWallWidth / 2 + kSwitchWidth / 2, kSwitchThickness / 2);

  shapes.push_back(top_wall);
  shapes.push_back(top_wall.RotateZ(90));
  shapes.push_back(top_wall.RotateZ(180));
  shapes.push_back(top_wall.RotateZ(270));

  if (add_side_nub) {
    Shape side_nub =
        Hull(Cube(kWallWidth, 2.75, kSwitchThickness)
                 .Translate(kWallWidth / 2 + kSwitchWidth / 2, 0, kSwitchThickness / 2),
             Cylinder(2.75, 1, 30).RotateX(90).Translate(kSwitchWidth / 2, 0, 1));
    shapes.push_back(side_nub);
    shapes.push_back(side_nub.RotateZ(180));
  }

  return UnionAll(shapes).TranslateZ(kSwitchThickness * -1);
}

struct CapSegment {
  double height;
  double width;
};

// Segments from bottom to top.
Shape MakeCap(const std::vector<CapSegment>& segments) {
  std::vector<Shape> shapes;
  double height_so_far = 0;
  for (int i = 0; i < segments.size() - 1; ++i) {
    double bottom_width = segments[i].width;
    double top_width = segments[i + 1].width;
    LinearExtrudeParams p;
    p.height = segments[i].height;
    p.scale = top_width / bottom_width;
    height_so_far += p.height;
    shapes.push_back(Square(bottom_width)
                         .LinearExtrude(p)
                         .TranslateZ(-0.5 * p.height)
                         .TranslateZ(height_so_far));
  }
  return UnionAll(shapes).TranslateZ(-1 * height_so_far);
}

Shape MakeDsaCap() {
  return MakeCap({
      {kDsaHeight / 2, kDsaBottomSize},
      {kDsaHeight / 2, kDsaHalfSize},
      {0, kDsaTopSize},
  });
}

Shape MakeSaCap() {
  return MakeCap({
      {kSaHeight / 2, kDsaBottomSize},
      {kSaHeight / 2, kSaHalfSize},
      {0, kDsaTopSize},
  });
}

}  // namespace

Key& Key::SetPosition(double x, double y, double z) {
  t().x = x;
  t().y = y;
  t().z = z;
  return *this;
}

Key& Key::SetParent(const Key& key) {
  parent_transforms = key.GetTransforms();
  return *this;
}

Key& Key::SetParent(const TransformList& transforms) {
  parent_transforms = transforms;
  return *this;
}

TransformList Key::GetTransforms() const {
  TransformList transforms;
  transforms.Append(local_transforms);
  transforms.Append(parent_transforms);
  return transforms;
}

TransformList Key::GetSwitchTransforms() const {
  double switch_z_offset = type == KeyType::DSA ? kDsaSwitchZOffset : kSaSwitchZOffset;
  if (disable_switch_z_offset) {
    switch_z_offset = 0;
  }
  TransformList transforms;
  transforms.AddTransform().z = -1 * switch_z_offset - extra_z;
  return transforms.Append(GetTransforms());
}

Shape Key::GetInverseSwitch() const {
  Shape s = GetSwitch();
  return Hull(s).Subtract(s);
}

Shape Key::GetSwitch() const {
  std::vector<Shape> shapes;
  if (extra_z > 0) {
    Shape s = Union(MakeSwitch(false), MakeSwitch(add_side_nub).TranslateZ(extra_z));
    if (extra_z > 4) {
      s += MakeSwitch(false).TranslateZ(4);
    }
    shapes.push_back(GetSwitchTransforms().Apply(s));
  } else {
    shapes.push_back(GetSwitchTransforms().Apply(MakeSwitch(add_side_nub)));
  }
  if (extra_height_top > 0) {
    shapes.push_back(Hull(GetTopRight().Apply(GetPostConnector()),
                          GetTopRightInternal().Apply(GetPostConnector()),
                          GetTopLeftInternal().Apply(GetPostConnector()),
                          GetTopLeft().Apply(GetPostConnector())));
  }
  if (extra_height_bottom > 0) {
    shapes.push_back(Hull(GetBottomLeft().Apply(GetPostConnector()),
                          GetBottomLeftInternal().Apply(GetPostConnector()),
                          GetBottomRightInternal().Apply(GetPostConnector()),
                          GetBottomRight().Apply(GetPostConnector())));
  }
  if (extra_width_left > 0) {
    shapes.push_back(Hull(GetBottomLeft().Apply(GetPostConnector()),
                          GetBottomLeftInternal().Apply(GetPostConnector()),
                          GetTopLeftInternal().Apply(GetPostConnector()),
                          GetTopLeft().Apply(GetPostConnector())));
  }
  if (extra_width_right > 0) {
    shapes.push_back(Hull(GetBottomRight().Apply(GetPostConnector()),
                          GetBottomRightInternal().Apply(GetPostConnector()),
                          GetTopRightInternal().Apply(GetPostConnector()),
                          GetTopRight().Apply(GetPostConnector())));
  }
  return UnionAll(shapes);
}

Shape Key::GetCap(bool fill_in_cap_path) const {
  Shape cap = type == KeyType::DSA ? MakeDsaCap() : MakeSaCap();
  if (fill_in_cap_path) {
    Shape bottom = cap.Projection().LinearExtrude(6).TranslateZ(-3 - kDsaHeight);
    cap = cap.Add(bottom);
  }
  return GetTransforms().Apply(cap);
}

TransformList Key::GetTopRight(double offset) const {
  TransformList transforms;
  transforms.AddTransform({extra_width_right + offset, extra_height_top + offset, 0});
  return transforms.Append(GetTopRightInternal());
}

TransformList Key::GetTopRightInternal() const {
  TransformList transforms;
  transforms.AddTransform({kSwitchHorizontalOffset, kSwitchHorizontalOffset, 0});
  return transforms.Append(GetSwitchTransforms());
}

TransformList Key::GetTopLeft(double offset) const {
  TransformList transforms;
  transforms.AddTransform({-1 * (extra_width_left + offset), extra_height_top + offset, 0});
  return transforms.Append(GetTopLeftInternal());
}

TransformList Key::GetTopLeftInternal() const {
  TransformList transforms;
  transforms.AddTransform({-1 * kSwitchHorizontalOffset, kSwitchHorizontalOffset, 0});
  return transforms.Append(GetSwitchTransforms());
}

TransformList Key::GetBottomRight(double offset) const {
  TransformList transforms;
  transforms.AddTransform({extra_width_right + offset, -1 * (extra_height_bottom + offset), 0});
  return transforms.Append(GetBottomRightInternal());
}

TransformList Key::GetBottomRightInternal() const {
  TransformList transforms;
  transforms.AddTransform({kSwitchHorizontalOffset, -1 * kSwitchHorizontalOffset, 0});
  return transforms.Append(GetSwitchTransforms());
}

TransformList Key::GetBottomLeft(double offset) const {
  TransformList transforms;
  transforms.AddTransform(
      {-1 * (extra_width_left + offset), -1 * (extra_height_bottom + offset), 0});
  return transforms.Append(GetBottomLeftInternal());
}

TransformList Key::GetBottomLeftInternal() const {
  TransformList transforms;
  transforms.AddTransform({-1 * kSwitchHorizontalOffset, -1 * kSwitchHorizontalOffset, 0});
  return transforms.Append(GetSwitchTransforms());
}

TransformList Key::GetMiddle() const {
  return GetSwitchTransforms();
}

std::vector<TransformList> Key::GetCorners(double offset) const {
  return {GetTopLeft(offset), GetTopRight(offset), GetBottomRight(offset), GetBottomLeft(offset)};
}

Shape GetCapsuleConnector() {
  Shape s = Cube(2, 2, kSwitchThickness);
  return s.TranslateZ(kSwitchThickness / -2);
  /*
  // Expensive to render
  SphereParams params;
  params.r = 1;
  params.fn = 20;
  return Hull(Sphere(params).TranslateZ(-1), Sphere(params).TranslateZ(-3));
  */
}

Shape GetSphereConnector() {
  SphereParams params;
  params.r = 2;
  params.fn = 30;
  return Sphere(params).TranslateZ(-2);
}

Shape GetPostConnector() {
  return Cube(.01, .01, kSwitchThickness).TranslateZ(kSwitchThickness / -2.0);
}

Shape ConnectVertical(const Key& top, const Key& bottom, Shape connector, double offset) {
  return Union(Hull(top.GetBottomRight(offset).Apply(connector),
                    top.GetBottomLeft(offset).Apply(connector),
                    bottom.GetTopLeft(offset).Apply(connector)),
               Hull(bottom.GetTopLeft(offset).Apply(connector),
                    bottom.GetTopRight(offset).Apply(connector),
                    top.GetBottomRight(offset).Apply(connector)));
}

Shape ConnectHorizontal(const Key& left, const Key& right, Shape connector, double offset) {
  return Union(Hull(left.GetTopRight(offset).Apply(connector),
                    left.GetBottomRight(offset).Apply(connector),
                    right.GetBottomLeft(offset).Apply(connector)),
               Hull(right.GetBottomLeft(offset).Apply(connector),
                    right.GetTopLeft(offset).Apply(connector),
                    left.GetTopRight(offset).Apply(connector)));
}

Shape ConnectDiagonal(const Key& top_left,
                      const Key& top_right,
                      const Key& bottom_right,
                      const Key& bottom_left,
                      Shape connector,
                      double offset) {
  return Union(Hull(top_left.GetBottomRight(offset).Apply(connector),
                    top_right.GetBottomLeft(offset).Apply(connector),
                    bottom_right.GetTopLeft(offset).Apply(connector)),
               Hull(bottom_right.GetTopLeft(offset).Apply(connector),
                    bottom_left.GetTopRight(offset).Apply(connector),
                    top_left.GetBottomRight(offset).Apply(connector)));
}

Shape Tri(const TransformList& t1,
          const TransformList& t2,
          const TransformList& t3,
          Shape connector) {
  return Hull(t1.Apply(connector), t2.Apply(connector), t3.Apply(connector));
}

}  // namespace scad
