#include <glm/glm.hpp>
#include <iostream>

#include "util/scad.h"
#include "util/transform.h"

using std::cout;
using std::endl;

using namespace kb;

int main() {
  Transform t;
  t.z = 2;
  t.rx = 90;

  glm::vec3 point = t.ApplyToPoint(glm::vec3(0, 0, 1));
  cout << "x: " << point.x << " y: " << point.y << " z: " << point.z << endl;

  Shape sphere = Sphere(0.2);
  t.ApplyToShape(sphere).WriteToFile("/tmp/transform_test.scad");
}
