#include <glm/glm.hpp>
#include <iostream>

#include "util/scad.h"
#include "util/transform.h"

using std::cout;
using std::endl;

using namespace kb;

int main() {
  TransformList transforms;

  Transform& t = transforms.AddTransform();
  t.rx = 45;

  t = transforms.AddTransform();
  t.z = 10;

  t = transforms.AddTransform();
  t.rz = 45;

  Shape sphere = Sphere(0.2);
  transforms.ApplyToShape(sphere).WriteToFile("/tmp/transform_test.scad");
}
