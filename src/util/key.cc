#include "util/key.h"

#include <memory>

#include "util/frame.h"
#include "util/scad.h"

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
const Frame kTopLeft{-1 * kSwitchOffset, kSwitchOffset, 0};
const Frame kTopRight{kSwitchOffset, kSwitchOffset, 0};
const Frame kBottomRight{kSwitchOffset, -1 * kSwitchOffset, 0};
const Frame kBottomLeft{-1 * kSwitchOffset, -1 * kSwitchOffset, 0};

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

Frame Key::GetAbsoluteFrame() const {
  std::vector<const Key*> keys;
  keys.push_back(this);
  Key* key = parent;
  while (key != nullptr) {
    keys.push_back(key);
    key = key->parent;
  }
  std::reverse(keys.begin(), keys.end());
  Frame frame;
  for (const Key* key : keys) {
    frame = PlaceFrame(frame, key->f);
  }
  return frame;
}

Frame Key::GetSwitchFrame() const {
  double offset = -1 * kDsaHeight;
  offset += -6.4;
  Frame frame;
  frame.z = offset;
  return PlaceFrame(GetAbsoluteFrame(), frame);
}

Shape Key::GetSwitch() const {
  return GetSwitchFrame().PlaceShape(MakeSwitch());
}

Shape Key::GetCap() const {
  return GetAbsoluteFrame().PlaceShape(MakeDsaCap());
}

Frame Key::GetTopRight() const {
  return PlaceFrame(GetSwitchFrame(), kTopRight);
}

Frame Key::GetTopLeft() const {
  return PlaceFrame(GetSwitchFrame(), kTopLeft);
}

Frame Key::GetBottomRight() const {
  return PlaceFrame(GetSwitchFrame(), kBottomRight);
}

Frame Key::GetBottomLeft() const {
  return PlaceFrame(GetSwitchFrame(), kBottomLeft);
}

Shape GetConnector() {
  SphereParams params;
  params.r = 2;
  params.fn = 30;
  return Sphere(params).TranslateZ(kSwitchThickness / (-2.0));
}

}  // namespace kb
