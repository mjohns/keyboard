#include <vector>

#include "util/key.h"
#include "util/scad.h"
#include "util/transform.h"

using namespace kb;

const double kStride = 19;
const double kNegStride = -1 * kStride;

int main() {
  std::vector<Key*> keys;

  Key m2;
  keys.push_back(&m2);

  Key middle;
  keys.push_back(&middle);
  middle.t().ry = 45;
  middle.t().rz = 45;

  Key left(19, 0, 0);
  left.parent = &middle;
  left.t().ry = 20;


  std::vector<Shape> shapes;
  shapes.push_back(first);
  UnionAll(shapes).WriteToFile("/tmp/simple.scad");
}
