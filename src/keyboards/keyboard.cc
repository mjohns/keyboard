#include "util/key.h"
#include "util/scad.h"

using namespace kb;

int main() {
  Key middle(4, -4, 0);

  Key left(-19, 0, 0);
  left.parent = &middle;
  left.f.ry = 10;

  Union(middle.GetSwitch(),
        left.GetSwitch(),
        middle.GetBottomLeft().PlaceShape(GetConnector()),
        middle.GetTopLeft().PlaceShape(GetConnector()))
      .WriteToFile("/tmp/simple.scad");
}
