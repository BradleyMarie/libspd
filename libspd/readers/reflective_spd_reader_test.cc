#include "libspd/readers/reflective_spd_reader.h"

#include <fstream>

#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"
#include "tools/cpp/runfiles/runfiles.h"

namespace libspd {
namespace {

using ::bazel::tools::cpp::runfiles::Runfiles;
using ::testing::ElementsAre;
using ::testing::Pair;

std::ifstream OpenRunfile(const std::string& filename) {
  std::unique_ptr<Runfiles> runfiles(Runfiles::CreateForTest());
  std::string path = "__main__/libspd/readers/test_data/" + filename;
  return std::ifstream(runfiles->Rlocation(path),
                       std::ios::in | std::ios::binary);
}

TEST(ReadReflectiveSpdFrom, TooLargeFloat) {
  std::ifstream input = OpenRunfile("well_formed.spd");
  EXPECT_EQ(
      "The input contained a sample with a spectral power greater than one",
      ReadReflectiveSpdFrom<float>(input).error());
}

TEST(ReadReflectiveSpdFrom, TooLargeDouble) {
  std::ifstream input = OpenRunfile("well_formed.spd");
  EXPECT_EQ(
      "The input contained a sample with a spectral power greater than one",
      ReadReflectiveSpdFrom<double>(input).error());
}

TEST(ReadReflectiveSpdFrom, TooLargeLongDouble) {
  std::ifstream input = OpenRunfile("well_formed.spd");
  EXPECT_EQ(
      "The input contained a sample with a spectral power greater than one",
      ReadReflectiveSpdFrom<long double>(input).error());
}

TEST(ReadReflectiveSpdFrom, ReadsFloat) {
  std::ifstream input = OpenRunfile("well_formed_reflective.spd");
  EXPECT_THAT(ReadReflectiveSpdFrom<float>(input).value(),
              ElementsAre(Pair(1.0, 1.0), Pair(3.0, 0.0), Pair(5.0, 0.5)));
}

TEST(ReadReflectiveSpdFrom, ReadsDouble) {
  std::ifstream input = OpenRunfile("well_formed_reflective.spd");
  EXPECT_THAT(ReadReflectiveSpdFrom<double>(input).value(),
              ElementsAre(Pair(1.0, 1.0), Pair(3.0, 0.0), Pair(5.0, 0.5)));
}

TEST(ReadReflectiveSpdFrom, ReadsLongDouble) {
  std::ifstream input = OpenRunfile("well_formed_reflective.spd");
  EXPECT_THAT(ReadReflectiveSpdFrom<long double>(input).value(),
              ElementsAre(Pair(1.0, 1.0), Pair(3.0, 0.0), Pair(5.0, 0.5)));
}

}  // namespace
}  // namespace libspd