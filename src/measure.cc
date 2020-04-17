#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "util.h"

using namespace scad;

constexpr bool kShowPoints = false;

constexpr double kDefaultKeySpacing = 19;
constexpr double kBowlKeySpacing = 18;

constexpr double kDColumnRadius = 55;
constexpr double kAColumnRadius = 70;
constexpr double kSColumnRadius = 65;
constexpr double kGColumnRadius = 65;
constexpr double kFColumnRadius = 70;
constexpr double kCapsColumnRadius = 60;

// Rotates a key about the x axis until it has traveled the direct distance (not on the arc).
Key GetRotatedKey(double distance, double radius, bool up) {
  double rotation_direction = up ? 1.0 : -1.0;
  double degrees = 1;
  while (true) {
    Key k;
    k.local_transforms.TranslateZ(-1 * radius)
        .RotateX(rotation_direction * degrees)
        .TranslateZ(radius);
    glm::vec3 point = k.GetTransforms().Apply(kOrigin);
    float current_distance = glm::length(point);
    if (current_distance > distance) {
      return k;
    }
    degrees += .01;
  }
}

Shape GetPoints();

int main() {
  // This is the parent of all keys. If you want to tilt the entire keyboard changes this.
  Key key_origin;

  //
  // Thumb keys
  //

  Key thumb;
  thumb.Configure([&](Key& k) {
    k.name = "thumb";
    k.SetParent(key_origin);
    k.SetPosition(60, -9.18, 42.83);
    k.t().rz = -21;
    k.t().rx = 12;
    k.t().ry = -4.5;
  });

  // Second thumb key.
  Key thumb_delete;
  thumb_delete.Configure([&](Key& k) {
    k.name = "thumb_delete";
    k.SetParent(thumb);
    k.SetPosition(kDefaultKeySpacing, 0, 0);
  });

  // Bottom side key.
  Key thumb_end;
  thumb_end.Configure([&](Key& k) {
    k.name = "thumb_end";
    k.SetParent(thumb_delete);
    k.SetPosition(kDefaultKeySpacing, -9, 0);
  });

  // Middle side key.
  Key thumb_home;
  thumb_home.Configure([&](Key& k) {
    k.name = "thumb_home";
    k.SetParent(thumb_delete);
    k.SetPosition(kDefaultKeySpacing, 10, 0);
  });

  // Top side key;
  Key thumb_alt;
  thumb_alt.Configure([&](Key& k) {
    k.name = "thumb_alt";
    k.SetParent(thumb_delete);
    k.SetPosition(kDefaultKeySpacing, 10 + kDefaultKeySpacing, 0);
  });

  // Top left key.
  Key thumb_ctrl;
  thumb_ctrl.Configure([&](Key& k) {
    k.name = "thumb_ctrl";
    k.SetParent(thumb_delete);
    k.SetPosition(0, 10 + kDefaultKeySpacing, 0);
  });

  std::vector<Key*> thumb_keys = {
      &thumb, &thumb_delete, &thumb_end, &thumb_home, &thumb_alt, &thumb_ctrl};

  //
  // Main bowl keys
  //

  // D column - this is the column that all others in the bowl are relative to.
  Key key_d;
  key_d.Configure([&](Key& k) {
    k.name = "d";
    k.SetParent(key_origin);
    k.SetPosition(26.40, 50.32, 17.87);
    k.t().ry = -15;
  });

  Key key_e = GetRotatedKey(kBowlKeySpacing, kDColumnRadius, true);
  key_e.Configure([&](Key& k) {
    k.name = "e";
    k.SetParent(key_d);
  });

  // This key is rotated more than the others in the column.
  Key key_3 = GetRotatedKey(kBowlKeySpacing, 50, true);
  key_3.Configure([&](Key& k) {
    k.name = "3";
    k.SetParent(key_e);
  });

  Key key_c = GetRotatedKey(kBowlKeySpacing, kDColumnRadius, false);
  key_c.Configure([&](Key& k) {
    k.name = "c";
    k.SetParent(key_d);
  });

  Key key_left_arrow = GetRotatedKey(kBowlKeySpacing, kDColumnRadius, false);
  key_left_arrow.Configure([&](Key& k) {
    k.name = "left_arrow";
    k.SetParent(key_c);
  });

  // S column
  Key key_s;
  key_s.Configure([&](Key& k) {
    k.name = "s";

    // Absolute:
    // k.SetPosition(6.09, 50.23, 18.05);
    // k.t().ry = -10;

    k.SetParent(key_d);
    k.SetPosition(-19.571, -0.090, 5.430);
    k.t().ry = 5;
  });

  Key key_w = GetRotatedKey(kBowlKeySpacing, kSColumnRadius, true);
  key_w.Configure([&](Key& k) {
    k.name = "w";
    k.SetParent(key_s);
  });

  Key key_2 = GetRotatedKey(kBowlKeySpacing, kSColumnRadius, true);
  key_2.Configure([&](Key& k) {
    k.name = "2";
    k.SetParent(key_w);
  });

  Key key_x = GetRotatedKey(kBowlKeySpacing, kSColumnRadius, false);
  key_x.Configure([&](Key& k) {
    k.name = "x";
    k.SetParent(key_s);
  });

  Key key_slash = GetRotatedKey(kBowlKeySpacing, kSColumnRadius, false);
  key_slash.Configure([&](Key& k) {
    k.name = "slash";
    k.SetParent(key_x);
  });

  // F column
  Key key_f;
  key_f.Configure([&](Key& k) {
    k.name = "f";

    // Absolute:
    // k.SetPosition(44.3, 49.37, 28.1);
    // k.t().ry = -20;

    k.SetParent(key_d);
    k.SetPosition(19.938, -0.950, 5.249);
    k.t().ry = -5;
  });

  Key key_r = GetRotatedKey(kBowlKeySpacing, kFColumnRadius, true);
  key_r.Configure([&](Key& k) {
    k.name = "r";
    k.SetParent(key_f);
  });

  Key key_4 = GetRotatedKey(kBowlKeySpacing, kFColumnRadius, true);
  key_4.Configure([&](Key& k) {
    k.name = "4";
    k.SetParent(key_r);
  });

  Key key_v = GetRotatedKey(kBowlKeySpacing, kFColumnRadius, false);
  key_v.Configure([&](Key& k) {
    k.name = "v";
    k.SetParent(key_f);
  });

  Key key_right_arrow = GetRotatedKey(kBowlKeySpacing, kFColumnRadius, false);
  key_right_arrow.Configure([&](Key& k) {
    k.name = "right_arrow";
    k.SetParent(key_v);
  });

  // G column
  Key key_g;
  key_g.Configure([&](Key& k) {
    k.name = "g";

    // Absolute:
    // k.SetPosition(60.16, 48.06, 37.39);
    // k.t().ry = -30;

    k.SetParent(key_f);
    k.SetPosition(18.081, -1.310, 3.305);
    k.t().ry = -10;
  });

  Key key_t = GetRotatedKey(kBowlKeySpacing, kGColumnRadius, true);
  key_t.Configure([&](Key& k) {
    k.name = "t";
    k.SetParent(key_g);
  });

  Key key_5 = GetRotatedKey(kBowlKeySpacing, kGColumnRadius, true);
  key_5.Configure([&](Key& k) {
    k.name = "5";
    k.SetParent(key_t);
  });

  Key key_b = GetRotatedKey(kBowlKeySpacing, kGColumnRadius, false);
  key_b.Configure([&](Key& k) {
    k.name = "b";
    k.SetParent(key_g);
  });

  // A column
  Key key_a;
  key_a.Configure([&](Key& k) {
    k.name = "a";

    // Absolute:
    // k.SetPosition(-15.41, 44.06, 19.7);
    // k.t().ry = -10;

    k.SetParent(key_s);
    k.SetPosition(-20.887, -6.170, 5.358);
  });

  Key key_q = GetRotatedKey(kBowlKeySpacing, kAColumnRadius, true);
  key_q.Configure([&](Key& k) {
    k.name = "q";
    k.SetParent(key_a);
  });

  Key key_1 = GetRotatedKey(kBowlKeySpacing, kAColumnRadius, true);
  key_1.Configure([&](Key& k) {
    k.name = "1";
    k.SetParent(key_q);
  });

  Key key_z = GetRotatedKey(kBowlKeySpacing, kAColumnRadius, false);
  key_z.Configure([&](Key& k) {
    k.name = "z";
    k.SetParent(key_a);
  });

  Key key_tilda = GetRotatedKey(kBowlKeySpacing, kAColumnRadius, false);
  key_tilda.Configure([&](Key& k) {
    k.name = "tilda";
    k.SetParent(key_z);
  });

  // Caps column
  Key key_caps;
  key_caps.Configure([&](Key& k) {
    k.name = "caps";

    // Absolute:
    // k.SetPosition(-37.7, 48.06, 15.98);
    // k.t().ry = -5;

    k.SetParent(key_a);
    k.SetPosition(-22.597, 4.000, 0.207);
    k.t().ry = 5;
  });

  Key key_tab = GetRotatedKey(kBowlKeySpacing, kCapsColumnRadius, true);
  key_tab.Configure([&](Key& k) {
    k.name = "tab";
    k.SetParent(key_caps);
  });

  Key key_plus = GetRotatedKey(kBowlKeySpacing, kCapsColumnRadius, true);
  key_plus.Configure([&](Key& k) {
    k.name = "plus";
    k.SetParent(key_tab);
  });

  Key key_shift = GetRotatedKey(kBowlKeySpacing, kCapsColumnRadius, false);
  key_shift.Configure([&](Key& k) {
    k.name = "shift";
    k.SetParent(key_caps);
  });

  std::vector<Key*> main_keys = {
      &key_caps,       &key_tab, &key_plus, &key_shift,       &key_d, &key_e, &key_c,
      &key_left_arrow, &key_3,   &key_s,    &key_w,           &key_2, &key_x, &key_slash,
      &key_f,          &key_r,   &key_v,    &key_right_arrow, &key_4, &key_t, &key_5,
      &key_b,          &key_g,   &key_a,    &key_q,           &key_1, &key_z, &key_tilda};

  std::vector<Key*> keys_to_print;
  PushBackAll(&keys_to_print, thumb_keys);
  PushBackAll(&keys_to_print, main_keys);
  std::vector<Shape> shapes;
  if (kShowPoints) {
    shapes.push_back(GetPoints());
  }
  Shape s2 = Sphere(1, 30).Color("blue", 0.5);

  std::vector<Shape> golden_points;
  // Keys are measured from the tip of the switch and keys are measured from the tip of the cap.
  // amount.
  double switch_top_z_offset = 10;
  for (Key* key : keys_to_print) {
    Shape key_point = key->GetTransforms().Apply(s2);
    golden_points.push_back(key_point);
    if (kShowPoints) {
      shapes.push_back(key_point);
    }
    shapes.push_back(key_point);

    key->AddTransform();
    key->t().z -= 10;
    key->disable_switch_z_offset = true;
    key->add_side_nub = false;
    key->extra_z = 4;
    shapes.push_back(key->GetSwitch());
    // shapes.push_back(key->GetCap());
  }

  shapes.push_back(Import("../things/points.stl").Color("red", 0.5));
  UnionAll(shapes).WriteToFile("measure.scad");
  UnionAll(golden_points).WriteToFile("points.scad");
}

