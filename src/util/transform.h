#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "util/scad.h"

namespace kb {

struct Transform {
 public:
  float x = 0;
  float y = 0;
  float z = 0;

  // Rotation in degrees
  float rx = 0;
  float ry = 0;
  float rz = 0;

  Transform() {
  }

  Transform(double px, double py, double pz) {
    x = px;
    y = py;
    z = pz;
  }

  Transform(const glm::vec3& t) {
    x = t.x;
    y = t.y;
    z = t.z;
  }

  glm::vec3 translation() const {
    return glm::vec3(x, y, z);
  }

  Shape Apply(const Shape& shape) const {
    return shape.RotateZ(rz).RotateX(rx).RotateY(ry).Translate(x, y, z);
  }

  glm::vec3 Apply(const glm::vec3& p) const;
};

Shape ApplyTransforms(Shape shape, const std::vector<Transform>& transforms);

glm::vec3 ApplyTransforms(glm::vec3 point, const std::vector<Transform>& transforms);

}  // namespace kb
