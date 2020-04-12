#include <functional>
#include <vector>

#include "util.h"

using namespace scad;

// The distance between keys.
constexpr double kKeySpacing = 19;

constexpr double kRotationY = 10;
constexpr double kPlateHeight = 15;

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
  b3.SetParent(b2);
  b3.SetPosition(kKeySpacing, -9, 0);
  b3.extra_z = 1;

  // Mid right key.
  Key t3;
  t3.SetParent(b2);
  t3.SetPosition(kKeySpacing, 10, 0);
  t3.extra_z = 2;

  // Top right key;
  Key t2;
  t2.SetParent(b2);
  t2.SetPosition(kKeySpacing, 10 + kKeySpacing, 0);
  t2.extra_z = 3;

  // Top left key.
  Key t1;
  t1.SetParent(b2);
  t1.SetPosition(0, 10 + kKeySpacing, 0);
  t1.extra_z = 2.2;

  std::vector<Key*> keys{&b1, &b2, &b3, &t1, &t2, &t3};

  // Create the base
  double corner_radius = 2;
  Shape full_round_corner = Cylinder(1, corner_radius, 20);
  Shape round_corner = full_round_corner.Subtract(
      Cube(corner_radius * 2, corner_radius * 2, 1).TranslateX(corner_radius));
  Shape round_corner_negative = Cube(corner_radius, corner_radius, 1)
                                    .Translate(corner_radius / (-2), corner_radius / (-2), 0)
                                    .Subtract(round_corner.TranslateX(0.1))
                                    .Projection()
                                    .LinearExtrude(10);

  double long_width = 58;
  double short_width = 39;
  double very_short_width = long_width - short_width;
  double base_thickness = 2;

  Shape long_wall = Cube(long_width, base_thickness, base_thickness)
                        .Subtract(round_corner_negative.TranslateX(corner_radius - long_width / 2)
                                      .TranslateY(corner_radius / 2))
                        .Subtract(round_corner_negative.RotateZ(90)
                                      .TranslateX(long_width / 2 - corner_radius)
                                      .TranslateY(corner_radius / 2));

  Shape short_wall = Cube(short_width, base_thickness, base_thickness)
                         .Subtract(round_corner_negative.TranslateX(corner_radius - short_width / 2)
                                       .TranslateY(corner_radius / 2))
                         .Subtract(round_corner_negative.RotateZ(90)
                                       .TranslateX(short_width / 2 - corner_radius)
                                       .TranslateY(corner_radius / 2));
  Shape very_short_wall =
      Cube(very_short_width, base_thickness, base_thickness)
          .Subtract(round_corner_negative.TranslateX(corner_radius - very_short_width / 2)
                        .TranslateY(corner_radius / 2));

  Shape bottom = Union(
      very_short_wall.RotateZ(-90)
          .TranslateX(base_thickness / 2 + very_short_width)
          .TranslateY(long_width - very_short_width / 2),
      very_short_wall.MirrorY()
          .TranslateX(very_short_width / 2)
          .TranslateY(short_width - base_thickness / 2),
      Cube(base_thickness, base_thickness, base_thickness)
          .Translate(base_thickness / 2, base_thickness / -2, 0)
          .Translate(very_short_width, short_width, 0),

      long_wall.TranslateX(long_width / 2).TranslateY(base_thickness / 2),
      long_wall.RotateZ(90).TranslateX(long_width - base_thickness / 2).TranslateY(long_width / 2),

      short_wall.RotateZ(180)
          .TranslateX(long_width - short_width / 2)
          .TranslateY(long_width - base_thickness / 2),
      short_wall.RotateZ(-90).TranslateX(base_thickness / 2).TranslateY(short_width / 2));

  Shape bottom_left = full_round_corner.Translate(corner_radius, corner_radius, 0);
  Shape bottom_right = full_round_corner.Translate(long_width - corner_radius, corner_radius, 0);
  Shape top_right =
      full_round_corner.Translate(long_width - corner_radius, long_width - corner_radius, 0);
  Shape top_left =
      full_round_corner.Translate(very_short_width + corner_radius, long_width - corner_radius, 0);
  Shape mid_left = full_round_corner.Translate(corner_radius, short_width - corner_radius, 0);
  Shape mid_right =
      full_round_corner.Translate(long_width - corner_radius, short_width - corner_radius, 0);
  Shape mid_bottom =
      full_round_corner.Translate(very_short_width + corner_radius, corner_radius, 0);

  Shape top_plate = Union(Hull(bottom_left, mid_left, mid_right, bottom_right),
                          Hull(mid_bottom, bottom_right, top_right, top_left))
                        .Projection()
                        .LinearExtrude(kSwitchThickness);

  // Shapes to cut any overhand off the sides.
  Shape corner_bounds = Cube(5, 4, 1000).RotateZ(45).TranslateY(short_width);
  Shape horizontal_bounds = Cube(1000, 1000, 1000)
                                .Subtract(top_plate.Projection().LinearExtrude(1000))
                                .Add(corner_bounds);
  std::vector<Shape> switch_shapes;
  std::vector<Shape> key_holes;
  for (Key* key : keys) {
    key->switch_z_offset = 0;
    switch_shapes.push_back(key->GetSwitch());
    key_holes.push_back(key->GetInverseSwitch());
  }

  Shape full_top = Cube(500, 500, kSwitchThickness)
                       .TranslateZ(kSwitchThickness / (-2))
                       .Add(UnionAll(switch_shapes))
                       .Subtract(UnionAll(key_holes))
                       .RotateY(kRotationY)
                       .TranslateZ(kPlateHeight)
                       .Color("red");
  Shape above_full_top =
      Cube(800, 800, 100).TranslateZ(50).RotateY(kRotationY).TranslateZ(kPlateHeight);

  Shape shell = bottom.Projection()
                    .LinearExtrude(40)
                    .TranslateZ(20)
                    .Subtract(above_full_top)
                    .Add(full_top)
                    .Subtract(horizontal_bounds);

  shell.WriteToFile("ergodox_extension_left.scad");
  shell.MirrorX().WriteToFile("ergodox_extension_right.scad");
}
