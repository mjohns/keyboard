#!/bin/bash

if [ "$#" -lt 1 ]; then
	echo "Must specify target to build. ex: ergodox_extension"
  exit 1
fi

if [ ! -f src/$1.cc ]; then
	echo "src/$1.cc does not exist"
fi

if [ ! -d out ]; then
	mkdir out
fi
cd out

if [ ! -f scad.o ]; then
	echo "Building util"
	g++ -c -std=c++17 ../src/util/*.cc -I../src/util -I../src/glm/v0.9.9.2
	if [ $? -ne 0 ]; then
		echo "Failed to build util"
		exit 1
	fi
fi

echo "Building \"$1\""
g++ -std=c++17 ../src/$1.cc *.o -I../src/util -I../src/glm/v0.9.9.2 -o $1
if [ $? -ne 0 ]; then
  echo "Failed to build"
  exit 1
fi
echo "Running"
./$1 && echo "Done"
