#pragma once

#include <glm/glm.hpp>

#include "util/scad.h"

namespace kb {

glm::vec3 Rotate(const glm::vec3& p, double deg, double x, double y, double z);
glm::vec3 RotateX(const glm::vec3& p, double deg);
glm::vec3 RotateY(const glm::vec3& p, double deg);
glm::vec3 RotateZ(const glm::vec3& p, double deg);

struct Frame {
 public:
  double x = 0;
  double y = 0;
  double z = 0;

  // Rotation in degrees
  double rx = 0;
  double ry = 0;
  double rz = 0;

  Frame(double px, double py, double pz) : x(px), y(py), z(pz) {
  }

  glm::vec3 origin() const {
    return glm::vec3(x, y, z);
  }

  void SetOrigin(const glm::vec3& origin) {
    x = origin.x;
    y = origin.y;
    z = origin.z;
  }

  Shape PlaceShape(const Shape& in) {
    return in.RotateZ(rz).RotateX(rx).RotateY(ry).Translate(x, y, z);
  }

  glm::vec3 PlacePoint(const glm::vec3& p);
};

Frame PlaceFrame(const Frame& base, const Frame& frame);

}  // namespace kb
