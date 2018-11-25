#include <vector>

#include "util/key.h"
#include "util/scad.h"
#include "util/transform.h"

using namespace kb;

// const double kStride = 19;
// const double kNegStride = -1 * kStride;

int main() {
  std::vector<Key*> keys;

  double radius = 50;
  Key top_template;
  top_template.local_transform.TranslateZ(-1 * radius).RotateX(15).TranslateZ(radius);


  Key m2;
  keys.push_back(&m2);

  Key t2 = top_template;
  t2.parent = &m2;

  std::vector<Shape> shapes;
  shapes.push_back(m2.GetCap());
  shapes.push_back(t2.GetCap());
  UnionAll(shapes).WriteToFile("/tmp/simple.scad");
}
