#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "util.h"

using namespace scad;

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

  Shape s = Sphere(1, 30);


  Union(
      s.Translate(p_backspace),
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
      s.Translate(p_d).Color("green"),
      s.Translate(p_f).Color("green")).WriteToFile("measure.scad");

}
