#include <glm/glm.hpp>
#include <vector>

#include "key.h"
#include "scad.h"
#include "transform.h"

template <typename T>
void PushBackAll(std::vector<T>* v, const std::vector<T>& to_add) {
  v->insert(v->end(), to_add.begin(), to_add.end());
}
