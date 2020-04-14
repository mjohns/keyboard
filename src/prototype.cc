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

  Key index_1;
  index_1.Configure([&](Key& k) {
    k.SetParent(middle_1);
    k.SetPosition(kDefaultKeySpacing, -2.7, 5.1);
    k.t().ry = -5;

    k.t().y -= 3;
    k.t().x -= .4;
  });

  // This is the inner column - g
  Key index2_1;
  index2_1.Configure([&](Key& k) {
    k.SetParent(index_1);
    k.SetPosition(kDefaultKeySpacing, -1, .8);
    k.t().ry = -5;

    k.t().x -= .4;
    k.t().z += 2;
  });

  Key ring_1;
  ring_1.Configure([&](Key& k) {
    k.SetParent(middle_1);
    k.SetPosition(-1 * (kDefaultKeySpacing), -1, 2);
    k.t().ry = 3;

    k.t().y -= 4;
    k.t().z -= 2;
  });

  Key ring2_1;
  ring2_1.Configure([&](Key& k) {
    k.SetParent(ring_1);
    k.SetPosition(-1 * (kDefaultKeySpacing), -1, 1);
    k.t().ry = 4;
  });

  std::vector<Key*> main_keys = {&index_1, &index2_1, &middle_1, &ring_1, &ring2_1};

  std::vector<Shape> main_shapes;
  for (Key* key : main_keys) {
    key->add_side_nub = false;
    key->extra_z = 4;
    main_shapes.push_back(key->GetSwitch());
    if (kShowCaps) {
      main_shapes.push_back(key->GetCap(true).Color("red"));
    }
  }

  // Chop everything off below z = 0.
  Shape floor = Cube(1000).TranslateZ(-500);

  UnionAll(main_shapes).Subtract(floor).WriteToFile("main.scad");

  Key key;
  key.extra_z = 7;
  /*
  key.SetPosition(20, 30, 15);
  key.t().ry = 10;
  */

  double prong_height = 5;
  double prong_side_thickness = 1.5;
  Shape prong = Cube(prong_side_thickness, 4, prong_height)
                    .TranslateZ(prong_height / (-2))
                    .Add(Cylinder(4, 1, 30).RotateX(90).TranslateZ(1 - 3.2).TranslateX(.4))
                    .TranslateX(-.5 * prong_side_thickness);

  Shape fat_prong = prong + prong.TranslateX(-.5);
  double split = 10;

  Shape prong_connector = (prong + prong.MirrorX().TranslateX(split)).TranslateX(split / (-2));
  Shape prong_bottom = Cube(prong_side_thickness * 2 + split, 4, 2).TranslateZ(-1 - prong_height);
  prong_connector = prong_connector.Add(prong_bottom).TranslateZ(prong_height);

  Shape prong_hole = Cube(2, 4, 4).TranslateZ(2);
  double hole_distance = (split * .5) + 1;
  Shape bottom =
      Cube(20, 20, 2).TranslateZ(1).Subtract(Union(prong_connector,
                                                   prong_hole.TranslateX(hole_distance * -1),
                                                   prong_hole.TranslateX(hole_distance)));

  // Shape male_connector = Cube(5, 2,
  // 1).Projection().LinearExtrude(connector_params).TranslateZ(connector_params.height / 2);
  Union(bottom.TranslateX(6), prong_connector).WriteToFile("connectors.scad");

  bottom.WriteToFile("bottom.scad");
  prong_connector.WriteToFile("prong.scad");
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
