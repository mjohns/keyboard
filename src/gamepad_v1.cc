// Simple 5x5 grid keypad with 1.25 size keys in shift column.

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "key.h"
#include "scad.h"
#include "transform.h"

using namespace scad;

constexpr double kSpacing = 19;
constexpr double kSpacingLarge = 19 + 5;
constexpr double kHorizontalOffset = 4;

constexpr double kSmallStep = 9;
constexpr double kMedStep = 23 / 2.0;
constexpr double kLargeStep = 28 / 2.0;

constexpr bool kWriteTestKeys = false;
// Add the caps into the stl for testing.
constexpr bool kAddCaps = false;

enum class Direction { UP, DOWN, LEFT, RIGHT };

void AddShapes(std::vector<Shape>* shapes, std::vector<Shape> to_add) {
  for (Shape s : to_add) {
    shapes->push_back(s);
  }
}

int main() {
  printf("generating..\n");

  std::vector<Key> keys;
  double x = 0;
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      double y = kSpacing * j * -1;
      Key k;
      k.SetPosition(x, y, 0);
      keys.push_back(k);
    }
    if (i == 0) {
      x += kMedStep + kSmallStep + 1;
    } else {
      x += kSpacing;
    }
  }

  if (kWriteTestKeys) {
    std::vector<Shape> test_shapes;
    for (Key key : keys) {
      key.add_side_nub = false;
      key.add_top_nub = false;
      key.extra_z = 2;
      test_shapes.push_back(key.GetSwitch());
      if (kAddCaps) {
        test_shapes.push_back(key.GetCap().Color("red"));
      }
    }
    UnionAll(test_shapes).WriteToFile("test_keys.scad");
    return 0;
  }

  glm::vec3 top_left(-14 - 2.5, 14, 0);
  glm::vec3 top_right(93, 14, 0);
  glm::vec3 bottom_left(-14 - 2.5, -91, 0);
  glm::vec3 bottom_right(93, -91, 0);

  Shape plate = Polygon({
      {top_left.x, top_left.y},
      {top_right.x, top_right.y},
      {bottom_right.x, bottom_right.y},
      {bottom_left.x, bottom_left.y},
  });

  glm::vec3 zero(0.0, 0.0, 0.0);
  Key k;
  double z = k.GetTopLeft().Apply(zero).z;

  plate = plate.LinearExtrude(4).TranslateZ(z - 2);

  std::vector<Shape> key_holes;
  for (Key key : keys) {
    key.add_side_nub = true;
    key.add_top_nub = false;
    key_holes.push_back(key.GetInverseSwitch());
  }
  plate.Subtract(UnionAll(key_holes)).WriteToFile("test_keys.scad");

  double holder_y = 28;
  double holder_x = 60;

  glm::vec3 under_top_left(top_left.x, top_left.y + holder_y, 0);
  glm::vec3 under_top_mid(top_left.x + holder_x, top_left.y + holder_y, 0);
  glm::vec3 under_top_mid2(top_left.x + holder_x, top_left.y, 0);

  double wall_width = 4;

  Shape left = Polygon({
      {under_top_left.x, under_top_left.y},
      {under_top_left.x + wall_width, under_top_left.y},
      {bottom_left.x + wall_width, bottom_left.y},
      {bottom_left.x, bottom_left.y},
  });

  Shape right = Polygon({
      {top_right.x - wall_width, top_right.y},
      {top_right.x, top_right.y},
      {bottom_right.x, bottom_right.y},
      {bottom_right.x - wall_width, bottom_right.y},
  });

  Shape bottom = Polygon({
      {bottom_left.x, bottom_left.y},
      {bottom_right.x, bottom_right.y},
      {bottom_right.x, bottom_right.y + wall_width},
      {bottom_left.x, bottom_left.y + wall_width},
  });

  Shape top1 = Polygon({
      {under_top_left.x, under_top_left.y},
      {under_top_mid.x, under_top_mid.y},
      {under_top_mid.x, under_top_mid.y - wall_width},
      {under_top_left.x, under_top_left.y - wall_width},
  });

  Shape top2 = Polygon({
      {under_top_mid2.x, under_top_mid2.y},
      {top_right.x, top_right.y},
      {top_right.x, top_right.y - wall_width},
      {under_top_mid2.x, under_top_mid2.y - wall_width},
  });

  Shape top3 = Polygon({
      {under_top_mid.x, under_top_mid.y},
      {under_top_mid2.x, under_top_mid2.y - wall_width},
      {under_top_mid2.x - wall_width, under_top_mid2.y - wall_width},
      {under_top_mid.x - wall_width, under_top_mid.y},
  });

  double wall_height = 8.5;
  Shape wall = Union(left, right, bottom, top1, top2, top3)
                   .LinearExtrude(wall_height)
                   .TranslateZ(wall_height / 2.0);

  Shape bottom_poly = Polygon({
                                  {under_top_left.x, under_top_left.y},
                                  {under_top_mid.x, under_top_mid.y},
                                  {under_top_mid2.x, under_top_mid2.y},
                                  {top_right.x, top_right.y},
                                  {bottom_right.x, bottom_right.y},
                                  {bottom_left.x, bottom_left.y},
                              })
                          .LinearExtrude(1.5)
                          .TranslateZ(-1 * (1.5 / 2.0));

  Shape usb_hole = Cube(20, 12, 7.5)
                       .TranslateZ(7.5 / 2.0)
                       .Translate(top_left.x, (top_left.y + under_top_left.y) / 2.0, 1.2);

  Union(wall, bottom_poly).Subtract(usb_hole).WriteToFile("bottom.scad");

  Shape post = Cube(3, 3, wall_height);
  post.WriteToFile("post.scad");

  double hole_x = under_top_mid.x - 12;
  double hole_y = under_top_mid.y - 12.5;
  Shape top = Polygon({
                          {under_top_left.x, under_top_left.y},
                          {under_top_mid.x, under_top_mid.y},
                          {under_top_mid2.x, under_top_mid2.y},
                          {top_left.x, top_left.y},
                      })
                  .Subtract(Circle(1.5, 30).Translate(hole_x, hole_y, 0))
                  .LinearExtrude(4);
  top.WriteToFile("top.scad");

  return 0;
}