Shape GetPoints() {
  glm::vec3 p_3(25.66, 85.85, 30.1);
  glm::vec3 p_e(27.51, 69.14, 21.06);
  glm::vec3 p_d(27.45, 52.44, 17.13);
  glm::vec3 p_c(26.57, 34.7, 19.8);
  glm::vec3 p_left_arrow(23.8, 17.64, 28.49);

  glm::vec3 p_4(40.4, 84.05, 35.93);
  glm::vec3 p_r(43.39, 67.27, 29.52);
  glm::vec3 p_f(43.99, 47.96, 27.15);
  glm::vec3 p_v(43.15, 30.82, 29.63);
  glm::vec3 p_right_arrow(39.66, 13.72, 33.82);

  glm::vec3 p_5(56.74, 81.46, 46.84);
  glm::vec3 p_t(59.14, 63.05, 39.76);
  glm::vec3 p_g(59.63, 46.97, 36.22);
  glm::vec3 p_b(59.06, 28.93, 38.00);

  glm::vec3 p_2(5.49, 85.47, 26.82);
  glm::vec3 p_w(5.49, 67.57, 20.03);
  glm::vec3 p_s(5.44, 50.72, 17.44);
  glm::vec3 p_x(4.39, 31.48, 19.24);

  double xoff = 69.84 - 5.44;

  glm::vec3 p_plus(27.05 - xoff, 81.79, 26.94);
  glm::vec3 p_1(48.58 - xoff, 79.36, 27.47);

  glm::vec3 p_tab(27.14 - xoff, 66.13, 18.48);
  glm::vec3 p_q(49.12 - xoff, 62.06, 21.59);

  glm::vec3 p_caps(26.56 - xoff, 48.09, 15.77);
  glm::vec3 p_a(48.67 - xoff, 44.18, 19.37);

  glm::vec3 p_shift(25.5 - xoff, 30.34, 19);
  glm::vec3 p_z(47.58 - xoff, 25.86, 21.87);

  glm::vec3 p_tilda(46 - xoff, 8.91, 28.73);
  glm::vec3 p_slash(3.35, 14.74, 25.84);

  glm::vec3 p_backspace(60, -9.18, 42.83);
  glm::vec3 p_delete(77.71, -16, 42.78);
  glm::vec3 p_ctrl(87.27, 9.95, 48.94);
  glm::vec3 p_alt(104.74, 4, 48.75);

  Shape s = Sphere(1, 30).Color("red", 0.5);

  glm::vec3 pp_e(25.84, 68.15, 20.8);
  glm::vec3 pp_3(24.64, 84.84, 30.22);
  glm::vec3 pp_c(25.67, 32.65, 20.8);
  glm::vec3 pp_left_arrow(22.13, 15.90, 28.38);

  glm::vec3 pp_caps(-37.7, 48.06, 15.98);
  glm::vec3 pp_a(-15.41, 44.06, 19.7);
  glm::vec3 pp_s(6.09, 50.23, 18.05);
  glm::vec3 pp_d(26.40, 50.32, 17.87);
  glm::vec3 pp_f(44.3, 49.37, 28.1);
  glm::vec3 pp_g(60.16, 48.06, 37.39);

  printf("%.3f\n", glm::length(pp_g - pp_f));

  Shape s2 = s.Color("green", 0.5);

  return Union(s.Translate(p_backspace),
               s.Translate(p_delete),
               s.Translate(p_ctrl),
               s.Translate(p_alt),
               s.Translate(p_plus),
               s.Translate(p_1),
               s.Translate(p_tab),
               s.Translate(p_caps),
               s.Translate(p_shift),
               s.Translate(p_q),
               s.Translate(p_a),
               s.Translate(p_z),
               s.Translate(p_tilda),
               s.Translate(p_slash),
               s.Translate(p_2),
               s.Translate(p_w),
               s.Translate(p_s),
               s.Translate(p_x),
               s.Translate(p_5),
               s.Translate(p_t),
               s.Translate(p_g),
               s.Translate(p_b),
               s.Translate(p_e),
               s.Translate(p_c),
               s.Translate(p_v),
               s.Translate(p_r),
               s.Translate(p_left_arrow),
               s.Translate(p_right_arrow),
               s.Translate(p_3),
               s.Translate(p_4),
               s.Translate(p_d),
               s.Translate(p_f),

               s2.Translate(pp_d),
               s2.Translate(pp_e),
               s2.Translate(pp_3),
               s2.Translate(pp_c),
               s2.Translate(pp_s),
               s2.Translate(pp_f),
               s2.Translate(pp_g),
               s2.Translate(pp_a),
               s2.Translate(pp_caps),
               s2.Translate(pp_left_arrow));
}

/*
double r = 50;
std::vector<Shape> arcs;
for (int i = 0; i < 5; ++i) {
  Shape ss = Sphere(.5, 20);
  Key up1 = GetRotatedKey(18, r, true);
  Key up2 = GetRotatedKey(36, r, true);
  Key down1 = GetRotatedKey(18, r, false);

  Shape arc = Union(Hull(ss, up1.GetTransforms().Apply(ss)),
                    Hull(up1.GetTransforms().Apply(ss), up2.GetTransforms().Apply(ss)),
                    Hull(down1.GetTransforms().Apply(ss), ss))
                  .RotateY(90);
  arcs.push_back(arc.Projection().LinearExtrude(1.5).TranslateX(i * 10));
  r += 5;
}
 UnionAll(arcs).WriteToFile("measure.scad");
*/

/*
Key base;
base.t().ry = 10;
Key other;
other.Configure([&](Key& k) {
  k.SetParent(base);
  k.SetPosition(-22.29, 4, -3.72);
});

glm::vec3 rel = other.GetTransforms().Apply(kOrigin);
printf("k.SetPosition(%.3f, %.3f, %.3f);\n", rel.x, rel.y, rel.z);
*/
