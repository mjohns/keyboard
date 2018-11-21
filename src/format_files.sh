#!/bin/bash

clang-format --style=file -i util/*.h util/*.cc
buildifier glm/BUILD
buildifier util/BUILD
