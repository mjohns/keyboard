#include "key.h"

#include <cassert>
#include <memory>
#include <unordered_set>

#include "scad.h"
#include "transform.h"

namespace scad {
namespace {

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

Shape Key::GetCap() const {
  return GetTransforms().Apply(MakeDsaCap());
}

TransformList Key::GetTopRight() const {
  TransformList transforms;
  transforms.AddTransform({extra_width_right, extra_height_top, 0});
  return transforms.Append(GetTopRightInternal());
}

TransformList Key::GetTopRightInternal() const {
  TransformList transforms;
  transforms.AddTransform({kSwitchHorizontalOffset, kSwitchHorizontalOffset, 0});
  return transforms.Append(GetSwitchTransforms());
}

TransformList Key::GetTopLeft() const {
  TransformList transforms;
  transforms.AddTransform({-1 * extra_width_left, extra_height_top, 0});
  return transforms.Append(GetTopLeftInternal());
}

TransformList Key::GetTopLeftInternal() const {
  TransformList transforms;
  transforms.AddTransform({-1 * kSwitchHorizontalOffset, kSwitchHorizontalOffset, 0});
  return transforms.Append(GetSwitchTransforms());
}

TransformList Key::GetBottomRight() const {
  TransformList transforms;
  transforms.AddTransform({extra_width_right, -1 * extra_height_bottom, 0});
  return transforms.Append(GetBottomRightInternal());
}

TransformList Key::GetBottomRightInternal() const {
  TransformList transforms;
  transforms.AddTransform({kSwitchHorizontalOffset, -1 * kSwitchHorizontalOffset, 0});
  return transforms.Append(GetSwitchTransforms());
}

TransformList Key::GetBottomLeft() const {
  TransformList transforms;
  transforms.AddTransform({-1 * extra_width_left, -1 * extra_height_bottom, 0});
  return transforms.Append(GetBottomLeftInternal());
}

TransformList Key::GetBottomLeftInternal() const {
  TransformList transforms;
  transforms.AddTransform({-1 * kSwitchHorizontalOffset, -1 * kSwitchHorizontalOffset, 0});
  return transforms.Append(GetSwitchTransforms());
}

Shape GetCapsuleConnector() {
  Shape s = Cube(2, 2, kSwitchThickness);
  return s.TranslateZ(kSwitchThickness / -2);
  /*
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

Shape ConnectVertical(const Key& top, const Key& bottom, Shape connector) {
  return Union(Hull(top.GetBottomRight().Apply(connector),
                    top.GetBottomLeft().Apply(connector),
                    bottom.GetTopLeft().Apply(connector)),
               Hull(bottom.GetTopLeft().Apply(connector),
                    bottom.GetTopRight().Apply(connector),
                    top.GetBottomRight().Apply(connector)));
}

Shape ConnectHorizontal(const Key& left, const Key& right, Shape connector) {
  return Union(Hull(left.GetTopRight().Apply(connector),
                    left.GetBottomRight().Apply(connector),
                    right.GetBottomLeft().Apply(connector)),
               Hull(right.GetBottomLeft().Apply(connector),
                    right.GetTopLeft().Apply(connector),
                    left.GetTopRight().Apply(connector)));
}

Shape ConnectDiagonal(const Key& top_left,
                      const Key& top_right,
                      const Key& bottom_right,
                      const Key& bottom_left,
                      Shape connector) {
  return Union(Hull(top_left.GetBottomRight().Apply(connector),
                    top_right.GetBottomLeft().Apply(connector),
                    bottom_right.GetTopLeft().Apply(connector)),
               Hull(bottom_right.GetTopLeft().Apply(connector),
                    bottom_left.GetTopRight().Apply(connector),
                    top_left.GetBottomRight().Apply(connector)));
}

Shape Tri(const TransformList& t1,
          const TransformList& t2,
          const TransformList& t3,
          Shape connector) {
  return Hull(t1.Apply(connector), t2.Apply(connector), t3.Apply(connector));
}

}  // namespace scad
