#include "util/transform.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "util/scad.h"

namespace kb {

glm::vec3 Transform::Apply(const glm::vec3& p) const {
  glm::mat4 transform(1.0f);
  transform = glm::rotate(transform, glm::radians(rx), glm::vec3(1, 0, 0));
  transform = glm::rotate(transform, glm::radians(ry), glm::vec3(0, 1, 0));
  transform = glm::rotate(transform, glm::radians(rz), glm::vec3(0, 0, 1));
  transform = glm::translate(transform, translation());

  glm::vec4 transformed = transform * glm::vec4(p.x, p.y, p.z, 1);
  return glm::vec3(transformed.x, transformed.y, transformed.z);
}

Shape ApplyTransforms(Shape shape, const std::vector<Transform>& transforms) {
  for (auto& transform : transforms) {
    shape = transform.Apply(shape);
  }
  return shape;
}

glm::vec3 ApplyTransforms(glm::vec3 point, const std::vector<Transform>& transforms) {
  for (auto& transform : transforms) {
    point = transform.Apply(point);
  }
  return point;
}

}  // namespace kb
