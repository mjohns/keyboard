#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util/scad.h"

namespace kb {

glm::vec3 Rotate(const glm::vec3& p, double deg, double x, double y, double z) {
  glm::mat4 rotation(1.0f);
  rotation = glm::rotate(rotation, glm::radians(deg), glm::vec3(x, y, z));
  glm::vec4 rotated = rotation * glm::vec4(p.x, p.y, p.z, 1);
  return glm::vec3(rotated.x, rotated.y, rotated.z);
}

gmtl::Point3d RotateX(const glm::vec3& p, double deg) {
  return Rotate(p, deg, 1, 0, 0);
}

gmtl::Point3d RotateY(const glm::vec3& p, double deg) {
  return Rotate(p, deg, 0, 1, 0);
}

gmtl::Point3d RotateZ(const glm::vec3& p, double deg) {
  return Rotate(p, deg, 0, 0, 1);
}

struct Frame {
 public:
  double x = 0;
  double y = 0;
  double z = 0;

  // Rotation in degrees
  double rx = 0;
  double ry = 0;
  double rz = 0;

  Frame(double px, double py, double pz) {
    x = px;
    y = py;
    z = pz;
  }

  glm::vec3 origin() const {
    return glm::vec3(x, y, z);
  }

  Shape ApplyToShape(const Shape& in) {
    return in.RotateZ(rz).RotateX(rx).RotateY(ry).Translate(x, y, z);
  }

  glm::vec3 ApplyToPoint(const glm::vec3& p) {
    glm::mat4 rotation(1.0f);
    rotation = glm::rotate(rotation, glm::radians(rx), glm::vec3(1, 0, 0));
    rotation = glm::rotate(rotation, glm::radians(ry), glm::vec3(0, 1, 0));
    rotation = glm::rotate(rotation, glm::radians(rz), glm::vec3(0, 0, 1));

    glm::vec4 rotated4 = rotation * glm::vec4(p.x, p.y, p.z, 1);
    glm::vec3 rotated(rotated4.x, rotated4.y, rotated4.z);
    return rotated + glm::vec3(x, y, z);
  }

  glm::vec3 ApplyToPoint() {
    return ApplyToPoint(glm::vec3(0, 0, 0));
  }
};

}  // namespace kb
