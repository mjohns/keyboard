#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#if defined(__GNUC__) || defined(__GNUG__)
#define SCAD_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#define SCAD_WARN_UNUSED_RESULT
#endif

namespace scad {

using ScadWriter = std::function<void(std::FILE* file, int indent_level)>;

template <typename T>
class Optional {
 public:
  Optional() {
  }
  Optional(T value) : value_(value), has_value_(true) {
  }

  T value() const {
    return value_;
  }

  bool has_value() const {
    return has_value_;
  }

 private:
  T value_;
  bool has_value_ = false;
};

struct LinearExtrudeParams {
  double height = 0;
  double twist = 0;
  double convexity = 10;
  int slices = 20;
  double scale = 1;
  bool center = true;
};

struct Vec3 {
  double x = 0;
  double y = 0;
  double z = 0;
};

class Shape {
 public:
  Shape() {
  }
  explicit Shape(std::shared_ptr<ScadWriter> scad) : scad_(std::move(scad)) {
  }
  explicit Shape(ScadWriter scad) : scad_(std::make_shared<ScadWriter>(std::move(scad))) {
  }

  void WriteToFile(const std::string& file_name) const;
  void AppendScad(std::FILE* file, int indent_level) const;

  Shape Translate(double x, double y, double z) const SCAD_WARN_UNUSED_RESULT;
  Shape TranslateX(double x) const SCAD_WARN_UNUSED_RESULT;
  Shape TranslateY(double y) const SCAD_WARN_UNUSED_RESULT;
  Shape TranslateZ(double z) const SCAD_WARN_UNUSED_RESULT;
  Shape Translate(const Vec3& v) const SCAD_WARN_UNUSED_RESULT;

  Shape Mirror(double x, double y, double z) const SCAD_WARN_UNUSED_RESULT;
  Shape Mirror(const Vec3& v) const SCAD_WARN_UNUSED_RESULT;

  Shape Rotate(double degrees, double x, double y, double z) const SCAD_WARN_UNUSED_RESULT;
  Shape RotateX(double degrees) const SCAD_WARN_UNUSED_RESULT;
  Shape RotateY(double degrees) const SCAD_WARN_UNUSED_RESULT;
  Shape RotateZ(double degrees) const SCAD_WARN_UNUSED_RESULT;

  Shape LinearExtrude(const LinearExtrudeParams& params) const SCAD_WARN_UNUSED_RESULT;
  Shape LinearExtrude(double height) const SCAD_WARN_UNUSED_RESULT;

  Shape Color(double r, double g, double b, double a = 1.0) const SCAD_WARN_UNUSED_RESULT;
  Shape Color(const std::string& color, double a = 1) const SCAD_WARN_UNUSED_RESULT;
  Shape Alpha(double a) const SCAD_WARN_UNUSED_RESULT;

  Shape Subtract(const Shape& other) const SCAD_WARN_UNUSED_RESULT;
  Shape operator-(const Shape& other) const SCAD_WARN_UNUSED_RESULT;
  Shape& operator-=(const Shape& other);

  Shape Add(const Shape& other) const SCAD_WARN_UNUSED_RESULT;
  Shape operator+(const Shape& other) const SCAD_WARN_UNUSED_RESULT;
  Shape& operator+=(const Shape& other);

  Shape Scale(double x, double y, double z) const SCAD_WARN_UNUSED_RESULT;
  Shape Scale(double s) const SCAD_WARN_UNUSED_RESULT;

  Shape OffsetRadius(double r, bool chamfer = false) const SCAD_WARN_UNUSED_RESULT;
  Shape OffsetDelta(double delta, bool chamfer = false) const SCAD_WARN_UNUSED_RESULT;

  Shape Comment(const std::string& comment) const SCAD_WARN_UNUSED_RESULT;

  Shape Projection(bool cut = false) const SCAD_WARN_UNUSED_RESULT;

 private:
  std::shared_ptr<const ScadWriter> scad_;
};

using Transform = std::function<Shape(const Shape&)>;

struct CubeParams {
  double x = 1;
  double y = 1;
  double z = 1;
  bool center = true;
};
Shape Cube(const CubeParams& params) SCAD_WARN_UNUSED_RESULT;
Shape Cube(double x, double y, double z, bool center = true) SCAD_WARN_UNUSED_RESULT;
Shape Cube(double size, bool center = true) SCAD_WARN_UNUSED_RESULT;

struct SphereParams {
  double r = 1;
  Optional<double> fn;
  Optional<double> fa;
  Optional<double> fs;
};
Shape Sphere(const SphereParams& params) SCAD_WARN_UNUSED_RESULT;
Shape Sphere(double radius) SCAD_WARN_UNUSED_RESULT;

struct CircleParams {
  double r = 1;
  Optional<double> fn;
  Optional<double> fa;
  Optional<double> fs;
};
Shape Circle(const CircleParams& params) SCAD_WARN_UNUSED_RESULT;
Shape Circle(double radius) SCAD_WARN_UNUSED_RESULT;

struct CylinderParams {
  double h = 1;
  double r1 = 1;
  double r2 = 1;
  Optional<double> fn;
  bool center = true;
};
Shape Cylinder(const CylinderParams& params) SCAD_WARN_UNUSED_RESULT;
Shape Cylinder(double height, double radius, Optional<double> fn = {}) SCAD_WARN_UNUSED_RESULT;

struct SquareParams {
  double x = 1;
  double y = 1;
  bool center = true;
};
Shape Square(const CubeParams& params) SCAD_WARN_UNUSED_RESULT;
Shape Square(double x, double y, bool center = true) SCAD_WARN_UNUSED_RESULT;
Shape Square(double size, bool center = true) SCAD_WARN_UNUSED_RESULT;

struct Point2d {
  double x = 0;
  double y = 0;
};
Shape Polygon(const std::vector<Point2d>& points) SCAD_WARN_UNUSED_RESULT;

Shape HullAll(const std::vector<Shape>& shapes) SCAD_WARN_UNUSED_RESULT;

template <typename... Shapes>
Shape Hull(const Shape& shape, const Shapes&... more_shapes) {
  return HullAll({shape, more_shapes...});
}

Shape UnionAll(const std::vector<Shape>& shapes) SCAD_WARN_UNUSED_RESULT;

template <typename... Shapes>
Shape Union(const Shape& shape, const Shapes&... more_shapes) {
  return UnionAll({shape, more_shapes...});
}

Shape DifferenceAll(const std::vector<Shape>& shapes) SCAD_WARN_UNUSED_RESULT;

template <typename... Shapes>
Shape Difference(const Shape& shape, const Shapes&... more_shapes) {
  return DifferenceAll({shape, more_shapes...});
}

Shape IntersectionAll(const std::vector<Shape>& shapes) SCAD_WARN_UNUSED_RESULT;

template <typename... Shapes>
Shape Intersection(const Shape& shape, const Shapes&... more_shapes) {
  return IntersectionAll({shape, more_shapes...});
}

Shape Import(const std::string& file_name, int convexity = -1) SCAD_WARN_UNUSED_RESULT;

Shape Minkowski(const Shape& first, const Shape& second) SCAD_WARN_UNUSED_RESULT;

}  // namespace scad
