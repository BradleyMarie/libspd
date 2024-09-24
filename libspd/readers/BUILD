load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "emissive_spd_reader",
    srcs = ["emissive_spd_reader.cc"],
    hdrs = ["emissive_spd_reader.h"],
    deps = [
        ":validating_spd_reader",
    ],
)

cc_library(
    name = "reflective_spd_reader",
    srcs = ["reflective_spd_reader.cc"],
    hdrs = ["reflective_spd_reader.h"],
    deps = [
        ":validating_spd_reader",
    ],
)

cc_library(
    name = "validating_spd_reader",
    hdrs = ["validating_spd_reader.h"],
    deps = [
        "//libspd:spd_reader",
    ],
)
