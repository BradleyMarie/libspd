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

cc_test(
    name = "validating_spd_reader_test",
    srcs = ["validating_spd_reader_test.cc"],
    data = [
        "test_data/duplicate_wavelength.spd",
        "test_data/negative_power.spd",
        "test_data/negative_wavelength.spd",
        "test_data/power_too_large_for_float.spd",
        "test_data/wavelength_too_large_for_float.spd",
        "test_data/well_formed.spd",
        "test_data/zero_wavelength.spd",
    ],
    deps = [
        ":validating_spd_reader",
        "@bazel_tools//tools/cpp/runfiles",
        "@com_google_googletest//:gtest_main",
    ],
)
