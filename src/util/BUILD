package(default_visibility = ["//visibility:public"])

cc_library(
    name = "util",
    srcs = [],
    hdrs = ["util.h"],
)

cc_library(
    name = "transform",
    srcs = [],
    hdrs = ["transform.h"],
    deps = [
        ":scad",
        "//glm",
    ],
)

cc_binary(
    name = "transform_test",
    srcs = ["transform_test.cc"],
    deps = [
        ":scad",
        ":transform",
        "//glm",
    ],
)

cc_library(
    name = "scad",
    srcs = ["scad.cc"],
    hdrs = ["scad.h"],
    deps = [
        ":util",
    ],
)
