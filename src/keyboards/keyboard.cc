#include <vector>

#include "util/key.h"
#include "util/scad.h"
#include "util/transform.h"

using namespace kb;

int main() {
  Key middle;
  middle.t.x = 19;
  middle.t.y = 19;
  middle.t.ry = 45;
  middle.t.rz = 45;

  Key left(19, 0, 0);
  left.parent = &middle;
  left.t.ry = 20;

  Shape first = Union(middle.GetCap().Color("blue"),
                      left.GetCap().Color("yellow"),
                      middle.GetBottomLeft().Apply(GetConnector()),
                      middle.GetTopLeft().Apply(GetConnector()));

  std::vector<Transform> transforms;

  Transform m;
  m.x = 19;
  m.ry = 45;
  m.rz = 45;

  Transform l;
  l.x = 19;
  l.ry = 20;

  transforms.push_back(m);

  std::vector<Shape> shapes;
  shapes.push_back(ApplyTransforms(Key().GetCap(), transforms).Color("red"));

  transforms.insert(transforms.begin(), l);
  shapes.push_back(ApplyTransforms(Key().GetCap(), transforms).Color("green"));

  shapes.push_back(first);
  UnionAll(shapes).WriteToFile("/tmp/simple.scad");
}
