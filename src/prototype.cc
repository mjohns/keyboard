#include <functional>
#include <vector>

#include "util.h"

using namespace scad;

// Default spacing between keys. 19mm seems to be the most common spacing in boards.
constexpr double kDefaultKeySpacing = 19;
constexpr bool kShowCaps = false;
constexpr bool kIncludeTestSupport = false;

Shape CreatePosts(const std::vector<Key*> keys);
Shape CreateCheapRaft(const std::vector<Key*> keys);

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

    // Feedback - too close move back a little
    k.t().y += .5;  // was 1.2 at last print

    k.AddTransform();
    k.t().z = .3;
  });

  Key main_origin;
  main_origin.Configure([&](Key& k) { k.SetPosition(0, 0, 40 - 5); });

  // Main keys. These are the keys for the ring - index fingers. Pinky and thumb are handled
  // separately.
  Key middle_1;
  middle_1.Configure([&](Key& k) {
    k.name = "middle_1";
    k.SetParent(main_origin);
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
    // Feedback - pending - when adding key below this might be to high/slanted to get past.
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

    // Feedback - Can move higher ~2mm
    k.t().y += 2.5;
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

  Key pinky;
  pinky.Configure([&](Key& k) {
    k.name = "pinky";
    k.SetParent(ring_1);
    k.SetPosition(-21, -22, -13);
    k.t().ry = -6;

    // feedback - higher plus left (little)
    k.t().z += 3;
    k.t().x -= 3;

  });

  // Thumb
  // Naming based on kinesis key layout
  // Backspace
  Key thumb;
  thumb.Configure([&](Key& k) {
    k.name = "thumb";
    k.SetParent(middle_1);
    k.SetPosition(58, -58, 8);
    k.t().rz = -20;
    k.t().rx = 10;
    k.t().ry = 10;
    //k.extra_width_left = 4;

    // Feedback move left x - tilt more vertical. less z rotation
    k.t().rz = -15;
    k.t().rx = 12;
    k.t().ry = 5;
    k.t().x -= 8;
    k.t().y += 6;
  });

  // Second thumb key.
  Key thumb_delete;
  thumb_delete.Configure([&](Key& k) {
    k.name = "thumb_delete";
    k.SetParent(thumb);
    k.SetPosition(kDefaultKeySpacing, 0, 0);
  });

  // Bottom side key.
  Key thumb_end;
  thumb_end.Configure([&](Key& k) {
    k.name = "thumb_end";
    k.SetParent(thumb_delete);
    k.SetPosition(kDefaultKeySpacing, -9, 0);
  });

  // Middle side key.
  Key thumb_home;
  thumb_home.Configure([&](Key& k) {
    k.name = "thumb_home";
    k.SetParent(thumb_delete);
    k.SetPosition(kDefaultKeySpacing, 10, 0);
  });

  // Top side key;
  Key thumb_alt;
  thumb_alt.Configure([&](Key& k) {
    k.name = "thumb_alt";
    k.SetParent(thumb_delete);
    k.SetPosition(kDefaultKeySpacing, 10 + kDefaultKeySpacing, 0);
  });

  // Top left key.
  Key thumb_ctrl;
  thumb_ctrl.Configure([&](Key& k) {
    k.name = "thumb_ctrl";
    k.SetParent(thumb_delete);
    k.SetPosition(0, 10 + kDefaultKeySpacing, 0);
  });

  std::vector<Key*> thumb_keys = {
      &thumb, &thumb_delete, &thumb_end, &thumb_home, &thumb_alt, &thumb_ctrl};

  // just like kinesis
  // distance 73.5, x = 40,

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
      &pinky,
  };

  // std::vector<Key*> keys_to_print = {&ring_1, &middle_1, &pinky, &ring_outer_1, &ring_0};
  std::vector<Key*> keys_to_print = {
      &ring_1, &middle_1, &pinky, &index_1, &index_inner_0, &thumb, &index_inner_1};
  keys_to_print = {&ring_1, &middle_1, &index_1, &thumb, &thumb_delete,
      &index_inner_0, &pinky
  };
  //PushBackAll(&keys_to_print, thumb_keys);
  // keys_to_print = main_keys;

  std::vector<Shape> shapes;
  for (Key* key : keys_to_print) {
    key->add_side_nub = false;
    key->extra_z = 4;
    shapes.push_back(key->GetSwitch());
    if (kShowCaps) {
      shapes.push_back(key->GetCap(true).Color("red"));
    }
  }

  if (kIncludeTestSupport) {
    shapes.push_back(CreateCheapRaft(keys_to_print));
    shapes.push_back(CreatePosts(keys_to_print));
  }
  UnionAll(shapes).Subtract(Union(thumb.GetInverseSwitch(), index_inner_0.GetInverseSwitch())).WriteToFile("main.scad");

  /*
  std::vector<Shape> t_shapes;
  std::vector<Shape> t_holes;
  for (Key* k : thumb_keys) {
    k->add_side_nub = false;
    t_shapes.push_back(k->GetSwitch());
    t_holes.push_back(k->GetInverseSwitch());
  }
  HullAll(t_shapes).Subtract(UnionAll(t_holes)).WriteToFile("thumb_plate.scad");
  */

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

Shape CreatePosts(const std::vector<Key*> keys) {
  std::vector<Shape> shapes;
  Shape connector = Cube(2, 2, .001).TranslateZ(-4);
  for (Key* k : keys) {
    for (auto t : k->GetCorners(-1)) {
      Shape top = t.Apply(connector);
      shapes.push_back(Hull(top, top.Projection().LinearExtrude(1).TranslateZ(.5)));
    }
  }
  return UnionAll(shapes);
}

// Create a square grid on the ground enclosing all the keys.
Shape CreateCheapRaft(const std::vector<Key*> keys) {
  std::vector<TransformList> points;
  for (Key* k : keys) {
    PushBackAll(&points, k->GetCorners(-1));
  }

  std::vector<Shape> shapes;
  Shape c = Cube(1).TranslateZ(.5);
  const glm::vec3 offset(0, 0, -4);
  for (TransformList t : points) {
    glm::vec3 p = t.Apply(offset);
    p.z = 0;
    for (TransformList other_t : points) {
      glm::vec3 other_p = other_t.Apply(offset);
      other_p.z = 0;
      float distance = glm::length(other_p - p);
      if (distance > 1 && distance < 22) {
        shapes.push_back(Hull(c.Translate(p), c.Translate(other_p)));
      }
    }
  }
  return UnionAll(shapes);
}
