#include <functional>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "util.h"

using namespace scad;

const bool kIncludeConnectors = true;
const bool kUseCapsuleConnector = true;
const bool kShowManuform = false;
const bool kAddCaps = false;

std::ostream& operator<<(std::ostream& out, const glm::vec3& vec) {
  out << "{" << vec.x << " " << vec.y << " " << vec.z << "}";
  return out;
}

Shape Connector() {
  return kUseCapsuleConnector ? GetCapsuleConnector() : GetPostConnector();
}

int main() {
  std::vector<Key*> keys;

  double extra_width = 4;
  double extra_height = 4;

  double radius = 60;
  double rotation = 17;
  Key top_template;
  top_template.local_transforms.TranslateZ(-1 * radius).RotateX(rotation).TranslateZ(radius);

  Key origin;

  auto configure_key = [&keys](Key* k, std::function<void(Key & k)> fn) {
    k->extra_z = 1;
    fn(*k);
    keys.push_back(k);
  };

  Key bottom_template;
  bottom_template.local_transforms.TranslateZ(-1 * radius)
      .RotateX(-1 * rotation)
      .TranslateZ(radius);

  // Middle Finger - d
  Key m2{-3, 3, 28.5};
  configure_key(&m2, [&](Key& k) {
    k.SetParent(origin);
    k.t().ry = -15;
    k.AddTransform();
    k.t().x = -.3;
    k.t().z = 1;
    k.extra_width_left = 1;
    k.extra_width_right = 1;
  });

  Key t2 = top_template;
  configure_key(&t2, [&](Key& k) {
    k.SetParent(m2);
    k.extra_width_left = 1;
    k.extra_width_right = 1;
  });

  Key tt2 = top_template;
  configure_key(&tt2, [&](Key& k) {
    k.SetParent(t2);
    k.extra_height_top = extra_height;
  });

  Key b2 = bottom_template;
  configure_key(&b2, [&](Key& k) {
    k.SetParent(m2);
    k.extra_width_left = 1;
    k.extra_width_right = 1;
  });

  Key bb2 = bottom_template;
  configure_key(&bb2, [&](Key& k) {
    k.SetParent(b2);
    k.extra_width_right = 2;
    k.extra_height_bottom = extra_height;
    k.extra_width_left = 1;
    k.extra_width_right = 1;
  });

  // Index Finger - f
  Key m1{19.5, -2.7, 5.1};
  configure_key(&m1, [&](Key& k) {
    k.SetParent(m2);
    k.t().ry = -5;
    k.AddTransform();
    k.t().x = .2;
    k.t().z = .2;
    k.t().y = -.2;
    k.extra_z = 2;
  });

  Key t1 = top_template;
  configure_key(&t1, [&](Key& k) {
    k.SetParent(m1);
    k.extra_z = 2;
  });

  Key tt1 = top_template;
  configure_key(&tt1, [&](Key& k) {
    k.SetParent(t1);
    k.extra_z = 3;
    k.extra_height_top = extra_height;
  });

  Key b1 = bottom_template;
  configure_key(&b1, [&](Key& k) {
    k.SetParent(m1);
    k.extra_z = 2;
    k.extra_height_bottom = extra_height;
  });

  // Innermost Column - g
  Key m0{19.5, -1.0, 0.5};
  configure_key(&m0, [&](Key& k) {
    k.SetParent(m1);
    k.t().ry = -5;
    k.AddTransform();
    k.t().y = 1;
    k.t().z = .3;
    k.t().x = .2;
    k.extra_width_right = extra_width;
  });

  Key t0 = top_template;
  configure_key(&t0, [&](Key& k) {
    k.SetParent(m0);
    k.extra_width_right = extra_width;
  });

  Key tt0 = top_template;
  configure_key(&tt0, [&](Key& k) {
    k.SetParent(t0);
    k.extra_width_right = extra_width;
    k.extra_height_top = extra_height;
  });

  Key b0 = bottom_template;
  configure_key(&b0, [&](Key& k) {
    k.SetParent(m0);
    k.extra_z = 2;
    k.extra_width_right = extra_width;
  });

  // Ring Finger - s
  Key m3(-20, -3, 5.1);
  configure_key(&m3, [&](Key& k) {
    k.SetParent(m2);
    k.t().ry = 5;
    k.AddTransform();
    k.t().x = .2;
    k.t().z = .28;
    k.extra_z = 2;
    k.extra_width_left = 2;
  });

  Key t3 = top_template;
  configure_key(&t3, [&](Key& k) {
    k.SetParent(m3);
    k.extra_z = 2;
    k.extra_width_left = 2;
  });

  Key tt3 = top_template;
  configure_key(&tt3, [&](Key& k) {
    k.SetParent(t3);
    k.extra_z = 3;
    k.extra_height_top = extra_height;
    k.extra_width_left = 2;
  });

  Key b3 = bottom_template;
  configure_key(&b3, [&](Key& k) {
    k.SetParent(m3);
    k.extra_z = 2;
    k.extra_width_left = 2;
  });

  Key bb3 = bottom_template;
  configure_key(&bb3, [&](Key& k) {
    k.SetParent(b3);
    k.extra_height_bottom = extra_height;
  });

  // Pinky Finger - a
  Key m4{-20.2, -11.5, 6.3};
  configure_key(&m4, [&](Key& k) {
    k.t().y -= 2;
    k.SetParent(m3);
    k.t().ry = 5;
    k.AddTransform();
    k.t().z = .2;
    k.t().y = -.45;
    k.t().y -= 2;
    k.extra_z = 2;
  });

  Key t4 = top_template;
  configure_key(&t4, [&](Key& k) {
    k.SetParent(m4);
    k.extra_z = 2;
  });

  Key tt4 = top_template;
  configure_key(&tt4, [&](Key& k) {
    k.SetParent(t4);
    k.extra_z = 2;
    k.extra_height_top = extra_height;
  });

  Key b4 = bottom_template;
  configure_key(&b4, [&](Key& k) {
    k.SetParent(m4);
    k.extra_z = 2;
    k.extra_height_bottom = extra_height;
    k.extra_width_right = 1;
  });

  // Shift Column
  Key m5(-19.8, 0, 0.8);
  configure_key(&m5, [&](Key& k) {
    k.SetParent(m4);
    k.t().ry = 5;
    k.extra_z = 2;
    k.extra_width_left = extra_width;
  });

  Key t5 = top_template;
  configure_key(&t5, [&](Key& k) {
    k.SetParent(m5);
    k.extra_z = 2;
    k.extra_width_left = extra_width;
  });

  Key tt5 = top_template;
  configure_key(&tt5, [&](Key& k) {
    k.SetParent(t5);
    k.extra_z = 2;
    k.extra_height_top = extra_height;
    k.extra_width_left = extra_width;
  });

  Key b5 = bottom_template;
  configure_key(&b5, [&](Key& k) {
    k.SetParent(m5);
    k.extra_z = 2;
    k.extra_height_bottom = extra_height;
    k.extra_width_left = extra_width;
  });

  // Thumb

  // Top Right
  Key th1{22.5, -54.1, 50.2};
  configure_key(&th1, [&](Key& k) {
    k.SetParent(origin);
    k.t().rx = 13.3;
    k.t().ry = 21.6;
    k.t().rz = -9;
    k.t().y -= 2;
    k.AddTransform();
    k.t().z = 1;
    k.extra_height_top = 4;
    k.extra_height_bottom = extra_height;
    k.extra_width_right = 1;
    k.extra_width_left = 1;
    k.extra_z = 1;
  });

  Key th2{20, -5, 3.2};
  configure_key(&th2, [&](Key& k) {
    k.SetParent(th1);
    k.extra_z = 1;
    k.extra_height_bottom = extra_height;
    k.extra_width_right = 2;
  });

  Key th2_t{0, 20, 2.2};
  configure_key(&th2_t, [&](Key& k) {
    k.SetParent(th2);
    k.extra_z = 2;
    k.extra_width_right = 2;
  });

  Key th3{20, -12, 1.5};
  configure_key(&th3, [&](Key& k) {
    k.SetParent(th2);
    k.extra_z = 2;
    k.extra_height_bottom = extra_height;
    k.extra_width_right = extra_width;
  });

  Key th3_t{0, 20, 4.2};
  configure_key(&th3_t, [&](Key& k) {
    k.SetParent(th3);
    k.extra_z = 3.5;
    k.extra_height_top = extra_height;
    k.extra_width_right = extra_width;
  });

  std::vector<Shape> shapes;
  for (Key* key : keys) {
    shapes.push_back(key->GetSwitch());
    if (kAddCaps) {
      shapes.push_back(key->GetCap().Color("red", .6));
    }
  }

  if (kIncludeConnectors) {
    TransformList th_c1 = b1.GetBottomRight();
    std::vector<Shape> connectors = {
        // Connect middle row
        ConnectHorizontal(m5, m4, Connector()),
        ConnectHorizontal(m4, m3, Connector()),
        ConnectHorizontal(m3, m2, Connector()),
        ConnectHorizontal(m2, m1, Connector()),
        ConnectHorizontal(m1, m0, Connector()),

        // Connect top row
        ConnectHorizontal(t5, t4, Connector()),
        ConnectHorizontal(t4, t3, Connector()),
        ConnectHorizontal(t3, t2, Connector()),
        ConnectHorizontal(t2, t1, Connector()),
        ConnectHorizontal(t1, t0, Connector()),

        // Connect top/top row
        ConnectHorizontal(tt5, tt4, Connector()),
        ConnectHorizontal(tt4, tt3, Connector()),
        ConnectHorizontal(tt3, tt2, Connector()),
        ConnectHorizontal(tt2, tt1, Connector()),
        ConnectHorizontal(tt1, tt0, Connector()),

        ConnectHorizontal(bb3, bb2, Connector()),

        // Connect bottom row
        ConnectHorizontal(b5, b4, Connector()),
        ConnectHorizontal(b4, b3, Connector()),
        ConnectHorizontal(b3, b2, Connector()),
        ConnectHorizontal(b2, b1, Connector()),
        ConnectHorizontal(b1, b0, Connector()),

        ConnectVertical(tt5, t5, Connector()),
        ConnectVertical(t5, m5, Connector()),
        ConnectVertical(m5, b5, Connector()),

        ConnectVertical(tt4, t4, Connector()),
        ConnectVertical(t4, m4, Connector()),
        ConnectVertical(m4, b4, Connector()),

        ConnectVertical(tt3, t3, Connector()),
        ConnectVertical(t3, m3, Connector()),
        ConnectVertical(m3, b3, Connector()),
        ConnectVertical(b3, bb3, Connector()),

        ConnectVertical(tt2, t2, Connector()),
        ConnectVertical(t2, m2, Connector()),
        ConnectVertical(m2, b2, Connector()),
        ConnectVertical(b2, bb2, Connector()),

        ConnectVertical(tt1, t1, Connector()),
        ConnectVertical(t1, m1, Connector()),
        ConnectVertical(m1, b1, Connector()),

        ConnectVertical(tt0, t0, Connector()),
        ConnectVertical(t0, m0, Connector()),
        ConnectVertical(m0, b0, Connector()),

        ConnectDiagonal(t5, t4, m4, m5, Connector()),
        ConnectDiagonal(t4, t3, m3, m4, Connector()),
        ConnectDiagonal(t3, t2, m2, m3, Connector()),
        ConnectDiagonal(t2, t1, m1, m2, Connector()),
        ConnectDiagonal(t1, t0, m0, m1, Connector()),

        ConnectDiagonal(tt5, tt4, t4, t5, Connector()),
        ConnectDiagonal(tt4, tt3, t3, t4, Connector()),
        ConnectDiagonal(tt3, tt2, t2, t3, Connector()),
        ConnectDiagonal(tt2, tt1, t1, t2, Connector()),
        ConnectDiagonal(tt1, tt0, t0, t1, Connector()),

        ConnectDiagonal(m5, m4, b4, b5, Connector()),
        ConnectDiagonal(m4, m3, b3, b4, Connector()),
        ConnectDiagonal(m3, m2, b2, b3, Connector()),
        ConnectDiagonal(m2, m1, b1, b2, Connector()),
        ConnectDiagonal(m1, m0, b0, b1, Connector()),

        ConnectDiagonal(b3, b2, bb2, bb3, Connector()),

        Tri(bb3.GetTopLeft(), bb3.GetBottomLeft(), b4.GetBottomRight(), Connector()),
        Tri(b3.GetBottomLeft(), bb3.GetTopLeft(), b4.GetBottomRight(), Connector()),

        Tri(th2_t.GetTopLeft(), b0.GetBottomLeft(), b0.GetBottomRight(), Connector()),
        Tri(th2_t.GetTopLeft(), th2_t.GetTopRight(), b0.GetBottomRight(), Connector()),

        Tri(b1.GetBottomLeft(), bb2.GetTopRight(), b2.GetBottomRight(), Connector()),

        // Thumb connectors
        ConnectHorizontal(th1, th2, Connector()),
        ConnectHorizontal(th2, th3, Connector()),
        ConnectHorizontal(th2_t, th3_t, Connector()),
        ConnectVertical(th2_t, th2, Connector()),
        ConnectVertical(th3_t, th3, Connector()),
        ConnectDiagonal(th2_t, th3_t, th3, th2, Connector()),

        Tri(th1.GetTopRight(), th2.GetTopLeft(), th2_t.GetBottomLeft(), Connector()),
        Tri(th1.GetTopRight(), th2_t.GetTopLeft(), th2_t.GetBottomLeft(), Connector()),
        Tri(b0.GetBottomLeft(), b1.GetBottomRight(), th2_t.GetTopLeft(), Connector()),
        Tri(th1.GetTopRight(), b1.GetBottomRight(), th2_t.GetTopLeft(), Connector()),
        ConnectVertical(b1, th1, Connector()),
        Tri(b1.GetBottomLeft(), bb2.GetBottomRight(), th1.GetTopLeft(), Connector()),
        Tri(th1.GetBottomLeft(), bb2.GetBottomRight(), th1.GetTopLeft(), Connector()),
        // Tri(bb3.GetBottomRight(), bb2.GetBottomLeft(), bb2.GetBottomRight(), Connector()),
        // Tri(bb3.GetBottomRight(), th1.GetBottomLeft(), bb2.GetBottomRight(), Connector()),
        Tri(th2_t.GetTopRight(), b0.GetBottomRight(), b0.GetTopRight(), Connector()),
        Tri(th2_t.GetTopRight(), th3_t.GetTopLeft(), th3_t.GetTopRight(), Connector()),
        // Tri(th2_t.GetTopRight(), th3_t.GetTopRight(), b0.GetTopRight(), Connector()),

    };

    shapes.insert(shapes.end(), connectors.begin(), connectors.end());

    // Going around clockwise.
    std::vector<TransformList> wall_points = {
        tt0.GetTopLeft(),       tt1.GetTopRight(),   tt1.GetTopLeft(),     tt2.GetTopRight(),
        tt2.GetTopLeft(),       tt3.GetTopRight(),   tt3.GetTopLeft(),     tt4.GetTopRight(),
        tt4.GetTopLeft(),       tt5.GetTopRight(),   tt5.GetTopLeft(),     tt5.GetBottomLeft(),
        t5.GetTopLeft(),        t5.GetBottomLeft(),  m5.GetTopLeft(),      m5.GetBottomLeft(),
        b5.GetTopLeft(),        b5.GetBottomLeft(),  b5.GetBottomRight(),  b4.GetBottomLeft(),
        b4.GetBottomRight(),    bb3.GetBottomLeft(), bb3.GetBottomRight(), bb2.GetBottomLeft(),
        bb2.GetBottomRight(),   th1.GetBottomLeft(), th1.GetBottomRight(), th2.GetBottomLeft(),
        th2.GetBottomRight(),   th3.GetBottomLeft(), th3.GetBottomRight(), th3.GetTopRight(),
        th3_t.GetBottomRight(), th3_t.GetTopRight(), th2_t.GetTopRight(),  b0.GetTopRight(),
        m0.GetBottomRight(),    m0.GetTopRight(),    t0.GetBottomRight(),  t0.GetTopRight(),
        tt0.GetBottomRight(), tt0.GetTopRight(),
    };
    auto get_post = [](TransformList t) {
      Shape s = t.Apply(Connector());
      return Hull(s, s.Projection().LinearExtrude(1));
    };
    for (size_t i = 0; i < wall_points.size(); ++i) {
      /*
       *     glm::vec3 up = {0, 0, 1};
       *         std::vector<glm::vec3> points;
       *               TransformList t1 = wall_points[i];
       *                     TransformList t2 = wall_points[(i + 1) % wall_points.size()];
       *                           glm::vec3 p0{0, 0, 0};
       *                                 glm::vec3 p1 = t1.Apply(p0);
       *                                       glm::vec3 p2 = t2.Apply(p0);
       *                                             glm::vec3 diff = p2 - p1;
       *                                                   if (glm::length(diff) > 12) {
       *                                                           glm::vec3 mid_point = (diff
       * / 2.f) + p1; glm::vec3 out = glm::normalize(glm::cross(glm::normalize(diff), up));
       *                                                                           glm::vec3 p =
       * mid_point + (out * 5.f - glm::vec3(0, 0, 8)); Shape p_shape =
       * TransformList().Translate(p).Apply(GetSphereConnector()); shapes.push_back(p_shape);
       *                                                                                                 }
       */

      shapes.push_back(
          Hull(get_post(wall_points[i]), get_post(wall_points[(i + 1) % wall_points.size()])));
    }
  }

  if (kShowManuform) {
    shapes.push_back(Import("left.stl").Color("green", .3));
  }

  UnionAll(shapes).WriteToFile("dactyl_manuform.scad");
}

