#include <functional>
#include <vector>

#include "util.h"

using namespace scad;

// Default spacing between keys. 19mm seems to be the most common spacing in boards.
constexpr double kDefaultKeySpacing = 19;
constexpr bool kShowCaps = false;

int main() {
  // The keys are indexed with 0 being the bottom row, 1 the home row, 2 the row above etc.
  // Everything is centered around the middle finger column and all keys are ultimately relative to
  // the d key (based on left hand).

  // Main keys. These are the keys for the ring - index fingers. Pinky and thumb are handled
  // separately.
  Key middle_1;
  middle_1.t().z += 15;

  Key top_template{0, kDefaultKeySpacing - 1.4, 2.4};
  top_template.t().rx = 12;
  top_template.type = KeyType::SA;

  Key bottom_template{0, -1 * (kDefaultKeySpacing - 2.4), 1};
  bottom_template.t().rx = -18;
  bottom_template.type = KeyType::SA;

  Key index_1;
  index_1.Configure([&](Key& k) {
    k.SetParent(middle_1);
    k.SetPosition(kDefaultKeySpacing, -2.7, 5.1);
    k.t().ry = -5;

    k.t().y -= 3;
    k.t().x -= .4;
  });

  Key index_2 = top_template;
  index_2.Configure([&](Key& k) { k.SetParent(index_1); });

  Key index_0 = bottom_template;
  index_0.Configure([&](Key& k) { k.SetParent(index_1); });

  // This is the inner column - g
  Key index_inner_1;
  index_inner_1.Configure([&](Key& k) {
    k.SetParent(index_1);
    k.SetPosition(kDefaultKeySpacing, -1, .8);
    k.t().ry = -5;

    k.t().x -= .4;
    k.t().z += 2;
  });

  Key index_inner_2 = top_template;
  index_inner_2.Configure([&](Key& k) {
    k.SetParent(index_inner_1);
    k.t().ry += -2;
    k.t().rx += 2;
    k.t().x += -.95;
    k.t().z += 1.6;
    k.t().y += -1.2;
  });

  // This is the s column.
  Key ring_1;
  ring_1.Configure([&](Key& k) {
    k.SetParent(middle_1);
    k.SetPosition(-1 * (kDefaultKeySpacing), -1, 2);
    k.t().ry = 3;

    k.t().y -= 4;
    k.t().z -= 2;
  });

  Key ring_outer_1;
  ring_outer_1.Configure([&](Key& k) {
    k.SetParent(ring_1);
    k.SetPosition(-1 * (kDefaultKeySpacing), -1, 1);
    k.t().ry = 4;
  });

  std::vector<Key*> main_keys = {&index_1,
                                 &index_2,
                                 &index_0,
                                 &index_inner_1,
                                 &index_inner_2,
                                 &middle_1,
                                 &ring_1,
                                 &ring_outer_1};

  main_keys = {&index_1, &index_2, &index_0, &index_inner_1, &index_inner_2, &middle_1};

  std::vector<Shape> main_shapes;
  for (Key* key : main_keys) {
    key->add_side_nub = false;
    key->extra_z = 4;
    main_shapes.push_back(key->GetSwitch());
    if (kShowCaps) {
      main_shapes.push_back(key->GetCap(true).Color("red"));
    }
  }

  UnionAll(main_shapes).WriteToFile("main.scad");
}

void DropWalls() {
  /*
  // Drop the switch walls down.
  Shape connector = Cube(2);
  double connector_offset = -1;
  auto corners = key->GetCorners(connector_offset);
  for (int i = 0; i < 4; ++i) {
    TransformList t = corners[i];
    TransformList t_next = corners[(i + 1) % 4];

    // Shapes on ground.
    glm::vec3 ground = t.Apply(kOrigin);
    ground.z = 0;
    glm::vec3 ground_next = t_next.Apply(kOrigin);
    ground_next.z = 0;

    main_shapes.push_back(Hull(connector.Translate(ground).TranslateZ(1),
                               connector.Translate(ground_next).TranslateZ(1),
                               t.Apply(connector.TranslateZ(-3)),
                               t_next.Apply(connector.TranslateZ(-3))));
  }
  */
}
