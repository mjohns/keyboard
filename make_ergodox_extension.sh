./build.sh ergodox_extension
if [ $? -ne 0 ]; then
  echo "Failed to build"
  exit 1
fi

echo "Generating STL"
openscad -o things/ergodox_extension_left.stl out/ergodox_extension_left.scad

echo "Generating right STL"
openscad -o things/ergodox_extension_right.stl out/ergodox_extension_right.scad
