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

  Key top_template{0, kDefaultKeySpacing - 1.4, 2.4};
  top_template.t().rx = 12;
  top_template.type = KeyType::SA;

  Key bottom_template{0, -1 * (kDefaultKeySpacing - 2.4), 1};
  bottom_template.Configure([&](Key& k) {
    k.t().rx = -18;
    k.type = KeyType::SA;

    // angle it more and move down -- changes
    k.t().rx += -2;
    k.t().z += -1;
  });

  // Template for number row keys.
  Key top_top_template{0, kDefaultKeySpacing - 2.5, 2.4};
  top_top_template.Configure([&](Key& k) {
    k.t().rx = 19;
    k.type = KeyType::SA_TALL_EDGE;
    k.sa_edge_type = SaEdgeType::BOTTOM;

    k.AddTransform();
    k.t().z = .3;
  });

  // Main keys. These are the keys for the ring - index fingers. Pinky and thumb are handled
  // separately.
  Key middle_1;
  middle_1.Configure([&](Key& k) {
    k.name = "middle_1";
    k.t().z += 15;
  });

  Key middle_2 = top_template;
  middle_2.Configure([&](Key& k) {
    k.name = "middle_2";
    k.SetParent(middle_1);
    k.t().z += .4;

    // Reset transform so now adding z will bring it forward relative to its no
    // rotational orientation.
    k.AddTransform();
    k.t().z += .3;
  });

  Key middle_3 = top_top_template;
  middle_3.Configure([&](Key& k) {
    k.name = "middle_3";
    k.SetParent(middle_2);
  });

  Key middle_0 = bottom_template;
  middle_0.Configure([&](Key& k) {
    k.name = "middle_0";
    k.SetParent(middle_1);
  });

  Key index_1;
  index_1.Configure([&](Key& k) {
    k.name = "index_1";
    k.SetParent(middle_1);
    k.SetPosition(kDefaultKeySpacing, -2.7, 5.1);
    k.t().ry = -5;

    k.t().y -= 3;
    k.t().x -= .4;
  });

  Key index_2 = top_template;
  index_2.Configure([&](Key& k) {
    k.name = "index_2";
    k.SetParent(index_1);
    k.t().z += .4;

    // Reset transform so now adding z will bring it forward relative to its no
    // rotational orientation.
    k.AddTransform();
    k.t().z += .3;
  });

  Key index_0 = bottom_template;
  index_0.Configure([&](Key& k) {
    k.name = "index_0";
    k.SetParent(index_1);
  });

  // This is the inner column - g
  Key index_inner_1;
  index_inner_1.Configure([&](Key& k) {
    k.name = "index_inner_1";
    k.type = KeyType::SA;
    k.SetParent(index_1);
    k.SetPosition(kDefaultKeySpacing, -1, .8);
    k.t().ry = -7;

    k.t().x -= .6;
    k.t().z += 2;
  });

  Key index_inner_2 = top_template;
  index_inner_2.Configure([&](Key& k) {
    k.name = "index_inner_2";
    k.SetParent(index_inner_1);
    k.t().rx += 2;
    k.t().x += -.75;
    k.t().z += 1.6;
    k.t().y += -1.2;
  });

  Key index_inner_0 = bottom_template;
  index_inner_0.Configure([&](Key& k) {
    k.name = "index_inner_0";
    k.SetParent(index_inner_1);
    k.t().rx += 2;
    k.t().x += -.75;
    // k.t().z += 1;
    k.t().y += 1;
  });

  // Final customization of index_inner_1 without moving children.
  index_inner_1.Configure([&](Key& k) { k.t().x += -.5; });

  // This is the s column.
  Key ring_1;
  ring_1.Configure([&](Key& k) {
    k.name = "ring_1";
    k.SetParent(middle_1);
    k.SetPosition(-1 * (kDefaultKeySpacing), -1, 2);
    k.t().ry = 3;

    k.t().y -= 4;
    k.t().z -= 2;
  });

  Key ring_2 = top_template;
  ring_2.Configure([&](Key& k) {
    k.name = "ring_2";
    k.SetParent(ring_1);
    k.t().z += .4;

    // Reset transform so now adding z will bring it forward relative to its no
    // rotational orientation.
    k.AddTransform();
    k.t().z += .3;
  });

  Key ring_0 = bottom_template;
  ring_0.Configure([&](Key& k) {
    k.name = "ring_0";
    k.SetParent(ring_1);
  });

  Key ring_outer_1;
  ring_outer_1.Configure([&](Key& k) {
    k.name = "ring_outer_1";
    k.type = KeyType::SA;
    k.SetParent(ring_1);
    k.SetPosition(-1 * (kDefaultKeySpacing), 1.4, 1);
    k.t().ry = 4;
    k.t().x += .3;
  });

  Key ring_outer_2 = top_template;
  ring_outer_2.Configure([&](Key& k) {
    k.name = "ring_outer_2";
    k.SetParent(ring_outer_1);
    k.t().rx += -2;
    k.t().x += .4;
    k.t().z += 1.6;
    k.t().y += -1.2;
  });

  std::vector<Key*> main_keys = {
      &index_1,
      &index_2,
      &index_0,
      &index_inner_1,
      &index_inner_2,
      &index_inner_0,
      &middle_0,
      &middle_1,
      &middle_2,
      &middle_3,
      &ring_0,
      &ring_1,
      &ring_2,
      &ring_outer_1,
      &ring_outer_2,
  };

  std::vector<Key*> keys_to_print = {&middle_1, &middle_2, &middle_3};
  //keys_to_print = main_keys;

  std::vector<Shape> shapes;
  for (Key* key : keys_to_print) {
    key->add_side_nub = false;
    key->extra_z = 4;
    shapes.push_back(key->GetSwitch());
    if (kShowCaps) {
      shapes.push_back(key->GetCap(true).Color("red"));
    }
  }

  UnionAll(shapes).WriteToFile("main.scad");

  // Print out distances between close keys.
  for (Key* key : main_keys) {
    glm::vec3 v1 = key->GetTransforms().Apply(kOrigin);
    for (Key* other_key : main_keys) {
      if (key != other_key) {
        glm::vec3 v2 = other_key->GetTransforms().Apply(kOrigin);
        float length = glm::length(v2 - v1);
        if (length < 20) {
          printf(
              "Distance from %s -> %s: %.1f\n", key->name.c_str(), other_key->name.c_str(), length);
        }
      }
    }
  }
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
