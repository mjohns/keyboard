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

class TransformList {
 public:
  Shape Apply(const Shape& shape) const;
  glm::vec3 Apply(const glm::vec3& p) const;

  Transform& AddTransform(Transform t = {}) {
    transforms_.push_back(t);
    return transforms_.back();
  }

  bool empty() const {
    return transforms_.empty();
  }

  Transform& mutable_front() {
    if (empty()) {
      return AddTransform();
    }
    return transforms_.front();
  }

  TransformList& RotateX(float deg) {
    Transform& t = AddTransform();
    t.rx = deg;
    return *this;
  }

  TransformList& RotateY(float deg) {
    Transform& t = AddTransform();
    t.ry = deg;
    return *this;
  }

  TransformList& RotateZ(float deg) {
    Transform& t = AddTransform();
    t.rz = deg;
    return *this;
  }

  TransformList& Translate(float x, float y, float z) {
    AddTransform({x, y, z});
    return *this;
  }

  TransformList& Translate(const glm::vec3& v) {
    return Translate(v.x, v.y, v.z);
  }

  TransformList& TranslateX(float x) {
    return Translate(x, 0, 0);
  }

  TransformList& TranslateY(float y) {
    return Translate(0, y, 0);
  }

  TransformList& TranslateZ(float z) {
    return Translate(0, 0, z);
  }

  TransformList& Transform(const TransformList& other) {
    transforms_.insert(transforms_.end(), other.transforms_.begin(), other.transforms_.end());
  }

 private:
  std::vector<Transform> transforms_;
};

}  // namespace kb