/*
 * These were an attempt to locate the dactyl manuform thumb keys.
 * Key th_2;
 * th_2.relative_to = &th_2_r;
 * th_2.y -= 4;
 * Key th_3_r{44.5, -5.5, 2.2};
 * th_3_r.relative_to = &th_1;
 * th_3_r.rz = -27;
 * th_3_r.rx = 10;
 * th_3_r.ry = 5;
 * Key th_3;
 * th_3.relative_to = &th_3_r;
 * Key th_4_r{28, -24.5, -3.3};
 * th_4_r.relative_to = &th_1;
 * th_4_r.rz = -33;
 * th_4_r.rx = 6;
 * th_4_r.ry = 7;
 * Key th_4{-.8, 1, 0};
 * th_4.relative_to = &th_4_r;
 * th_4.rx = 3;
 * th_4.y -= 2;
 * Key th_5_r{20, -10, 1};
 * th_5_r.relative_to = &th_3_r;
 * th_5_r.rz = -11;
 * Key th_5{-.4, -0.5, -1.4};
 * th_5.relative_to = &th_5_r;
 * th_5.ry = 1;
 * th_5.rz = 1;
 * Key th_6_r{-1.3, -18.8, -5.5};
 * th_6_r.relative_to = &th_5_r;
 * th_6_r.rx = -11.5;
 * th_6_r.ry = -1;
 * Key th_6;
 * th_6.relative_to = &th_6_r;
 * th_6.rx = 3;
 * th_6.y -= 2;
 * Key th_7;
 * th_7.relative_to = &th_2;
 * th_7.y += 18.7;
 * th_7.x += 5;
 * th_7.z += 5.7;
 * th_7.rx = 7;
 * th_7.rz = -4;
 * th_7.ry = -2;
 * Key th_8;
 * th_8.relative_to = &th_7;
 * th_8.y -= 2.1;
 * th_8.x += 21;
 * th_8.z -= .1;
 * th_8.rz = -15;
 * th_8.ry = -3;
 * */

