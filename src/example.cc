#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "key.h"
#include "scad.h"
#include "transform.h"

using namespace scad;

int main() {
  Key k;
  k.GetSwitch().WriteToFile("key.scad");
  return 0;
}
