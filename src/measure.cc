#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "util.h"

using namespace scad;

constexpr double kDefaultKeySpacing = 19;

std::ostream& operator<<(std::ostream& out, const glm::vec3& vec) {
  out << "{" << vec.x << " " << vec.y << " " << vec.z << "}";
  return out;
}

float FindRadius(glm::vec3 start, glm::vec3 to_center, glm::vec3 other) {
  to_center = glm::normalize(to_center);
  float r = 0.1;
  while (true) {
    glm::vec3 center = start + (to_center * r);
    if (glm::length(center - start) > glm::length(center - other)) {
      return r;
    }
    r += .01;
  }
}

// Rotates a key about the x axis until it has traveled the direct distance (not on the arc).
Key GetRotatedKey(double distance, double radius, bool up) {
  double rotation_direction = up ? 1.0 : -1.0;
  double degrees = .1;
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
    degrees += .1;
  }
}

int main() {
  // std::unordered_map<std::string, glm::vec3> positions;

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

  Shape points = Union(s.Translate(p_backspace),
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
                       s.Translate(p_f));

  Key thumb;
  thumb.Configure([&](Key& k) {
    k.name = "thumb";
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

  Key key_d;
  key_d.Configure([&](Key& k) {
    k.name = "d";
    k.SetPosition(27.45, 52.44, 17.13);
    k.t().ry = -8;
    k.t().rz = -2;
    k.t().rx = 2;
  });

  double d_distance = 18;
  double d_radius = 53;

  Key key_e = GetRotatedKey(d_distance, d_radius, true);
  key_e.Configure([&](Key& k) {
    k.name = "e";
    k.SetParent(key_d);
  });

  Key key_3 = GetRotatedKey(d_distance, d_radius, true);
  key_3.Configure([&](Key& k) {
    k.name = "3";
    k.SetParent(key_e);
  });

  Key key_c = GetRotatedKey(d_distance, d_radius, false);
  key_c.Configure([&](Key& k) {
    k.name = "c";
    k.SetParent(key_d);
  });

  Key key_left_arrow = GetRotatedKey(d_distance, d_radius, false);
  key_left_arrow.Configure([&](Key& k) {
    k.name = "left_arrow";
    k.SetParent(key_c);
  });

  Key key_s;
  key_s.Configure([&](Key& k) {
    k.name = "s";
    k.SetPosition(5.44, 50.72, 17.44);
    k.t().ry = -3;
    k.t().rz = -2;
    k.t().rx = 1;
  });

  double s_distance = 18;
  double s_radius = 70;

  Key key_w = GetRotatedKey(s_distance, s_radius, true);
  key_w.Configure([&](Key& k) {
    k.name = "w";
    k.SetParent(key_s);
  });

  Key key_2 = GetRotatedKey(s_distance, s_radius, true);
  key_2.Configure([&](Key& k) {
    k.name = "2";
    k.SetParent(key_w);
  });

  Key key_x = GetRotatedKey(s_distance, s_radius, false);
  key_x.Configure([&](Key& k) {
    k.name = "x";
    k.SetParent(key_s);
  });

  Key key_slash = GetRotatedKey(s_distance, s_radius, false);
  key_slash.Configure([&](Key& k) {
    k.name = "slash";
    k.SetParent(key_x);
  });

  Key key_f;
  key_f.Configure([&](Key& k) {
    k.name = "f";
    k.SetPosition(43.99, 47.96, 27.15);
    k.t().ry = -19;
    k.t().rz = -2;
    k.t().rx = 0;
  });

  double f_distance = 18;
  double f_radius = 70;

  Key key_r = GetRotatedKey(f_distance, f_radius, true);
  key_r.Configure([&](Key& k) {
    k.name = "r";
    k.SetParent(key_f);
  });

  Key key_4 = GetRotatedKey(f_distance, f_radius, true);
  key_4.Configure([&](Key& k) {
    k.name = "4";
    k.SetParent(key_r);
  });

  Key key_v = GetRotatedKey(f_distance, f_radius, false);
  key_v.Configure([&](Key& k) {
    k.name = "v";
    k.SetParent(key_f);
  });

  Key key_right_arrow = GetRotatedKey(f_distance, f_radius, false);
  key_right_arrow.Configure([&](Key& k) {
    k.name = "right_arrow";
    k.SetParent(key_v);
  });

  Key key_g;
  key_g.Configure([&](Key& k) {
    k.name = "g";
    k.SetPosition(59.63, 46.97, 36.22);
    k.t().ry = -21;
    k.t().rz = -2;
    k.t().rx = 2;
  });

  double g_distance = 18;
  double g_radius = 60;

  Key key_t = GetRotatedKey(g_distance, g_radius, true);
  key_t.Configure([&](Key& k) {
    k.name = "t";
    k.SetParent(key_g);
  });

  Key key_5 = GetRotatedKey(g_distance, g_radius, true);
  key_5.Configure([&](Key& k) {
    k.name = "5";
    k.SetParent(key_t);
  });

  Key key_b = GetRotatedKey(g_distance, g_radius, false);
  key_b.Configure([&](Key& k) {
    k.name = "b";
    k.SetParent(key_g);
  });

  Key key_a;
  key_a.Configure([&](Key& k) {
    k.name = "a";
    k.SetPosition(-15.73, 44.18, 19.37);
    k.t().ry = -4;
    k.t().rz = -2;
    k.t().rx = 0;
  });

  double a_distance = 18;
  double a_radius = 70;

  Key key_q = GetRotatedKey(a_distance, a_radius, true);
  key_q.Configure([&](Key& k) {
    k.name = "q";
    k.SetParent(key_a);
  });

  Key key_1 = GetRotatedKey(a_distance, a_radius, true);
  key_1.Configure([&](Key& k) {
    k.name = "1";
    k.SetParent(key_q);
  });

  Key key_z = GetRotatedKey(a_distance, a_radius, false);
  key_z.Configure([&](Key& k) {
    k.name = "z";
    k.SetParent(key_a);
  });

  Key key_tilda = GetRotatedKey(a_distance, a_radius, false);
  key_tilda.Configure([&](Key& k) {
    k.name = "tilda";
    k.SetParent(key_z);
  });

  Key key_caps;
  key_caps.Configure([&](Key& k) {
    k.name = "caps";
    k.SetPosition(-37.84, 48.09, 15.77);
    k.t().ry = -4;
    k.t().rz = -2;
    k.t().rx = 0;
  });

  double caps_distance = 18;
  double caps_radius = 65;

  Key key_tab = GetRotatedKey(caps_distance, caps_radius, true);
  key_tab.Configure([&](Key& k) {
    k.name = "tab";
    k.SetParent(key_caps);
  });

  Key key_plus = GetRotatedKey(caps_distance, caps_radius, true);
  key_plus.Configure([&](Key& k) {
    k.name = "plus";
    k.SetParent(key_tab);
  });

  Key key_shift = GetRotatedKey(caps_distance, caps_radius, false);
  key_shift.Configure([&](Key& k) {
    k.name = "shift";
    k.SetParent(key_caps);
  });

  std::vector<Key*> keys_to_print = {
    &key_caps, &key_tab, &key_plus, &key_shift,
      &key_d, &key_e,     &key_c, &key_left_arrow, &key_3, &key_s,           &key_w, &key_2,
      &key_x, &key_slash, &key_f, &key_r,          &key_v, &key_right_arrow, &key_4, &key_t,
      &key_5, &key_b,     &key_g, &key_a,          &key_q, &key_1,           &key_z, &key_tilda};
  PushBackAll(&keys_to_print, thumb_keys);
  std::vector<Shape> shapes;
  shapes.push_back(points);
  Shape s2 = Sphere(1, 30).Color("blue", 0.5);
  for (Key* key : keys_to_print) {
    shapes.push_back(key->GetTransforms().Apply(s2));
  }

  UnionAll(shapes).WriteToFile("measure.scad");
}
