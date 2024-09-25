load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "spd_reader",
    srcs = ["spd_reader.cc"],
    hdrs = ["spd_reader.h"],
)

cc_test(
    name = "spd_reader_test",
    srcs = ["spd_reader_test.cc"],
    data = [
        "test_data/comment_only.spd",
        "test_data/empty.spd",
        "test_data/mismatched_line_endings.spd",
        "test_data/odd_number_of_tokens.spd",
        "test_data/raw_with_comments.spd",
        "test_data/single_line.spd",
        "test_data/unparsable_token.spd",
        "test_data/valid_mac.spd",
        "test_data/valid_unix.spd",
        "test_data/valid_windows.spd",
    ],
    deps = [
        ":spd_reader",
        "@bazel_tools//tools/cpp/runfiles",
        "@com_google_googletest//:gtest_main",
    ],
)
