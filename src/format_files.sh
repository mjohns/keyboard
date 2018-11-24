#!/bin/bash

clang-format --style=file -i util/*.h util/*.cc keyboards/*.cc
buildifier glm/BUILD
buildifier util/BUILD
buildifier keyboards/BUILD
