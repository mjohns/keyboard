#include <vector>

#include "util/key.h"
#include "util/scad.h"
#include "util/transform.h"

using namespace kb;

int main() {
  std::vector<Key*> keys;

  double radius = 60;
  double rotation = 17;
  Key top_template;
  top_template.local_transforms.TranslateZ(-1 * radius).RotateX(rotation).TranslateZ(radius);

  Key bottom_template;
  bottom_template.local_transforms.TranslateZ(-1 * radius)
      .RotateX(-1 * rotation)
      .TranslateZ(radius);

  // Middle Finger - d
  Key m2{-3, 3, 28.5};
  m2.t().ry = -15;
  m2.AddTransform();
  m2.t().x = -.3;
  m2.t().z = 1;
  keys.push_back(&m2);

  Key t2 = top_template;
  t2.parent = &m2;
  keys.push_back(&t2);

  Key tt2 = top_template;
  tt2.parent = &t2;
  keys.push_back(&tt2);

  Key b2 = bottom_template;
  b2.parent = &m2;
  keys.push_back(&b2);

  Key bb2 = bottom_template;
  bb2.parent = &b2;
  keys.push_back(&bb2);

  // Index Finger - f
  Key m1{19.5, -2.7, 5.1};
  m1.parent = &m2;
  m1.t().ry = -5;
  m1.AddTransform();
  m1.t().x = .2;
  m1.t().z = .2;
  m1.t().y = -.2;
  keys.push_back(&m1);

  Key t1 = top_template;
  t1.parent = &m1;
  keys.push_back(&t1);

  Key tt1 = top_template;
  tt1.parent = &t1;
  keys.push_back(&tt1);

  Key b1 = bottom_template;
  b1.parent = &m1;
  keys.push_back(&b1);

  // Innermost Column - g
  Key m0{19.5, -1.0, 0.5};
  m0.parent = &m1;
  m0.t().ry = -5;
  m0.AddTransform();
  m0.t().y = 1;
  m0.t().z = .3;
  m0.t().x = .2;
  keys.push_back(&m0);

  Key t0 = top_template;
  t0.parent = &m0;
  keys.push_back(&t0);

  Key tt0 = top_template;
  tt0.parent = &t0;
  keys.push_back(&tt0);

  Key b0 = bottom_template;
  b0.parent = &m0;
  keys.push_back(&b0);

  // Ring Finger - s
  Key m3(-20, -3, 5.1);
  m3.parent = &m2;
  m3.t().ry = 5;
  m3.AddTransform();
  m3.t().x = .2;
  m3.t().z = .28;
  keys.push_back(&m3);

  Key t3 = top_template;
  t3.parent = &m3;
  keys.push_back(&t3);

  Key tt3 = top_template;
  tt3.parent = &t3;
  keys.push_back(&tt3);

  Key b3 = bottom_template;
  b3.parent = &m3;
  keys.push_back(&b3);

  Key bb3 = bottom_template;
  bb3.parent = &b3;
  keys.push_back(&bb3);

  // Pinky Finger - a
  Key m4{-20.2, -11.5, 6.3};
  m4.t().y -= 2;
  m4.parent = &m3;
  m4.t().ry = 5;
  m4.AddTransform();
  m4.t().z = .2;
  m4.t().y = -.45;
  keys.push_back(&m4);

  Key t4 = top_template;
  t4.parent = &m4;
  keys.push_back(&t4);

  Key tt4 = top_template;
  tt4.parent = &t4;
  keys.push_back(&tt4);

  Key b4 = bottom_template;
  b4.parent = &m4;
  keys.push_back(&b4);

  // Shift Column
  Key m5(-19.8, 0, 0.8);
  m5.parent = &m4;
  m5.t().ry = 5;
  keys.push_back(&m5);

  Key t5 = top_template;
  t5.parent = &m5;
  keys.push_back(&t5);

  Key tt5 = top_template;
  tt5.parent = &t5;
  keys.push_back(&tt5);

  Key b5 = bottom_template;
  b5.parent = &m5;
  keys.push_back(&b5);

  // Thumb

  // Top Right
  Key th1{22.5, -54.1, 50.2};
  th1.t().rx = 13.3;
  th1.t().ry = 21.6;
  th1.t().rz = -9;
  th1.AddTransform();
  th1.t().z = 1;
  keys.push_back(&th1);

  /*
  Key th_2_r{20, 4.65, 2.2};
  th_2_r.relative_to = &th_1;

  Key th_2;
  th_2.relative_to = &th_2_r;
  th_2.y -= 4;

  Key th_3_r{44.5, -5.5, 2.2};
  th_3_r.relative_to = &th_1;
  th_3_r.rz = -27;
  th_3_r.rx = 10;
  th_3_r.ry = 5;

  Key th_3;
  th_3.relative_to = &th_3_r;

  Key th_4_r{28, -24.5, -3.3};
  th_4_r.relative_to = &th_1;
  th_4_r.rz = -33;
  th_4_r.rx = 6;
  th_4_r.ry = 7;

  Key th_4{-.8, 1, 0};
  th_4.relative_to = &th_4_r;
  th_4.rx = 3;
  th_4.y -= 2;

  Key th_5_r{20, -10, 1};
  th_5_r.relative_to = &th_3_r;
  th_5_r.rz = -11;

  Key th_5{-.4, -0.5, -1.4};
  th_5.relative_to = &th_5_r;
  th_5.ry = 1;
  th_5.rz = 1;

  Key th_6_r{-1.3, -18.8, -5.5};
  th_6_r.relative_to = &th_5_r;
  th_6_r.rx = -11.5;
  th_6_r.ry = -1;

  Key th_6;
  th_6.relative_to = &th_6_r;
  th_6.rx = 3;
  th_6.y -= 2;

  Key th_7;
  th_7.relative_to = &th_2;
  th_7.y += 18.7;
  th_7.x += 5;
  th_7.z += 5.7;
  th_7.rx = 7;
  th_7.rz = -4;
  th_7.ry = -2;

  Key th_8;
  th_8.relative_to = &th_7;
  th_8.y -= 2.1;
  th_8.x += 21;
  th_8.z -= .1;
  th_8.rz = -15;
  th_8.ry = -3;
  */

  std::vector<Shape> shapes;
  for (Key* key : keys) {
    shapes.push_back(key->GetSwitch());
    //    shapes.push_back(key->GetCap().Color("red", .3));
  }

  /*
  shapes.push_back(ConnectHorizontal(m5, m4, GetConnector()));
  shapes.push_back(ConnectHorizontal(m4, m3, GetConnector()));
  shapes.push_back(ConnectHorizontal(m3, m2, GetConnector()));
  shapes.push_back(ConnectHorizontal(m2, m1, GetConnector()));
  shapes.push_back(ConnectHorizontal(m1, m0, GetConnector()));

  shapes.push_back(ConnectHorizontal(t5, t4, GetConnector()));
  shapes.push_back(ConnectHorizontal(t4, t3, GetConnector()));
  shapes.push_back(ConnectHorizontal(t3, t2, GetConnector()));
  shapes.push_back(ConnectHorizontal(t2, t1, GetConnector()));
  shapes.push_back(ConnectHorizontal(t1, t0, GetConnector()));

  shapes.push_back(ConnectHorizontal(b5, b4, GetConnector()));
  shapes.push_back(ConnectHorizontal(b4, b3, GetConnector()));
  shapes.push_back(ConnectHorizontal(b3, b2, GetConnector()));
  shapes.push_back(ConnectHorizontal(b2, b1, GetConnector()));
  shapes.push_back(ConnectHorizontal(b1, b0, GetConnector()));

  shapes.push_back(ConnectVertical(t5, m5, GetConnector()));
  shapes.push_back(ConnectVertical(m5, b5, GetConnector()));

  shapes.push_back(ConnectVertical(t4, m4, GetConnector()));
  shapes.push_back(ConnectVertical(m4, b4, GetConnector()));

  shapes.push_back(ConnectVertical(t3, m3, GetConnector()));
  shapes.push_back(ConnectVertical(m3, b3, GetConnector()));

  shapes.push_back(ConnectVertical(t2, m2, GetConnector()));
  shapes.push_back(ConnectVertical(m2, b2, GetConnector()));

  shapes.push_back(ConnectVertical(t1, m1, GetConnector()));
  shapes.push_back(ConnectVertical(m1, b1, GetConnector()));

  shapes.push_back(ConnectVertical(t0, m0, GetConnector()));
  shapes.push_back(ConnectVertical(m0, b0, GetConnector()));

  shapes.push_back(ConnectDiagonal(t5, t4, m4, m5, GetConnector()));
  shapes.push_back(ConnectDiagonal(t4, t3, m3, m4, GetConnector()));
  shapes.push_back(ConnectDiagonal(t3, t2, m2, m3, GetConnector()));
  shapes.push_back(ConnectDiagonal(t2, t1, m1, m2, GetConnector()));
  shapes.push_back(ConnectDiagonal(t1, t0, m0, m1, GetConnector()));

  shapes.push_back(ConnectDiagonal(m5, m4, b4, b5, GetConnector()));
  shapes.push_back(ConnectDiagonal(m4, m3, b3, b4, GetConnector()));
  shapes.push_back(ConnectDiagonal(m3, m2, b2, b3, GetConnector()));
  shapes.push_back(ConnectDiagonal(m2, m1, b1, b2, GetConnector()));
  shapes.push_back(ConnectDiagonal(m1, m0, b0, b1, GetConnector()));
  */

  shapes.push_back(Import("/tmp/left.stl").Color("green", .3));

  UnionAll(shapes).WriteToFile("/tmp/simple.scad");
}
