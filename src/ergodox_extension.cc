#include <functional>
#include <vector>

#include "util.h"

using namespace scad;

// The distance between keys.
constexpr double kKeySpacing = 19;

// This is how slanted the top plate is.
constexpr double kRotationY = 10;
// Translate the keys in the z direction.
constexpr double kPlateHeight = 15;
// The corners of the ergodox ez are rounded.
constexpr double kCornerRadius = 2;

int main() {
  // b stands for bototm, t for top.

  // First thumb key.
  Key b1;
  b1.SetPosition(10, 19, 0);
  b1.t().x = 11.5;

  // Second thumb key.
  Key b2;
  b2.SetParent(b1);
  b2.SetPosition(kKeySpacing, 0, 0);

  // Bottom right key.
  Key b3;
  b3.Configure([&](Key& k) {
    k.SetParent(b2);
    k.SetPosition(kKeySpacing, -9, 1);
    k.extra_z = 1;
  });

  // Mid right key.
  Key t3;
  t3.SetParent(b2);
  t3.SetPosition(kKeySpacing, 10, 2);
  t3.extra_z = 2;

  // Top right key;
  Key t2;
  t2.SetParent(b2);
  t2.SetPosition(kKeySpacing, 10 + kKeySpacing, 3);
  t2.extra_z = 3;

  // Top left key.
  Key t1;
  t1.SetParent(b2);
  t1.SetPosition(0, 10 + kKeySpacing, 2.2);
  t1.extra_z = 2.2;

  std::vector<Key*> keys{&b1, &b2, &b3, &t1, &t2, &t3};
  std::vector<Shape> switches;
  std::vector<Shape> key_holes;
  for (Key* key : keys) {
    key->switch_z_offset = 0;
    switches.push_back(key->GetSwitch());
    key_holes.push_back(key->GetInverseSwitch());
  }

  // Create the base.
  double long_width = 58;
  double short_width = 39;

  Shape corner = Cylinder(1, kCornerRadius, 20);
  Shape base_inner_right = Cube(short_width - kCornerRadius * 2, long_width - kCornerRadius * 2, 1)
                               .Projection()
                               .LinearExtrude(1);
  // Cut a little of the top left corner to accomadate where the thumb keys meet the other keys.
  Shape base_inner_bottom = base_inner_right.RotateZ(90).Subtract(
      Cube(10, 7, 1).RotateZ(45).Translate(long_width / (-2), short_width / 2, 0));

  base_inner_right = base_inner_right.Translate(long_width - short_width / 2, long_width / 2, 0);
  base_inner_bottom = base_inner_bottom.Translate(long_width / 2, short_width / 2, 0);

  Shape base_right = Minkowski(base_inner_right, corner).Projection().LinearExtrude(1);
  Shape base_bottom = Minkowski(base_inner_bottom, corner).Projection().LinearExtrude(1);

  Shape base_outline_right = base_right - base_inner_right;
  Shape base_outline_bottom = base_bottom - base_inner_bottom;

  Shape base_outline =
      (base_outline_right + base_outline_bottom) - (base_inner_bottom + base_inner_right);

  // Shapes to cut any overhang off the sides and top.
  Shape full_base = Union(base_right, base_bottom);
  Shape horizontal_bounds =
      Cube(1000, 1000, 1000).Subtract(full_base.Projection().LinearExtrude(1000));

  Shape final_key_holes = UnionAll(key_holes).RotateY(kRotationY).TranslateZ(kPlateHeight);
  Shape top_plate = Cube(500, 500, kSwitchThickness)
                        .TranslateZ(kSwitchThickness / (-2))
                        .Add(UnionAll(switches))
                        .RotateY(kRotationY)
                        .TranslateZ(kPlateHeight);
  Shape top_bounds =
      Cube(800, 800, 100).TranslateZ(50).RotateY(kRotationY).TranslateZ(kPlateHeight);

  Shape ext = base_outline.Projection()
                  .LinearExtrude(40)
                  .TranslateZ(20)
                  .Subtract(top_bounds)
                  .Add(top_plate)
                  .Subtract(final_key_holes)
                  .Subtract(horizontal_bounds);

  ext.WriteToFile("ergodox_extension_left.scad");
  ext.MirrorX().WriteToFile("ergodox_extension_right.scad");
}
