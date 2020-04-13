This repo defines utilities for generating keyboard related OpenSCAD files
 using c++ and is heavily inspired by the
 [dactyl](https://github.com/adereth/dactyl-keyboard) and
 [dactyl_manuform](https://github.com/abstracthat/dactyl-manuform). The key
 switch shapes and general positioning of the keys was derived from those
 projects for the version of the dactyl_manuform included.

```
// To generate the ergodox extension.
./build ergodox_extension
ls out/ergodox*.scad

// To generate dactyl manuform.
./build dactyl_manuform
ls out/dactyl_manuform.scad
```
