#include <vector>

#include "util/key.h"
#include "util/scad.h"
#include "util/transform.h"

using namespace kb;

const double kStride = 29;
// const double kNegStride = -1 * kStride;

int main() {
  std::vector<Key*> keys;

  double radius = 50;
  Key top_template;
  top_template.local_transforms.TranslateZ(-1 * radius).RotateX(20).TranslateZ(radius);

  Key bottom_template;
  bottom_template.local_transforms.TranslateZ(-1 * radius).RotateX(-20).TranslateZ(radius);

  // Middle Finger - d
  Key m2;
  keys.push_back(&m2);

  Key t2 = top_template;
  t2.parent = &m2;
  keys.push_back(&t2);

  Key b2 = bottom_template;
  b2.parent = &m2;
  keys.push_back(&b2);

  // Index Finger - f
  Key m1(kStride, -6, 0);
  m1.parent = &m2;
  m1.t().ry = -3;
  keys.push_back(&m1);

  Key t1 = top_template;
  t1.parent = &m1;
  keys.push_back(&t1);

  Key b1 = bottom_template;
  b1.parent = &m1;
  keys.push_back(&b1);

  // Innermost Column - g
  Key m0(kStride, -2, 0);
  m0.parent = &m1;
  m0.t().ry = -3;
  keys.push_back(&m0);

  Key t0 = top_template;
  t0.parent = &m0;
  keys.push_back(&t0);

  Key b0 = bottom_template;
  b0.parent = &m0;
  keys.push_back(&b0);

  // Ring Finger - s
  Key m3(-1 * kStride, -6, 0);
  m3.parent = &m2;
  m3.t().ry = 3;
  keys.push_back(&m3);

  Key t3 = top_template;
  t3.parent = &m3;
  keys.push_back(&t3);

  Key b3 = bottom_template;
  b3.parent = &m3;
  keys.push_back(&b3);

  // Pinky Finger - a
  Key m4(-1 * kStride, -10, 0);
  m4.parent = &m3;
  m4.t().ry = 3;
  keys.push_back(&m4);

  Key t4 = top_template;
  t4.parent = &m4;
  keys.push_back(&t4);

  Key b4 = bottom_template;
  b4.parent = &m4;
  keys.push_back(&b4);

  // Shift Column
  Key m5(-1 * kStride, -2, 0);
  m5.parent = &m4;
  m5.t().ry = 3;
  keys.push_back(&m5);

  Key t5 = top_template;
  t5.parent = &m5;
  keys.push_back(&t5);

  Key b5 = bottom_template;
  b5.parent = &m5;
  keys.push_back(&b5);

  std::vector<Shape> shapes;
  for (Key* key : keys) {
    shapes.push_back(key->GetSwitch());
    //    shapes.push_back(key->GetCap().Color("red", .3));
  }
  shapes.push_back(ConnectHorizontal(m5, m4));

  UnionAll(shapes).WriteToFile("/tmp/simple.scad");
}
