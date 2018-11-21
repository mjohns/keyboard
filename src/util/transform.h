#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util/scad.h"

namespace kb {

// A transformation of a point or scad shape. First the rotations are applied and then the point
// or shape is translated away from the origin.
class Transform {
 public:
  float x = 0;
  float y = 0;
  float z = 0;

  // Rotation in degrees
  float rx = 0;
  float ry = 0;
  float rz = 0;

  Shape ApplyToShape(const kb::Shape& in) {
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
