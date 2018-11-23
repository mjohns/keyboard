#include "frame.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace kb {

glm::vec3 Rotate(
    const glm::vec3& p, double deg, double x, double y, double z) {
  glm::mat4 rotation(1.0f);
  rotation = glm::rotate(rotation, glm::radians(deg), glm::vec3(x, y, z));
  glm::vec4 rotated = rotation * glm::vec4(p.x, p.y, p.z, 1);
  return glm::vec3(rotated.x, rotated.y, rotated.z);
}

glm::vec3 RotateX(const glm::vec3& p, double deg) {
  return Rotate(p, deg, 1, 0, 0);
}

glm::vec3 RotateY(const glm::vec3& p, double deg) {
  return Rotate(p, deg, 0, 1, 0);
}

glm::vec3 RotateZ(const glm::vec3& p, double deg) {
  return Rotate(p, deg, 0, 0, 1);
}

glm::vec3 Frame::PlacePoint(const glm::vec3& p) {
  glm::vec3 result = p;
  result = RotateX(result, rx);
  result = RotateY(result, ry);
  result = RotateZ(result, rz);
  return result + origin();
}

Frame PlaceFrame(const Frame& base, const Frame& frame) {
  glm::vec3 p = frame.origin();
  p = RotateZ(p, base.rz);
  p = RotateX(p, base.rx);
  p = RotateY(p, base.ry);

  Frame result = base;
  result.SetOrigin(result.origin() + p);
  result.rx += frame.rx;
  result.ry += frame.ry;
  result.rz += frame.rz;
  return result;
}

}  // namespace kb
