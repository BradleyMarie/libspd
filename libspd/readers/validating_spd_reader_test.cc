#include "libspd/readers/validating_spd_reader.h"

#include <fstream>

#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"
#include "tools/cpp/runfiles/runfiles.h"

namespace libspd {
namespace {

using ::bazel::tools::cpp::runfiles::Runfiles;
using ::testing::_;
using ::testing::ElementsAre;
using ::testing::InSequence;
using ::testing::IsEmpty;
using ::testing::Pair;
using ::testing::Return;

class MockValidatingSpdReader final : public ValidatingSpdReader<float> {
 public:
  MOCK_METHOD((std::expected<void, std::string>), HandleComment,
              (std::string_view), (override));
  MOCK_METHOD((std::expected<void, std::string>), HandleSample,
              ((std::pair<const float, float>)&), (override));
};

std::ifstream OpenRunfile(const std::string& filename) {
  std::unique_ptr<Runfiles> runfiles(Runfiles::CreateForTest());
  std::string path = "__main__/libspd/readers/test_data/" + filename;
  return std::ifstream(runfiles->Rlocation(path),
                       std::ios::in | std::ios::binary);
}

TEST(ValidatingSpdReader, Nothing) {
  MockValidatingSpdReader spd_reader;
  EXPECT_CALL(spd_reader, HandleComment(_)).Times(0);
  EXPECT_CALL(spd_reader, HandleSample(_)).Times(0);
  EXPECT_THAT(spd_reader.Reset(), IsEmpty());
}

TEST(ValidatingSpdReader, ReturnsCommentError) {
  std::ifstream input = OpenRunfile("well_formed.spd");

  MockValidatingSpdReader spd_reader;

  EXPECT_CALL(spd_reader, HandleComment("Comment"))
      .WillOnce(Return(std::unexpected("error")));
  EXPECT_CALL(spd_reader, HandleSample(_)).Times(0);

  EXPECT_EQ("error", spd_reader.ReadFrom(input).error());
}

TEST(ValidatingSpdReader, ReturnsSampleError) {
  std::ifstream input = OpenRunfile("well_formed.spd");

  MockValidatingSpdReader spd_reader;

  EXPECT_CALL(spd_reader, HandleComment("Comment"))
      .WillOnce(Return(std::expected<void, std::string>()));
  EXPECT_CALL(spd_reader, HandleSample(Pair(1.0, 2.0)))
      .WillOnce(Return(std::unexpected("error")));

  EXPECT_EQ("error", spd_reader.ReadFrom(input).error());
}

TEST(ValidatingSpdReader, WellFormed) {
  std::ifstream input = OpenRunfile("well_formed.spd");

  MockValidatingSpdReader spd_reader;

  {
    InSequence sequence;
    EXPECT_CALL(spd_reader, HandleComment("Comment"))
        .WillOnce(Return(std::expected<void, std::string>()));
    EXPECT_CALL(spd_reader, HandleSample(Pair(1.0, 2.0)))
        .WillOnce(Return(std::expected<void, std::string>()));
    EXPECT_CALL(spd_reader, HandleSample(Pair(5.0, 6.0)))
        .WillOnce(Return(std::expected<void, std::string>()));
    EXPECT_CALL(spd_reader, HandleSample(Pair(3.0, 4.0)))
        .WillOnce(Return(std::expected<void, std::string>()));
  }

  EXPECT_THAT(spd_reader.Reset(), IsEmpty());
  EXPECT_TRUE(spd_reader.ReadFrom(input));
  EXPECT_THAT(spd_reader.Reset(),
              ElementsAre(Pair(1.0, 2.0), Pair(3.0, 4.0), Pair(5.0, 6.0)));
  EXPECT_THAT(spd_reader.Reset(), IsEmpty());
}

TEST(ValidatingSpdReader, NegativeWavelength) {
  std::ifstream input = OpenRunfile("negative_wavelength.spd");

  MockValidatingSpdReader spd_reader;
  EXPECT_CALL(spd_reader, HandleComment(_)).Times(0);
  EXPECT_CALL(spd_reader, HandleSample(_)).Times(0);
  EXPECT_EQ("The input contained a sample with a negative wavelength",
            spd_reader.ReadFrom(input).error());
}

TEST(ValidatingSpdReader, NegativeSpectralPower) {
  std::ifstream input = OpenRunfile("negative_power.spd");

  MockValidatingSpdReader spd_reader;
  EXPECT_CALL(spd_reader, HandleComment(_)).Times(0);
  EXPECT_CALL(spd_reader, HandleSample(_)).Times(0);
  EXPECT_EQ("The input contained a sample with a negative spectral power",
            spd_reader.ReadFrom(input).error());
}

TEST(ValidatingSpdReader, ZeroWavelength) {
  std::ifstream input = OpenRunfile("zero_wavelength.spd");

  MockValidatingSpdReader spd_reader;
  EXPECT_CALL(spd_reader, HandleComment(_)).Times(0);
  EXPECT_CALL(spd_reader, HandleSample(_)).Times(0);
  EXPECT_EQ("The input contained a sample with a wavelength of zero",
            spd_reader.ReadFrom(input).error());
}

TEST(ValidatingSpdReader, NonFiniteWavelength) {
  std::ifstream input = OpenRunfile("wavelength_too_large_for_float.spd");

  MockValidatingSpdReader spd_reader;
  EXPECT_CALL(spd_reader, HandleComment(_)).Times(0);
  EXPECT_CALL(spd_reader, HandleSample(_)).Times(0);
  EXPECT_EQ("The input contained a sample with a non-finite wavelength",
            spd_reader.ReadFrom(input).error());
}

TEST(ValidatingSpdReader, NonFiniteSpectralPower) {
  std::ifstream input = OpenRunfile("power_too_large_for_float.spd");

  MockValidatingSpdReader spd_reader;
  EXPECT_CALL(spd_reader, HandleComment(_)).Times(0);
  EXPECT_CALL(spd_reader, HandleSample(_)).Times(0);
  EXPECT_EQ("The input contained a sample with a non-finite spectral power",
            spd_reader.ReadFrom(input).error());
}

TEST(ValidatingSpdReader, DuplicateWavelength) {
  std::ifstream input = OpenRunfile("duplicate_wavelength.spd");

  MockValidatingSpdReader spd_reader;
  EXPECT_CALL(spd_reader, HandleComment(_)).Times(0);
  EXPECT_CALL(spd_reader, HandleSample(Pair(1.0, 0.0)))
      .WillOnce(Return(std::expected<void, std::string>()));
  EXPECT_EQ("The input contained multiple samples with the same wavelength",
            spd_reader.ReadFrom(input).error());
}

}  // namespace
}  // namespace libspd