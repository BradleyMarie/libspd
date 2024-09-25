#include "libspd/readers/emissive_spd_reader.h"

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

TEST(ReadEmissiveSpdFrom, ReadsFloat) {
  std::ifstream input = OpenRunfile("well_formed.spd");
  EXPECT_THAT(ReadEmissiveSpdFrom<float>(input).value(),
              ElementsAre(Pair(1.0, 2.0), Pair(3.0, 4.0), Pair(5.0, 6.0)));
}

TEST(ReadEmissiveSpdFrom, ReadsDouble) {
  std::ifstream input = OpenRunfile("well_formed.spd");
  EXPECT_THAT(ReadEmissiveSpdFrom<double>(input).value(),
              ElementsAre(Pair(1.0, 2.0), Pair(3.0, 4.0), Pair(5.0, 6.0)));
}

TEST(ReadEmissiveSpdFrom, ReadsLongDouble) {
  std::ifstream input = OpenRunfile("well_formed.spd");
  EXPECT_THAT(ReadEmissiveSpdFrom<long double>(input).value(),
              ElementsAre(Pair(1.0, 2.0), Pair(3.0, 4.0), Pair(5.0, 6.0)));
}

}  // namespace
}  // namespace libspd