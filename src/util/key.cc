#include "util/key.h"

#include <memory>

#include "util/scad.h"
#include "util/transform.h"

namespace kb {
namespace {

// All sizes in mm.
const double kSwitchWidth = 14.4;
const double kSwitchThickness = 4;
const double kWallWidth = 2;

const double kDsaHeight = 8;
const double kDsaTopSize = 13.2;     // 0.5 * kMmPerInch;
const double kDsaBottomSize = 18.4;  // 0.725 * kMmPerInch;
const double kDsaHalfSize = 16.2;

const double kSwitchOffset = kSwitchWidth / 2 + kWallWidth;

Shape MakeSwitch() {
  std::vector<Shape> shapes;
  Shape top_wall = Cube(kSwitchWidth + kWallWidth * 2, kWallWidth, kSwitchThickness)
                       .Translate(0, kWallWidth / 2 + kSwitchWidth / 2, kSwitchThickness / 2);

  shapes.push_back(top_wall);
  shapes.push_back(top_wall.RotateZ(90));
  shapes.push_back(top_wall.RotateZ(180));
  shapes.push_back(top_wall.RotateZ(270));

  Shape side_nub = Hull(Cube(kWallWidth, 2.75, kSwitchThickness)
                            .Translate(kWallWidth / 2 + kSwitchWidth / 2, 0, kSwitchThickness / 2),
                        Cylinder(2.75, 1, 30).RotateX(90).Translate(kSwitchWidth / 2, 0, 1));
  shapes.push_back(side_nub);
  shapes.push_back(side_nub.RotateZ(180));

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

Transforms Key::GetTransforms() const {
  Key* key = parent;
  std::vector<Transform> transforms;
  transforms.push_back(t);
  while (key != nullptr) {
    transforms.push_back(key->t);
    key = key->parent;
  }
  return {transforms};
}

Transforms Key::GetSwitchTransforms() const {
  Transforms transforms = GetTransforms();
  transforms.AddTransform().z = -1 * kDsaHeight - 6.4;
  return transforms;
}

Shape Key::GetSwitch() const {
  return GetSwitchTransforms().Apply(MakeSwitch());
}

Shape Key::GetCap() const {
  return GetTransforms().Apply(MakeDsaCap());
}

Transforms Key::GetTopRight() const {
  Transforms transforms = GetTransforms();
  transforms.AddTransform({kSwitchOffset, kSwitchOffset, 0});
  return transforms;
}

Transforms Key::GetTopLeft() const {
  Transforms transforms = GetTransforms();
  transforms.AddTransform({-1 * kSwitchOffset, kSwitchOffset, 0});
  return transforms;
}

Transforms Key::GetBottomRight() const {
  Transforms transforms = GetTransforms();
  transforms.AddTransform({kSwitchOffset, -1 * kSwitchOffset, 0});
  return transforms;
}

Transforms Key::GetBottomLeft() const {
  Transforms transforms = GetTransforms();
  transforms.AddTransform({-1 * kSwitchOffset, -1 * kSwitchOffset, 0});
  return transforms;
}

Shape GetConnector() {
  SphereParams params;
  params.r = 2;
  params.fn = 30;
  return Sphere(params).TranslateZ(kSwitchThickness / (-2.0));
}

}  // namespace kb
