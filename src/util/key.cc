#include "util/key.h"

#include <cassert>
#include <memory>
#include <unordered_set>

#include "util/scad.h"
#include "util/transform.h"

namespace kb {
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

TransformList Key::GetTransforms() const {
  Key* key = parent;
  std::vector<const Key*> keys;
  keys.push_back(this);
  std::unordered_set<Key*> visited;
  while (key != nullptr) {
    bool inserted = visited.insert(key).second;
    assert(inserted && "found cycle in parents");
    keys.push_back(key);
    key = key->parent;
  }
  TransformList transforms;
  for (auto it = keys.begin(); it != keys.end(); ++it) {
    transforms.Append((*it)->local_transforms);
  }
  return transforms;
}

TransformList Key::GetSwitchTransforms() const {
  TransformList transforms;
  transforms.AddTransform().z = -1 * kDsaHeight - 6.4 - extra_z;
  return transforms.Append(GetTransforms());
}

Shape Key::GetSwitch() const {
  std::vector<Shape> shapes;
  if (extra_z > 0) {
    Shape s = Union(MakeSwitch(false), MakeSwitch(add_side_nub).TranslateZ(extra_z));
    if (extra_z > 4) {
      s+= MakeSwitch(false).TranslateZ(4);
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
  transforms.AddTransform({kSwitchOffset, kSwitchOffset, 0});
  return transforms.Append(GetSwitchTransforms());
}

TransformList Key::GetTopLeft() const {
  TransformList transforms;
  transforms.AddTransform({-1 * extra_width_left, extra_height_top, 0});
  return transforms.Append(GetTopLeftInternal());
}

TransformList Key::GetTopLeftInternal() const {
  TransformList transforms;
  transforms.AddTransform({-1 * kSwitchOffset, kSwitchOffset, 0});
  return transforms.Append(GetSwitchTransforms());
}

TransformList Key::GetBottomRight() const {
  TransformList transforms;
  transforms.AddTransform({extra_width_right, -1 * extra_height_bottom, 0});
  return transforms.Append(GetBottomRightInternal());
}

TransformList Key::GetBottomRightInternal() const {
  TransformList transforms;
  transforms.AddTransform({kSwitchOffset, -1 * kSwitchOffset, 0});
  return transforms.Append(GetSwitchTransforms());
}

TransformList Key::GetBottomLeft() const {
  TransformList transforms;
  transforms.AddTransform({-1 * extra_width_left, -1 * extra_height_bottom, 0});
  return transforms.Append(GetBottomLeftInternal());
}

TransformList Key::GetBottomLeftInternal() const {
  TransformList transforms;
  transforms.AddTransform({-1 * kSwitchOffset, -1 * kSwitchOffset, 0});
  return transforms.Append(GetSwitchTransforms());
}

Shape GetCapsuleConnector() {
  SphereParams params;
  params.r = 1;
  params.fn = 30;
  return Hull(Sphere(params).TranslateZ(-1), Sphere(params).TranslateZ(-3));
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

}  // namespace kb
